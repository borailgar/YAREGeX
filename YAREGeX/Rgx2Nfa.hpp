#pragma once

/**
 * C++ wrapper and implementation of Regular Expression Matching Can Be Simple and Fast article
 * Compiles regular expression to NFA. Supports (|) * + ? .
 * For detail see the article https://swtch.com/~rsc/regexp/regexp1.html
 *
 * Author   : Bora Ilgar
 * Version  : 0.9.1
 */

#include "Rgx2Postfix.hpp"
#include <cassert>
#include <memory>

#ifdef LDEBUG
#include "Benchmark.hpp"
#include <iostream>
#endif

namespace lambda
{

// Represents State and it's StateType
// According the scenario, state object created with two different ways
// in StateType == Split case: other pointer (next1) may be used for searching.
// in StateType == Match case: represents matched states in created NFA-state.
struct State
{
    enum class Type : uint32_t
    {
        Split = 256,
        Match = 257
    };

    State(const int data) : ch(data), next0(nullptr), next1(nullptr)
    {
    }
    State(const int data, const std::shared_ptr<State> &n0, const std::shared_ptr<State> &n1)
        : ch(data), next0(n0), next1(n1)
    {
    }

    int ch;
    int last_list{0};
    std::shared_ptr<State> next0;
    std::shared_ptr<State> next1;
};

// If any non - static data member of a union has a
// non - trivial default constructor(12.1),
// copy constructor(12.8),
// move constructor(12.8),
// copy assignment operator(12.8),
// move assignment operator(12.8), or destructor(12.4),
// the corresponding member function of the union must be user - provided or it will be implicitly *deleted*

union StateList {
    ~StateList()
    {
    }
    std::shared_ptr<State> start;
    StateList *next;
};

using StatePtr_t = std::shared_ptr<State>;
template <typename T, typename U, typename... Args> StatePtr_t make_state(const U data, Args &&...arg)
{
    return std::make_shared<T>(static_cast<int>(data), std::forward<Args>(arg)...);
}

// Holds state and it's stateList object.
// Represents single NFA state.
struct NFA
{
    NFA(StatePtr_t &state, StateList *sl) : start(state), state_list(sl)
    {
    }
    StateList *state_list;
    StatePtr_t start;
};

// singleton-class for modifications on state-list(s)
struct StateHelper
{
    StateHelper(const StateHelper &) = delete;
    StateHelper &operator=(const StateHelper &) = delete;

    static StateHelper &Funcs()
    {
        static StateHelper sInstance;
        return sInstance;
    }

    // NOT initializing for state-list
    // Only holds an address for indicated state.
    StateList *create_list(StatePtr_t *state)
    {
        StateList *stateList = reinterpret_cast<StateList *>(state);
        stateList->next = nullptr;
        return stateList;
    }

    // Add state to state-list.
    void patch_list(StateList *state_list, StatePtr_t &state)
    {
        StateList *temp;
        while (state_list)
        {
            temp = state_list->next;
            state_list->start = state;
            state_list = temp;
        }
    }

    // Merge two list of state.
    StateList *append_list(StateList *sList0, StateList *sList1)
    {
        StateList *iter;
        iter = sList0;
        while (sList0->next)
        {
            sList0 = sList0->next;
        }

        sList0->next = sList1;
        return iter;
    }

  private:
    StateHelper()
    {
    }
};

StatePtr_t make_nfa(lambda::RgxString &&postRegex)
{
#ifdef LDEBUG
    std::cout << "postRegex = " << postRegex << std::endl;
    PROFILE_FUNCTION();
#endif

    std::stack<NFA> nfa_stack;
    for (auto &&ch : postRegex)
    {
        if (ch >= 'a' && ch <= 'z')
        {
            auto statePtr = make_state<State>(ch);
            nfa_stack.emplace(statePtr, StateHelper::Funcs().create_list(std::addressof(statePtr->next0)));
        }
        if (ch == '.')
        {
            auto nfa1 = nfa_stack.top();
            nfa_stack.pop();

            auto nfa0 = nfa_stack.top();
            nfa_stack.pop();

            StateHelper::Funcs().patch_list(nfa0.state_list, nfa1.start);
            nfa_stack.emplace(nfa0.start, nfa1.state_list);
        }

        if (ch == '+')
        {
            auto nfa0 = nfa_stack.top();
            nfa_stack.pop();

            auto statePtr = make_state<State>(State::Type::Split, nfa0.start, nullptr);
            StateHelper::Funcs().patch_list(nfa0.state_list, statePtr);
            nfa_stack.emplace(statePtr, StateHelper::Funcs().create_list(std::addressof(statePtr->next1)));
        }

        if (ch == '|')
        {
            auto nfa1 = nfa_stack.top();
            nfa_stack.pop();

            auto nfa0 = nfa_stack.top();
            nfa_stack.pop();

            auto state = make_state<State>(State::Type::Split, nfa0.start, nfa1.start);
            nfa_stack.emplace(state, StateHelper::Funcs().append_list(nfa0.state_list, nfa1.state_list));
        }

        if (ch == '*')
        {
            auto nfa0 = nfa_stack.top();
            nfa_stack.pop();
            auto state = make_state<State>(State::Type::Split, nfa0.start, nullptr);
            StateHelper::Funcs().patch_list(nfa0.state_list, state);
            nfa_stack.emplace(state, StateHelper::Funcs().create_list(std::addressof(state->next1)));
        }

        if (ch == '?')
        {
            auto nfa0 = nfa_stack.top();
            nfa_stack.pop();
            auto state = make_state<State>(State::Type::Split, nfa0.start, nullptr);
            nfa_stack.emplace(state,
                              StateHelper::Funcs().append_list(
                                  nfa0.state_list, StateHelper::Funcs().create_list(std::addressof(state->next1))));
        }
    }
    auto state = nfa_stack.top();
    nfa_stack.pop();

    auto match_state{make_state<State>(State::Type::Match)};
    StateHelper::Funcs().patch_list(state.state_list, match_state);

    return state.start;
}

} // namespace lambda
