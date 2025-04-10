#include <iostream>

extern "C" void print(int value) {
  std::cout << value << "\n";
}
