#include "RunOneTest.hpp"

int main() {
    runOneTest("{"
                "let b = 10;"
                "loop (b) {"
                "    print(b);"
                "    b = b - 1;"
                "}}", "Basic loop");
}
