#include "PrintTokens.hpp"

int main() {
    runOneTest("let int a = [11, 12, 13];", "arr with brackets");
    runOneTest("let s = \"hello world!\";", " double quotes");
    runOneTest("let s = \'hello world!\';", "single quotes");
    runOneTest("let f1 = 10.1;", "float1");
    runOneTest("let f2 = 0.1;", "float2");
    runOneTest("let f3 = -1001.112;", "minut float");
}
