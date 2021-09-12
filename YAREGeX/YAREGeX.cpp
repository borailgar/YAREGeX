#include "Rgx2Nfa.hpp"

#ifdef LDEBUG
#include "Benchmark.hpp"
#include <iostream>
#endif

using namespace lambda;
int main(void)
{
    StatePtr_t nfa = make_nfa({"a.(a|b)*"});

    return 0;
}
