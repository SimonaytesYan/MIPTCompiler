#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"

int main() {
    std::istringstream in1("(");
    std::istringstream in2(")");
    std::istringstream in3("()");
    std::istringstream in4("{");
    std::istringstream in5("}");
    std::istringstream in6("{}");
    std::istringstream in7("func(){}");

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
    std::cout << "\n";
    auto res7 = tokenize(in7);
}
