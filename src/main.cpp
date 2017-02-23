#include <echelon/transform/type-rule-lookup.hpp>
#include <echelon/model/scope/scope.hpp>
#include <echelon/util/event/event-container.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>
#include <echelon/util/ast-to-graphviz.hpp>
#include <echelon/runner/spidermonkey-runner.hpp>
#include <echelon/model/context/context.hpp>
#include <echelon/model/internal/enhanced-ast-variable-node.hpp>
#include <echelon/model/internal/enhanced-ast-module-node.hpp>
#include <echelon/model/internal/enhanced-ast-custom-type-node.hpp>
#include <echelon/model/internal/enhanced-ast-function-node.hpp>

void gv_out(EnhancedAstNode* e) {
  std::ofstream f("main-ast-out.gv", std::ios::out);
  f << toGraphviz(e);
  f.close();
  // dot main-ast-out.gv -Tjpg -omain-ast-out.jpg && main-ast-out.jpg
}

int main(int argc, char **args) {
  Logger *log = LoggerSharedInstance::get();

#ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
#else
  log->at(Level::Info) << "This is a release build.\n";
#endif

  EchelonCompiler compiler;

  // TODO can actually detect this error and report it. "Did you forget the function keyword?"
  // auto out = compiler.parse("type MyType {\n  toString(integer t) -> string\n}");

  try {
    RunnerData runnerData;
    runnerData.setScript("function tf() {return 'test string'} tf()");
    runner(runnerData);

    // TODO Need private variables to try to implement anything here.. so it's context time.
    //auto out = compiler.enhance("behaviour ToString {\n  function toString() -> string\n}\n\ntype BigInteger {\n}");

    // TODO fixed, should fail to find oops variable, not accessible. Extract test.
    // auto out = compiler.enhance("module ModuleOne {\ndecimal oops = 0.5\n\ntype MyType {\n  integer my_type_var\n  function get_my_type_var() -> integer\n}\n}\n\nmodule ModuleTwo {\nfunction ModuleOne::MyType::get_my_type_var() {\n  oops + my_type_var\n}\n}");

    // TODO I need the linker (partial) for this part, because I am generating a constructor and linking it to MyType
    /*auto out = compiler.enhance("type MyType {function get_val() -> integer {5}} myinstance = MyType.create() myinstance.get_val()");
    gv_out(out);
    log->at(Level::Info) << to_string(out) << "\n";*/

    auto out = compiler.enhance("function callMe(integer r) {r + 5} x = callMe(1)");
    gv_out(out->getEnhancedAstNode());
    log->at(Level::Info) << to_string(out->getEnhancedAstNode()) << "\n";
    if (out->getTransformWorkingData()->getEventContainer().hasListeners()) {
      // At some point this will be allowed, provided the resulting program is going to the linker.
      // This check can then go in the compiler? check if the programs integrity is okay.
      throw std::runtime_error("There are unresolved references, program is invalid.");
    }

    // TODO, once the compiler figures out what code calls what functions the name can be mangled
    // so that overloading is invisible to the code generator.
  }
  catch (const std::runtime_error &e) {
    LoggerSharedInstance::get()->at(Level::Fatal) << "dev run failed [" << e.what() << "]";
    return 1;
  }
  catch (...) {
    LoggerSharedInstance::get()->at(Level::Fatal) << "dev run failed. Exception handling in the program isn't catching this error.";
    return 2;
  }

  log->at(Level::Info) << "\nProgram will exit normally.\n";
  return 0;
}
