#include "RunOneTest.hpp"

int main() {
    ExecutionPass pass;

    runOneTest("{ print(123); }", pass, "Print number");
    runOneTest("{ print(-123); }", pass, "Print negative number");
}
