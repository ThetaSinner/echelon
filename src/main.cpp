#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/stream-dump.hpp>
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

  // TODO a function in a behaviour block without the function keyword will cause an infinite loop. This is a class of problems with nested processing.

  try {
    auto ast = compiler.parse("2 * -(2 * -5)");
    stream_dump(Level::Info, ast);
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
