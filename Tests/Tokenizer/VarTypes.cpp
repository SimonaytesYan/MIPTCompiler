#include "PrintTokens.hpp"

int main() {
    runOneTest("let int a;", "int");
    runOneTest("let flt b;", "flt");
    runOneTest("let str c;", "str");
    runOneTest("let arr flt d;", "arr");
    runOneTest("let arr int d = [11, 12, 13];", "arr with brackets");
    runOneTest("let str s = \"hello world!\";", " double quotes");
    runOneTest("let str s = \'hello world!\';", "single quotes");
}
