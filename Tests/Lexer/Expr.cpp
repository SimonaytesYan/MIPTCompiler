#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("123 + 444");
    std::istringstream in2("123 - 3434");
    std::istringstream in3("23 * 10");
    std::istringstream in4("23 / 10");
    std::istringstream in5("a + b");
    std::istringstream in6("(a + b) * c");
    std::istringstream in7("a + b + c");
    std::istringstream in8("(a + b) * c / 10 - 11 + ((-10))");

    GraphicDumpPass pass("LexerExprDump");
    // pass.graphicDump(parse(tokenize(in1)));
    // std::cout << "\n";
    // std::cerr << "\n";
    // pass.graphicDump(parse(tokenize(in2)));
    // std::cout << "\n";
    // std::cerr << "\n";
    // pass.graphicDump(parse(tokenize(in3)));
    // std::cout << "\n";
    // std::cerr << "\n";
    // pass.graphicDump(parse(tokenize(in4)));
    // std::cout << "\n";
    // std::cerr << "\n";
    // pass.graphicDump(parse(tokenize(in5)));
    // std::cout << "\n";
    // std::cerr << "\n";
    pass.graphicDump(parse(tokenize(in6)));
    std::cout << "\n";
    std::cerr << "\n";
    pass.graphicDump(parse(tokenize(in7)));
    std::cout << "\n";
    std::cerr << "\n";
    pass.graphicDump(parse(tokenize(in8)));
    std::cout << "\n";
    std::cerr << "\n";
}
