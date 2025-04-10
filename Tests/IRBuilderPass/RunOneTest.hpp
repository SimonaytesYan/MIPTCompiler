#pragma once

#include <algorithm>
#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/IRBuilderPass.hpp"

void runOneTest(std::string_view test_case, const std::string_view test_name) {
    std::cout << "TEST: " << test_name << "\n";
    std::cout << "code:\t" << test_case << "\n";

    IRBuilderPass pass;
    std::istringstream in(test_case.data());
    GrammarUnit* unit = parse(tokenize(in));

    // Put llvm IR into file
    std::string output_file_name = test_name.data();
    std::replace(output_file_name.begin(), output_file_name.end(), ' ', '_');
    output_file_name += ".ll";

    pass.buildAndDumpIR(unit, output_file_name);

    // Put llvm IR into std output
    // std::cout << "IR:\n";
    // pass.buildAndDumpIR(unit, "");

    // Delete all functions
    recursiveUnitDelete(unit);

    // Run program
    std::cout << "Program output:\n";

    std::string command = "Src/Compiler.sh " + output_file_name;
    system(command.c_str());
    system("bin/a.out");

    std::cout << "======================================\n";
}