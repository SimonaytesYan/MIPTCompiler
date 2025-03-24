#pragma once

#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/ExecutionPass.hpp"

void runOneTest(std::string_view test_case, ExecutionPass& pass, std::string_view test_name) {
    std::cout << "TEST: " << test_name << "\n";
    std::cout << "code:\t" << test_case << "\n";

    std::istringstream in(test_case.data());

    GrammarUnit* unit = parse(tokenize(in));
    pass.execute(unit);
    recursiveUnitDelete(unit);

    std::cout << "\n";
    std::cerr << "\n";
}