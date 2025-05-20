#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("TypeStatementDump");
    runOneTest("{ if (1) {"
                    "let b = 1;"
                    "print(b);"
                 "}"
                 "else {"
                    "print(0);"
               "}}", pass);

    runOneTest("{ let b = 1;"
                "if (1) {"
                "let b = 42;"
                "print(b);"
                "}"
                "else {"
                "print(0);"
                "}}", pass);
}