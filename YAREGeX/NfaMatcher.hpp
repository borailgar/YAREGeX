#pragma once

/**
 * C++ Wrapper and implementation of Regular Expression Matching
 * Simulates NFA using Thompson's algorithm (Thompson, Ken.  Regular Expression Search Algorithm).
 * Supports (|) * + ? .
 * For detail see: https://swtch.com/~rsc/regexp/regexp1.html
 *
 * Author   : Bora Ilgar
 * Version  : 0.9.1
 *
 */

#include "Rgx2Nfa.hpp"
#include <vector>

#ifdef LDEBUG
#include "Benchmark.hpp"
#include <iostream>
#endif

namespace lambda
{

struct RgxMatch
{
    RgxMatch(StatePtr_t &start) : m_Start(start)
    {
    }

    // The simulation uses two lists : current_list and next_list
    // current_list : is the current set if states that the NFA is in
    // next_list    : is the set set if states that the NFA ** will be ** in after processing the character
    // The execution loop initializes current_list to contain just the start state
    // and then runs the machine one step a time.

    // To avoid allocation on every iteration of the loop, match uses two pre-allocated lists list1 and list2 as
    // current_list and next_list.
    bool match(const std::string &checkStr)
    {
#ifdef LDEBUG
        PROFILE_FUNCTION();
#endif

        init(m_Start, curr);
        for (const auto &ch : checkStr)
        {
            step(curr, ch, next);
            std::swap(curr, next);
        }

        return is_match(curr);
    }

  private:
    // If the final state list contain *match-state* the the string matches.
    bool is_match(std::vector<StatePtr_t> &sHolder)
    {
        for (auto &state : sHolder)
        {
            if (state->ch == static_cast<int>(State::Type::Match))
            {
                return true;
            }
        }
        return false;
    }

    // add_state(...) adds a state to the list(stateHolder), but if not already on the list.
    // Scanning entire character for each _add_ operation would be inneficient
    // instead the variable *list_id* acts as a list generation number.
    // When add_state adds state to a list, it records in __state->lastList = list_id__
    // If the two are already equal, then state is already on the list being built.
    // add_state(...) also follows unlabeled ptr_arrows;
    // if state is a SPLIT state with two unlabeld ptr_arrows to new states,
    // add_state(...) add those states to the list instead of state
    void add_state(std::vector<StatePtr_t> &nextHolder, StatePtr_t &state)
    {
#ifdef LDEBUG
        PROFILE_FUNCTION();
#endif
        if (state == nullptr || state->last_list == m_ListID)
        {
            return;
        }

        state->last_list = m_ListID;
        if (state->ch == static_cast<int>(State::Type::Split))
        {
            add_state(nextHolder, state->next0);
            add_state(nextHolder, state->next1);
            return;
        }
        nextHolder.push_back(state);
    }

    // initial_state creates an initial state list by adding just a start state
    auto init(StatePtr_t &state, std::vector<StatePtr_t> &currHolder) -> decltype(currHolder)
    {
        m_ListID++;
        add_state(currHolder, state);
        return currHolder;
    }

    // Finally, step advances NFA past a single character, using the current list (currentHolder)
    // to compute the next list (nextHolder)
    void step(std::vector<StatePtr_t> &currentHolder, int8_t ch, std::vector<StatePtr_t> &nextHolder)
    {
#ifdef LDEBUG
        PROFILE_FUNCTION();
#endif
        std::shared_ptr<State> state;
        m_ListID++;

        for (auto &curr_state : currentHolder)
        {
            state = curr_state;
            if (state->ch == ch)
            {
                add_state(nextHolder, state->next0);
            }
        }
    }

  private:
    // NFA has been built, we need to simulate it.
    // The simulation requires tracking State sets, which are stored as a simple vector:
    uint32_t m_ListID{0};
    std::vector<StatePtr_t> curr;
    std::vector<StatePtr_t> next;

    StatePtr_t m_Start;
};

} // namespace lambda
