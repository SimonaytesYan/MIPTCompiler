#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"

int main() {
    std::istringstream in1("let");
    std::istringstream in2("if");
    std::istringstream in3("else");
    std::istringstream in4("out");
    std::istringstream in5("hui");
    std::istringstream in6("else44aa");

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
