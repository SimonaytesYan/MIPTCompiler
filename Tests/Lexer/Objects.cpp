#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("{let a = 123}");
    std::istringstream in2("{let b = -123}");
    std::istringstream in3("{let c = Abcd; }");

    GraphicDumpPass pass("LexerObjDump");
    pass.graphicDump(parse(tokenize(in1)));
    std::cout << "\n";
    std::cerr << "\n";
    pass.graphicDump(parse(tokenize(in2)));
    std::cout << "\n";
    std::cerr << "\n";
    pass.graphicDump(parse(tokenize(in3)));
    std::cout << "\n";
    std::cerr << "\n";
}
