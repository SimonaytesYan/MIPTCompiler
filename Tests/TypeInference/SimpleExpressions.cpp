#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("TypeSimpleDump");

    runOneTest("{let a = 10; }", pass);
    runOneTest("{let b = 10.1; }", pass);
    runOneTest("{let c = \"hui\"; }", pass);
    runOneTest("{let d = 10 + 11; }", pass);
    runOneTest("{let e = (10 * 11) / 13 + 11; }", pass);
    runOneTest("{let f = (10 * 11) / 13.0 + 11; }", pass);
    runOneTest("{let g = [10, 11, 12, 13]; }", pass);
}