#include "PrintTokens.hpp"

int main() {
    runOneTest("let", "let");
    runOneTest("if", "if");
    runOneTest("else", "else");
    runOneTest("out", "out");
    runOneTest("hui", "some name");
    runOneTest("else44aa", "name with start of keyword");
}
