#include "NfaMatcher.hpp"
#include <chrono>

#include <iostream>
#ifdef LDEBUG
#include "Benchmark.hpp"
#endif

int main(void)
{
#if LDEBUG
    Instrumentor::Get().BeginSession("OLD_REGEX_MATCH");
#endif
    auto nfa_state = lambda::make_nfa({"a.(b.b)+.a"});
    lambda::RgxMatch rgx_match(nfa_state);
    if (rgx_match.match("abba"))
    {
        std::cout << "YES\n";
    }
    else
    {
        std::cout << "NO\n";
    }

#if LDEBUG
    Instrumentor::Get().EndSession();
#endif

    return 0;
}
