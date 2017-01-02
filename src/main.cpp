#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>

#include <echelon/util/stream-dump.hpp>

#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/transform-stage/node-enhancer-lookup.hpp>
#include <echelon/transform/ast-enhancer.hpp>
#include <echelon/ast/transform-stage/scope.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/logging/logger.hpp>
#include <echelon/transform/transform-data/ast-enhancer-data.hpp>

int main(int argc, char** args) {
  Logger* log = LoggerSharedInstance::get();

  #ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
  #else
  log->at(Level::Info) << "This is a release build.\n";
  #endif

  EchelonCompiler compiler;


  log->at(Level::Info) << "\nProgram will exit normally.\n";
  return 0;
}
