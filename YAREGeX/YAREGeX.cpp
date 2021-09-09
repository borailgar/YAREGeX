#include "NfaMatcher.hpp"
#include <chrono>

#ifdef LDEBUG
#include "Benchmark.hpp"
#include <iostream>
#endif

int main(void)
{
#if LDEBUG
    Instrumentor::Get().BeginSession("Regex Match");
#endif
    auto nfa_state = lambda::make_nfa({"a.(a+b)*.b"});
    lambda::RgxMatch rgx_match(nfa_state);
    rgx_match.match("abba");

#if LDEBUG
    Instrumentor::Get().EndSession();
#endif

    return 0;
}
