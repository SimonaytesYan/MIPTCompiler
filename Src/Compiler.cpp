#include <iostream>
#include <fstream>

#include "Headers/Tokenizer.hpp"
#include "Headers/Lexer.hpp"
#include "Headers/TypeInference.hpp"
#include "Headers/IRBuilderPass.hpp"

int main(int argc, char** argv) {

    if (argc != 2) {
        std::cerr << "Wrong command line argument number\n";
        return -1;
    }

    std::ifstream in(argv[1]);
    if (in.fail()) {
        std::cerr << "Error during file open\n";
        return -1;
    }
    GrammarUnit* unit = parse(tokenize(in));
    TypeSystem type_sys;
    unit = type_sys.inferTypes(unit);

    // Put llvm IR into file
    IRBuilderPass pass;

    std::string output_file_name = argv[1];
    output_file_name += ".ll";
    pass.buildAndDumpIR(unit, output_file_name);

    // Delete units
    recursiveUnitDelete(unit);

    // Compile and link with std library
    std::string command = "clang++ " + output_file_name + " obj/StdLib.o -o a.out";
    system(command.c_str());
}