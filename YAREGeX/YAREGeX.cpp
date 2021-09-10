#include "Nfa2Dfa.hpp"
#include "NfaMatcher.hpp"
#include <chrono>
#include <iostream>
#ifdef LDEBUG
#include "Benchmark.hpp"
#endif

int main(void)
{
    auto nfa = lambda::make_nfa({"a.(a|b)*"});
    lambda::RgxMatch rgx_match(nfa);
    if (rgx_match.match("abab"))
    {
        std::cout << "YES\n";
    }
    else
    {
        std::cout << "NO\n";
    }

    return 0;
}
