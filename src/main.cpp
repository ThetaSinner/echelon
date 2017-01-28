#include <echelon/transform/type-rule-lookup.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/util/event/event-container.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/operator-precedence-tree-restructurer.hpp>
#include <echelon/util/ast-to-graphviz.hpp>

int main(int argc, char **args) {
  Logger *log = LoggerSharedInstance::get();

#ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
#else
  log->at(Level::Info) << "This is a release build.\n";
#endif

  TypeRuleLookup::addRule(EnhancedAstNodeSubType::Subtract, "integer", "integer", "integer");
  TypeRuleLookup::addRule(EnhancedAstNodeSubType::Add, "integer", "integer", "integer");

  EventContainer eventContainer;

  eventContainer.addEventListener("event_id", [&eventContainer](EventKey& eventKey, void* data) {
    std::cout << "event handler\n";
    std::cout << ((EnhancedAstNode*) data)->getData() << "\n";
    eventContainer.removeEventListener(eventKey);
  });

  EnhancedAstNode* eventData = new EnhancedAstNode();
  eventData->setData("trick or treat");
  eventContainer.triggerEvent("event_id", eventData);
  eventContainer.triggerEvent("event_id", nullptr);

  LoggerSharedInstance::get()->setLevel(levelToInt(Level::Debug));

  EchelonCompiler compiler;

  // TODO split scope into parent and local so that pointers can be used and declarations can be in any order.

  // TODO allow comments inside patterns. e.g. multi line comment inside function call which has been broken over lines.

  // TODO can actually detect this error and report it. "Did you forget the function keyword?"
  // auto out = compiler.parse("type MyType {\n  toString(integer t) -> string\n}");

  try {
    // TODO Need private variables to try to implement anything here.. so it's context time.
    //auto out = compiler.enhance("behaviour ToString {\n  function toString() -> string\n}\n\ntype BigInteger {\n}");

    // TODO extract test auto expr = compiler.enhance("a * (b + c - d)");
    // TODO extract test auto expr = compiler.enhance("(a - b + c) * (d + e - f)");
    // TODO extract test auto expr = compiler.enhance("(a + b - c)");
    // TODO extract test auto expr = compiler.enhance("(a + b - c) * (d + e - f) * (g + h - i)");
    auto expr = compiler.enhance("(a + b - c) / (d + e - f) * (g + h - i)");

    auto e = OperatorPrecedenceTreeRestructurer::restructure(expr->getChild(0)); // skip the program node.

    std::ofstream f("main-ast-out.gv", std::ios::out);
    f << toGraphviz(e);
    f.close();
    // dot main-ast-out.gv -Tjpg -omain-ast-out.jpg && main-ast-out.jpg

    log->at(Level::Info) << to_string(e);

    // TODO this is working as it should (apart from function return type, that's another problem) so create a test.
    //auto out = compiler.enhance("package PackageName\ntype MyType {\n  integer my_x = 5\n  integer my_y=4  public function get_product() -> integer}\n\nfunction MyType::get_product() -> integer {\n  my_x * my_y}");
    auto out = compiler.enhance("function get_two_plus_two() {\n  2 + 2}");
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
