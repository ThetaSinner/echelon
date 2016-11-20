#include <iostream>

std::string toString(bool b) {
  return b ? "true" : "false";
}

int main(int argc, char** args) {
  std::cout << "Clean main.\n";

  return 0;
}
