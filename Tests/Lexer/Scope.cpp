#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("{}");
    std::istringstream in2("{ if (a + b) {} else {} }");
    std::istringstream in3("{ if (a - 10) { let b = 11; } else { let b = 12; } }");
    std::istringstream in4("{ loop (a - b) {} }");
    std::istringstream in5("{ loop (a - b) { let b = 100; } }");

    std::istringstream in6("{ let a = 123 + 444;"
                            "let b = (-10) / 1; }");
    std::istringstream in7("{ let a = 123 + 444;"
                            "if (a - 10) { let b = 11; } else { let b = 12; }"
                            "if (a + b) {} else {}"
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

    unit = parse(tokenize(in3));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in4));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in5));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in6));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in7));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";
}
