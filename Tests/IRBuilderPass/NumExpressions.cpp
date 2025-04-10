#include "RunOneTest.hpp"

int main() {
    IRBuilderPass pass;

    runOneTest("{ print(22 + 20); }", pass, "Print add");
    runOneTest("{ print(142 - 100); }", pass, "Print sub");
    runOneTest("{ print(6 * 7); }", pass, "Print mul");
    runOneTest("{ print(420 / 10); }", pass, "Print div");
    runOneTest("{ print((21 + (20 / 10) * 11) - 1); }", pass, "Print complex expr");

    #if 0
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "print(a + b); }", pass, "Print variable add");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c); }", pass, "Print variable add and mul");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print(a + b + c); }", pass, "Print variable sum of three");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c / 10 - 11 + ((-10))); }", pass, "Print complex expression");
    #endif
}
