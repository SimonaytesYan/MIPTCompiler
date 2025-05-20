#include <iostream>

extern "C" void print_int(int value) {
  std::cout << value << "\n";
}

extern "C" void print_float(float value) {
  std::cout << value << "\n";
}

extern "C" void print_string(const char* value) {
  std::cout << value << "\n";
}
