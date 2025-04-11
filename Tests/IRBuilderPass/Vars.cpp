#include "RunOneTest.hpp"

int main() {
    runOneTest("{ let a = 10;"
                 "print(a); }", "Variable");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "print(a + b); }", "Variable add");
    runOneTest("{ let a = 10;"
                 "print(a);"
                 "a = 11;"
                 "print(a); }", "Change variable");
    runOneTest("{ let a = 6;"
                 "let b = 11;"
                 "b = a * 7;"
                 "print(b); }", "Two variables");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c / 10 - 11 + ((-10))); }", "Complex expression with var");
}
