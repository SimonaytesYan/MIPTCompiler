#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("{ let a = 123 + 444;"
                            "let b = (-10) / 1; }");
    std::istringstream in2("{ let a = 123 + 444;"
                            "if (a - 10) { let b = 11; } else { let b = 12; }"
                            "}");

    GraphicDumpPass pass("LexerScopeDump");

    GrammarUnit* unit = parse(tokenize(in1));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in2));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";
}
