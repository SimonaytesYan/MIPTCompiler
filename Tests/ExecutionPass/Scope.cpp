#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{}", pass);

    runOneTest("{ print(1); }", pass);
    runOneTest("{ let b = 10;"
                 "print(123 - b * 10); }", pass);

    runOneTest("{ let a = 1; }", pass);
    runOneTest("{ let b = 10;"
                 "let a = b + 1; }"
                 "print(a);", pass);

    runOneTest("{ if (10 + 11) { print(1); } else { print(2); } }", pass);
    runOneTest("{ let a = 10;"
                 "let b = 0;"
                 "if (a - 10) { b = 11; } else { b = 12; }"
                 "print(b); }", pass);

    runOneTest("{ loop (0) { print(10); } }", pass);
    runOneTest("{ let b = 10;"
                 "loop (b - 10)"
                 "{ print(b);"
                   "b = b - 1; } }", pass);
}
