#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("LexerObjDump");

    runOneTest("{let a = 123; }", pass);
    runOneTest("{let b = -123; }", pass);
    runOneTest("{let c = Abcd; }", pass);
    runOneTest("{let d = -0.1; }", pass);
    runOneTest("{let e = []; }", pass);
    runOneTest("{let f = [10]; }", pass);
    runOneTest("{let g = [10, 12, 13, 11.7]; }", pass);
}
