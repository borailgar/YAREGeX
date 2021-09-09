#pragma once

/**
 * C++ implementation of shunting-yard algorithm
 * Compiles regular expression to postfix notation.
 * Algorithm's wiki page        : https://en.wikipedia.org/wiki/Shunting-yard_algorithm
 * Theodore Norvell's article   : https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
 *
 * Author   : Bora Ilgar
 * Version  : 0.9.1
 */

#include <array>
#include <cassert>
#include <deque>
#include <ostream>
#include <stack>

#define _IN_

#ifdef LDEBUG
#include "Benchmark.hpp"
#include <iostream>
#endif

// Input   -> Intermediate  -> RESULT
// =========================================
// a(bb)+a  = a.(b.b)+.a    -> abb.+a.
// a(a+b)*b = a.(a+b)*.b    -> aab+*.b.
// a+b      = ab+

namespace lambda
{

struct RgxString
{
    struct Token
    {
        // Operators
        enum class operator_type : int16_t
        {
            ALPHABET = 0,  //[a-z]|[A-Z]
            L_PARANTHESIS, //(
            R_PARANTHESIS, //)

            UNION,        // |
            CONCAT,       // .
            ONE_OR_MORE,  // +
            ZERO_OR_MORE, // ?
            CLOSURE       // *
        };

        Token() = default;
        Token(const operator_type &t, uint8_t ch, int pre = -1) : m_OpType(t), m_Ch(ch), m_Presedence(pre)
        {
        }

        operator_type m_OpType;
        uint8_t m_Ch;
        int m_Presedence;
    };

    // Prepare tokens by all char elem by it' s presedence and types
    // All the token class instances stored in a fixed-size array so iterate this array later
    template <size_t CArraySize> RgxString(const char (&ar)[CArraySize])
    {
        std::array<Token, CArraySize> tokens;

        for (int idx{0}; idx < CArraySize; ++idx)
        {
            if (is_letter(ar[idx]))
            {
                Token tkn(Token::operator_type::ALPHABET, ar[idx]);
                tokens[idx] = tkn;
            }

            if (ar[idx] == '(')
            {
                Token tkn(Token::operator_type::L_PARANTHESIS, ar[idx]);
                tokens[idx] = tkn;
            }
            if (ar[idx] == ')')
            {
                Token tkn(Token::operator_type::R_PARANTHESIS, ar[idx]);
                tokens[idx] = tkn;
            }
            if (ar[idx] == '*')
            {
                Token tkn(Token::operator_type::CLOSURE, ar[idx], 4);
                tokens[idx] = tkn;
            }
            if (ar[idx] == '|')
            {
                Token tkn(Token::operator_type::UNION, ar[idx], 1);
                tokens[idx] = tkn;
            }
            if (ar[idx] == '?')
            {
                Token tkn(Token::operator_type::ZERO_OR_MORE, ar[idx], 3);
                tokens[idx] = tkn;
            }
            if (ar[idx] == '+')
            {
                Token tkn(Token::operator_type::ONE_OR_MORE, ar[idx], 3);
                tokens[idx] = tkn;
            }
            // The concatenation expression usually does not have a symbol between the two letters ab. This will make
            // our computation more difficult than necessary when compute the conversation. So, in order to easily
            // handle this will be using an . symbol between the two letters for every concatenation.
            // So, ab will now turn into a.b
            if (ar[idx] == '.')
            {
                Token tkn(Token::operator_type::CONCAT, ar[idx], 2);
                tokens[idx] = tkn;
            }
        }
        calculate_postfix_from(tokens);
    }

    // Wrapping up std::deque iterator
    using DQType = std::deque<char>;
    DQType::iterator begin()
    {
        return m_Output.begin();
    }
    DQType::iterator end()
    {
        return m_Output.end();
    }

    friend std::ostream &operator<<(std::ostream &os, const RgxString &rString);

  private:
    // The Operator stack will hold all of the operators that pass throughand respond to new operators by following
    // the rules we used in the previous section. The Output queue will be the final postfix notation.
    template <size_t N> void calculate_postfix_from(_IN_ std::array<Token, N> const &arr)
    {
#ifdef LDEBUG
        PROFILE_FUNCTION();
#endif // LDEBUG

        for (const Token &tkn : arr)
        {
            if (tkn.m_OpType == Token::operator_type::ALPHABET)
            {
                m_Output.push_back(tkn.m_Ch);
            }

            if (tkn.m_OpType == Token::operator_type::CLOSURE || tkn.m_OpType == Token::operator_type::CONCAT ||
                tkn.m_OpType == Token::operator_type::ONE_OR_MORE || tkn.m_OpType == Token::operator_type::UNION ||
                tkn.m_OpType == Token::operator_type::ZERO_OR_MORE)
            {
                while (!m_TokenStack.empty())
                {
                    auto el = m_TokenStack.top();
                    if (el.m_Presedence >= tkn.m_Presedence)
                    {
                        m_TokenStack.pop();
                        m_Output.push_back(el.m_Ch);
                        continue;
                    }
                    break;
                }
                m_TokenStack.push(tkn);
            }

            if (tkn.m_OpType == Token::operator_type::L_PARANTHESIS)
            {
                m_TokenStack.push(tkn);
            }

            if (tkn.m_OpType == Token::operator_type::R_PARANTHESIS)
            {
                while (!m_TokenStack.empty() && m_TokenStack.top().m_Ch != '(')
                {
                    m_Output.push_back(m_TokenStack.top().m_Ch);
                    m_TokenStack.pop();
                }
                m_TokenStack.pop();
            }
        }
        if (!m_TokenStack.empty())
        {
            m_Output.push_back(m_TokenStack.top().m_Ch);
            m_TokenStack.pop();
        }
    }

    bool is_letter(const char ch)
    {
        return (ch <= 'z' && ch >= 'a') || (ch <= 'Z' && ch >= 'A');
    }

  private:
    std::stack<Token> m_TokenStack;
    DQType m_Output;
};

std::ostream &operator<<(std::ostream &os, RgxString &rString)
{
    for (auto &ch : rString)
    {
        os << ch;
    }
    return os;
}

} // namespace lambda
