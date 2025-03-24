#include <iostream>
#include <fstream>

#include "Headers/Tokenizer.hpp"
#include "Headers/Lexer.hpp"
#include "Headers/ExecutionPass.hpp"

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

    ExecutionPass pass;
    pass.execute(unit);

    recursiveUnitDelete(unit);
}