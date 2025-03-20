#pragma once

#include <sstream>
#include <iostream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

void runOneTest(const char* test_case, GraphicDumpPass& pass) {
    std::istringstream in(test_case);

    GrammarUnit* unit = parse(tokenize(in));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);

    std::cout << "\n";
    std::cerr << "\n";
}