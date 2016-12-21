#include <iostream>

#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/util/stream-dump.hpp>

#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/compiler/integrity-check.hpp>

int main(int argc, char** args) {
  #ifdef ECHELON_DEBUG
  std::cout << "This is a debug build.\n";
  #else
  std::cout << "This is a release build.\n";
  #endif

  loadParserStage1Data();
  loadParserStage2Data();

  IntegrityCheck::StartupCheck();

  std::cout << "Parser data loaded." << std::endl;

  // Need a good data structure which has all the declared data, types and functions.
  // It needs to be such that it is simple to find out whether something is defined in the current context or parent context
  // for example. Maybe it will be a complementary structure to the existing tree.

  EchelonCompiler compiler;
  std::cout << "Compiler output: " << compiler.compile("string x\nif (true || false) {\n  x = \"bildo\"\n}\nelse {\n  x = \"winki\"\n}\n") << std::endl;

  std::cout << std::endl << "Program will exit normally.";
  std::cout << std::endl;
  return 0;
}
