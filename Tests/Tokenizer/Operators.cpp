#include "PrintTokens.hpp"

int main() {
    runOneTest("+", "plus");
    runOneTest("-", "minus");
    runOneTest("*", "mul");
    runOneTest("/", "div");
    runOneTest("+-", "plus minus");
    runOneTest("=", "equal");
    runOneTest("/4", "div by 4");
    runOneTest("4/", "4 div by");
    runOneTest("10=11", "10 equal 11");
}
