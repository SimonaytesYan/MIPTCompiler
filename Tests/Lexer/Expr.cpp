#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("LexerExprDump");

    runOneTest("{ let a = 123 + 444; }", pass);
    runOneTest("{ let a = 123 - 3434; }", pass);
    runOneTest("{ let a = 23 * 10; }", pass);
    runOneTest("{ let a = 23 / 10; }", pass);
    runOneTest("{ let a = a + b; }", pass);
    runOneTest("{ let a = (a + b) * c; }", pass);
    runOneTest("{ let a = a + b + c; }", pass);
    runOneTest("{ let a = (a + b) * c / 10 - 11 + ((-10)); }", pass);
}
