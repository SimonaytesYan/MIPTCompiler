#include "PrintTokens.hpp"

int main() {
    runOneTest("abcd", "little english letters");
    runOneTest("a_bcd", "with underscore");
    runOneTest("Abcd", "different cases");
    runOneTest("_AbdF", "different cases with underscore");
    runOneTest("_A4", "with num");
    runOneTest("4ad", "start with num");
}
