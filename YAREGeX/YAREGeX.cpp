#include "FSM/NfaMatcher.hpp"
#include "FSM/Rgx2Nfa.hpp"
#include <iostream>

int main(void)
{
    auto nfa = lambda::make_nfa({"a.(a+b)*.b"});
    lambda::RgxMatch rgxMatch(nfa);

    if (rgxMatch.match("abba"))
    {
        // ...
    }
    else
    {
        // does not match with given string
    }

    return 0;
}
