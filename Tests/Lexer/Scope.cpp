#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("{ let a = 123 + 444;"
                            "let b = (-10) / 1; }");

    GraphicDumpPass pass("LexerScopeDump");

    GrammarUnit* unit = parse(tokenize(in1));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

}
