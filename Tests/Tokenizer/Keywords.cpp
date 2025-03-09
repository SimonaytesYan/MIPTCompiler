#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"

int main() {
    std::istringstream in1("abcd");
    std::istringstream in2("a_bcd");
    std::istringstream in3("Abcd");
    std::istringstream in4("_AbdF");
    std::istringstream in5("_A4");
    std::istringstream in6("4ad");

    auto res1 = tokenize(in1);
    std::cout << "\n";
    auto res2 = tokenize(in2);
    std::cout << "\n";
    auto res3 = tokenize(in3);
    std::cout << "\n";
    auto res4 = tokenize(in4);
    std::cout << "\n";
    auto res5 = tokenize(in5);
    std::cout << "\n";
    auto res6 = tokenize(in6);
}
