#pragma once

#include <iostream>
#include <sstream>
#include <string_view>

#include "Tokenizer.hpp"
#include "Grammar.hpp"
#include "GraphicDumpPass.hpp"
#include "TypeInference.hpp"


void runOneTest(std::string test, GraphicDumpPass& pass) {
    std::istringstream in(test);

    TypeSystem system;
    GraphicDumpPass dump;

    GrammarUnit* root = parse(tokenize(in));
    system.inferTypes(root);
    pass.graphicDump(root);

    std::cout << "is successfull " << system.isSuccessfull() << "\n";
    
}