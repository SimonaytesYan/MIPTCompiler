#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{ print(123 + 444); }", pass);
    runOneTest("{ print(123 - 3434); }", pass);
    runOneTest("{ print(23 * 10); }", pass);
    runOneTest("{ print(23 / 10); }", pass);
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "print(a + b); }", pass);
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c); }", pass);
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print(a + b + c); }", pass);
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c / 10 - 11 + ((-10))); }", pass);
}
