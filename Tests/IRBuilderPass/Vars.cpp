#include "RunOneTest.hpp"

int main() {
    IRBuilderPass pass;

    runOneTest("{ let a = 10;"
                 "print(a); }", pass, "Variable");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "print(a + b); }", pass, "Variable add");
    runOneTest("{ let a = 10;"
                 "print(a);"
                 "a = 11;"
                 "print(a); }", pass, "Change variable");
    runOneTest("{ let a = 6;"
                 "let b = 11;"
                 "b = a * 7;"
                 "print(b); }", pass, "Two variables");
    runOneTest("{ let a = 10;"
                 "let b = 11;"
                 "let c = 5;"
                 "print((a + b) * c / 10 - 11 + ((-10))); }", pass, "Complex expression with var");
}
