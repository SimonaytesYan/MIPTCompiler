#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{}", pass, "Empty brackets");

    runOneTest("{ print(1); }", pass, "Print expression");
    runOneTest("{ let b = 10;"
                 "print(123 - b * 10); }", pass, "Declare and print variable");

    runOneTest("{ let b = 10;"
                 "let a = b + 1;"
                 "print(a);}", pass, "Assign variable");

    runOneTest("{ if (10 + 11) { print(1); } else { print(0); } }", pass, "If");
    runOneTest("{ if (0) { print(1); } else { print(0); } }", pass, "If else branch");
    runOneTest("{ let a = 10;"
                 "let b = 0;"
                 "if (a - 10) { b = 11; } else { b = 12; }"
                 "print(b); }", pass, "If with variable");

    runOneTest("{ loop (0) { print(10); } }", pass, "Loop without execution");
    runOneTest("{ let b = 10;"
                 "loop (b)"
                 "{ print(b);"
                   "b = b - 1; } }", pass, "Loop with counter");
}
