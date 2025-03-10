#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("abcd");
    std::istringstream in2("a_bcd");
    std::istringstream in3("Abcd");
    std::istringstream in4("_AbdF");
    std::istringstream in5("_A4");
    std::istringstream in6("4ad");

    GraphicDumpPass pass;
    pass.graphicDump(parse(tokenize(in1)));
    pass.graphicDump(parse(tokenize(in2)));
    pass.graphicDump(parse(tokenize(in3)));
    pass.graphicDump(parse(tokenize(in4)));
    pass.graphicDump(parse(tokenize(in5)));
    pass.graphicDump(parse(tokenize(in6)));
}
