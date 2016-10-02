#include <iostream>
#include "elm/parser/parser.hpp"

int main(int argc, char** args) {
  std::cout << "Happy dappy" << std::endl;

  Parser p;
  p.parse("hello world");

  return 0;
}
