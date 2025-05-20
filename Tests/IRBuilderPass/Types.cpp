#include "RunOneTest.hpp"

int main() {
    runOneTest("{ print(10.0); }", "Print float");

    runOneTest("{ let a = 10.0;"
                 "print(a); }", "Float variables");
    runOneTest("{ let a = 10.0;"
                 "let b = a;"
                 "print(a + b); }", "Float variables");
    runOneTest("{ let a = 10.0;"
                 "print(a);"
                 "a = 11.0;"
                 "print(a); }", "Float change variable");

    runOneTest("{ print('hello world'); }", "hello world");
    runOneTest("{ let a = 'hello world';"
                 "print(a); }", "String variables");
    runOneTest("{ let a = 10.0;"
                 "let b = a;"
                 "print(b); }", "String variables");
    runOneTest("{ let a = 'bey bey world';"
                 "print(a);"
                 "a = 'hello world';"
                 "print(a); }", "String change variables");
}
