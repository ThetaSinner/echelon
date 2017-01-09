#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

int main(int argc, char **args) {
  Logger *log = LoggerSharedInstance::get();

#ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
#else
  log->at(Level::Info) << "This is a release build.\n";
#endif

  LoggerSharedInstance::get()->setLevel(levelToInt(Level::Debug));

  EchelonCompiler compiler;

  // TODO allow comments inside patterns. e.g. multi line comment inside function call which has been broken over lines.

  try {
    // TODO Need private variables to try to implement anything here.. so it's context time.
    //auto out = compiler.enhance("behaviour ToString {\n  function toString() -> string\n}\n\ntype BigInteger {\n}");

    // TODO can actually detect this error and report it. "Did you forget the function keyword?"
    auto out = compiler.parse("type MyType {\n  public function toString(integer t) -> string\n}");
    log->at(Level::Info) << to_string(out) << "\n";
  }
  catch (const std::runtime_error &e) {
    LoggerSharedInstance::get()->at(Level::Fatal) << "dev compile failed [" << e.what() << "]";
    return 1;
  }
  catch (...) {
    LoggerSharedInstance::get()->at(Level::Fatal)
        << "dev compile failed. Exception handling in the program isn't catching this error.";
    return 2;
  }

  log->at(Level::Info) << "\nProgram will exit normally.\n";
  return 0;
}
