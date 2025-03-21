#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{ print(123); }", pass);
    runOneTest("{ print(-123); }", pass);
    runOneTest("{ print(10 - 4 * 100 / 12); }", pass);
}
