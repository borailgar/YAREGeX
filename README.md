# YAREGeX
Yet another simple Regular Expression matching library. 
Compiles NFA from regex string and simulates NFA using Thompson's algorithm. 

Implements 2/3 states: 
1. [x] Infix notation to postfix
    - Shunting-yard algorithm
    - https://www.engr.mun.ca/~theo/Misc/exp_parsing.htm
2. [x] Regular Expression to NFA
    - [Regular Expression Matching Can Be Simple and Fast](https://swtch.com/~rsc/regexp/regexp1.html)
    - Thompson's construction 
3. [ ] NFA to DFA (not yet)

#### Test
```cpp
    // Crates NFA state from infix regex string
    auto nfa_state = lambda::make_nfa({"a.(a+b)*.b"});
    // Initialize matching state 
    lambda::RgxMatch rgx_match(nfa_state);
    if(rgx_match.match("abba")) {
        // ...
    } else{
        // does not match with given string
        return;
    }
```
