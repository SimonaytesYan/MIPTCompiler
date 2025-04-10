#include "RunOneTest.hpp"

int main() {
    runOneTest("{ if (1) { print(1); } else { print(0); } }", "If with constant condition");
    runOneTest("{ let a = 10;"
                 "if (a - 10) { print(0); } else { print(1); } }", "If with var in condition");
    runOneTest("{ let a = 10;"
                 "print(a);"
                 "a = 11;"
                 "print(a); }", "Change variable");
    runOneTest("{ if (1) {"
                 "let b = 1;"
                 "print(b)"
                 "}"
                 "else {"
                 "print(0);"
                 "}}", "Create var in if");
    runOneTest("{ let b = 1;"
                "if (1) {"
                "let b = 42;"
                "print(b)"
                "}"
                "else {"
                "print(0);"
                "}}", "Recreate var in if");
}
