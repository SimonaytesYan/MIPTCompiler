#include "PrintTokens.hpp"

int main() {
    runOneTest("(", "open bracket");
    runOneTest(")", "close bracket");
    runOneTest(";", "end statement");
    runOneTest("()", "brackets");
    runOneTest("{", "start scope");
    runOneTest("}", "end scope");
    runOneTest("{}", "simple scope");
    runOneTest("func(){};", "expression with spec symbols");
}
