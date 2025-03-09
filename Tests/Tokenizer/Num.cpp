#include <iostream>
#include <sstream>

#include "../../Src/Headers/Tokenizer.hpp"

int main() {
    std::istringstream in1("42");
    std::istringstream in2("-42");
    std::istringstream in3("4a");
    std::istringstream in4("hui");

    auto res1 = tokenize(in1);
    std::cout << "\n";
    auto res2 = tokenize(in2);
    std::cout << "\n";
    auto res3 = tokenize(in3);
    std::cout << "\n";
    auto res4 = tokenize(in4);
}
