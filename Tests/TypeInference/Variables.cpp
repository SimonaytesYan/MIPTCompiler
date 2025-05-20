#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("TypeVariableDump");

    runOneTest("{let a = 10;"
                "let b = a;}", pass);
    runOneTest("{let a = {10, 11, 12, 13.0};", pass);
    runOneTest("{let a = {10, 11, 12, 13};"
               " a = {10, 11, 12};", pass);
    runOneTest("{let a = {10, 11, 12, 13};"
               " a = {10.0, 11.0, 12.0, 13.0};", pass);
    runOneTest("{let a = 10.1; "
                "let b = 11;}", pass);
    runOneTest("{let a = \"hui\";"
                "a = \"pizda\"}", pass);
    runOneTest("{let a = 10 + 11;" 
                 "a = 11.1}", pass);
    runOneTest("{let a = (10 + 10.0); }", pass);
    runOneTest("{let a = (10 + 10.0);"
                 "a = 10; }", pass);
}