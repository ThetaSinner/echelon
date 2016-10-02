#include <iostream>
#include "elm/parser/parser.hpp"

int main(int argc, char** args) {
  std::cout << "Happy dappy" << std::endl;

  Parser p;
  p.parse("hello world");
  p.parse("hello_world");
  p.parse("12 3he4llo5 67 8wo9rld10 11");

  return 0;
}
