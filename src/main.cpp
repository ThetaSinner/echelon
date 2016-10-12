#include <iostream>
#include <elm/parser/tokenizer.hpp>

int main(int argc, char** args) {
  std::cout << "Happy dappy" << std::endl;

  Tokenizer t;
  t.tokenize("hello world");
  t.tokenize("hello_world");
  t.tokenize("12 3he4llo5 67 8wo9rld10 11");

  return 0;
}
