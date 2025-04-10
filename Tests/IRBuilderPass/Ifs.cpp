#include "RunOneTest.hpp"

int main() {
    IRBuilderPass pass;

    runOneTest("{ if (1) { print(1); } else { print(0); } }", pass, "If with constant condition");
    runOneTest("{ let a = 10;"
                 "if (a - 10) { print(0); } else { print(1); } }", pass, "If with var in condition");
    runOneTest("{ let a = 10;"
                 "print(a);"
                 "a = 11;"
                 "print(a); }", pass, "Change variable");
    runOneTest("{ if (1) {"
                 "let b = 1;"
                 "print(b)"
                 "}"
                 "else {"
                 "print(0);"
                 "}}", pass, "Create var in if");
    runOneTest("{ let b = 1;"
                "if (1) {"
                "let b = 42;"
                "print(b)"
                "}"
                "else {"
                "print(0);"
                "}}", pass, "Recreate var in if");
}
