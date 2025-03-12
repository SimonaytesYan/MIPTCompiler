#include "RunOneTest.hpp"

int main() {
    GraphicDumpPass pass("LexerObjDump");

    runOneTest("{let a = 123}", pass);
    runOneTest("{let b = -123}", pass);
    runOneTest("{let c = Abcd; }", pass);
}
