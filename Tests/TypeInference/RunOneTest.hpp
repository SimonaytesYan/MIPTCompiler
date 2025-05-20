#pragma once

#include <iostream>
#include <sstream>
#include <string_view>

#include "Tokenizer.hpp"
#include "Grammar.hpp"
#include "GraphicDumpPass.hpp"
#include "TypeInference.hpp"
#include "Lexer.hpp"

void runOneTest(std::string test, GraphicDumpPass& pass) {
    std::istringstream in(test);

    TypeSystem system;
    GraphicDumpPass dump;

    GrammarUnit* root = parse(tokenize(in));
    GrammarUnit* new_roop = system.inferTypes(root);
    
    std::cout << "test " << test << "\n";
    std::cout << "is successfull " << system.isSuccessfull() << "\n";
    pass.graphicDump(root);
    
}