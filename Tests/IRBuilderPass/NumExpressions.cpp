#include "RunOneTest.hpp"

int main() {
    runOneTest("{ print(22 + 20); }", "Print add");
    runOneTest("{ print(142 - 99); }", "Print sub");
    runOneTest("{ print(11 * 4); }", "Print mul");
    runOneTest("{ print(450 / 10); }", "Print div");
    runOneTest("{ print((21 + (20 / 10) * 11) - 1); }", "Print complex expr");
}
