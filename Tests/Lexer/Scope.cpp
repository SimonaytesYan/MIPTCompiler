#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("LexerScopeDump");

    runOneTest("{}", pass);
    runOneTest("{ if (a + b) {} else {} }", pass);
    runOneTest("{ if (a - 10) { let b = 11; } else { let b = 12; } }", pass);
    runOneTest("{ loop (a - b) {} }", pass);
    runOneTest("{ loop (a - b) { let b = 100; } }", pass);

    runOneTest("{ let a = 123 + 444;"
                 "let b = (-10) / 1; }", pass);
    runOneTest("{ let a = 123 + 444;"
                 "if (a - 10) { let b = 11; } else { let b = 12; }"
                 "if (a + b) {} else {}"
                "}", pass);
}
