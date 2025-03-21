#pragma once

#include <sstream>
#include <iostream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/ExecutionPass.hpp"

void runOneTest(const char* test_case, ExecutionPass& pass) {
    static size_t test_cnt = 0;
    test_cnt++;

    std::cout << "Test " << test_cnt << "\n";

    std::istringstream in(test_case);

    GrammarUnit* unit = parse(tokenize(in));
    pass.execute(unit);
    recursiveUnitDelete(unit);

    std::cout << "\n";
    std::cerr << "\n";
}