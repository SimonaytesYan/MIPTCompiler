#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{ print(123 + 444); }", pass, "Print add");
    runOneTest("{ print(123 - 3434); }", pass, "Print sub");
    runOneTest("{ print(23 * 10); }", pass, "Print mul");
    runOneTest("{ print(23 / 10); }", pass, "Print div");
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
}
