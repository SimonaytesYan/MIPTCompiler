#include "PrintTokens.hpp"

int main() {
    runOneTest("42", "positive int");
    runOneTest("-42", "negative int");
    runOneTest("4a", "start with num, but not num");
    runOneTest("hui", "name");
}
