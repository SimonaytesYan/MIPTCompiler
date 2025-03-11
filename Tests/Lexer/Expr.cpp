#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"
#include "../../Src/Headers/Lexer.hpp"
#include "../../Src/Headers/GraphicDumpPass.hpp"

int main() {
    std::istringstream in1("{ let a = 123 + 444; }");
    std::istringstream in2("{ let a = 123 - 3434; }");
    std::istringstream in3("{ let a = 23 * 10; }");
    std::istringstream in4("{ let a = 23 / 10; }");
    std::istringstream in5("{ let a = a + b; }");
    std::istringstream in6("{ let a = (a + b) * c; }");
    std::istringstream in7("{ let a = a + b + c; }");
    std::istringstream in8("{ let a = (a + b) * c / 10 - 11 + ((-10)); }");

    GraphicDumpPass pass("LexerExprDump");

    GrammarUnit* unit = parse(tokenize(in1));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in2));
    pass.graphicDump(unit);
    std::cout << "\n";
    std::cerr << "\n";

    unit = parse(tokenize(in3));
    pass.graphicDump(unit);
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

    unit = parse(tokenize(in8));
    pass.graphicDump(unit);
    recursiveUnitDelete(unit);
}
