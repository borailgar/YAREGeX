#include "NfaMatcher.hpp"
#include "Rgx2Nfa.hpp"
#include <iostream>
#ifdef LDEBUG
#include "Benchmark.hpp"
#endif

using namespace lambda;
int main(void)
{
    StatePtr_t nfa = make_nfa({"a.(b|c)*"});
    RgxString rgx{"a.(b|c)*"};
    std::cout << rgx << std::endl;
    return 0;
}
