#pragma once

/**
 * C++ implementation of Subset Construction
 * Compiles NFA states to DFA representation.
 * For detail see: Keith D. Cooper & Linda Torczon's Engineering a Compiler book.
 *
 * Author   : Bora Ilgar
 * Version  : 0.9.1
 */

#include "../utility/yaregex_common.h"
#include "NfaMatcher.hpp"

/**
 * The algorithm, shown in Figure 2.6, constructs a set Q whose elements, qi
 * are Valid configuration each a subset of N, that is, each qi 2 2N . When the algorithm halts, each
 * configuration of an NFA that can be reached by some input string
 * qi 2 Q corresponds to a state, di 2 D, in the dfa. The construction builds the
 * elements of Q by following the transitions that the nfa can make on a given
 * input. Thus, each qi represents a valid configuration of the nfa.
 *
 * The algorithm begins with an initial set, q0, that contains n0 and any states
 * in the nfa that can be reached from n0 along paths that contain only
 *
 *  - Engineering a Compiler Chapter-2 (Subset construction)
 */

namespace lambda
{
using StatePtrVec_t = std::vector<StatePtr_t>;
using T = std::pair<StatePtrVec_t, int8_t>;
struct DState
{
    DState(StatePtrVec_t &l)
    {
    }

  private:
    std::map<T, StatePtrVec_t> m_Next;
};

} // namespace lambda
