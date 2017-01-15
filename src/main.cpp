#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/ast/transform-stage/scope.hpp>
#include <echelon/util/event/event-container.hpp>

EnhancedAstNodeSubType nextOperator(EnhancedAstNodeSubType astNodeType) {
  switch (astNodeType) {
    case EnhancedAstNodeSubType::First:
      return EnhancedAstNodeSubType::Subtract;
    case EnhancedAstNodeSubType::Subtract:
      return EnhancedAstNodeSubType::Add;
    default:
      return EnhancedAstNodeSubType::Last;
  }
}

EnhancedAstNode* rotate(EnhancedAstNode *node, EnhancedAstNodeSubType subType = EnhancedAstNodeSubType::First) {
  auto operatorSubType = subType;
  while ((operatorSubType = nextOperator(operatorSubType)) != EnhancedAstNodeSubType::Last) {

    // Find the first operator of this type.
    EnhancedAstNode *parent = nullptr;
    auto oper = node;
    while (oper->getNodeSubType() != operatorSubType && oper->getChildCount() > 0) {
      parent = oper;
      oper = oper->getChild(1);
    }

    if (oper->getNodeType() != EnhancedAstNodeType::BinaryOperator) {
      // We've hit a value, there are no more operators of teh current type, skip to the next type.
      continue;
    }

    if (parent != nullptr) {
      // rotate around this first operator.

      // extract the operator node from its parent.
      parent->removeChild(oper);

      // Move the value from this operator node to the one above. i.e. associate left to the higher precedence operator.
      auto value = oper->getChild(0);
      oper->removeChild(value);
      parent->putChild(value);

      // rotate the left part.
      auto leftPart = rotate(node, subType);
      oper->putChild(leftPart);

      // rotate the right part.
      auto rightPart = oper->getChild(1);
      oper->removeChild(rightPart);
      oper->putChild(rotate(rightPart, subType));

      return oper;
    }
  }

  return node;
}

int main(int argc, char **args) {
  Logger *log = LoggerSharedInstance::get();

#ifdef ECHELON_DEBUG
  log->at(Level::Info) << "This is a debug build.\n";
#else
  log->at(Level::Info) << "This is a release build.\n";
#endif

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

    auto expr = compiler.enhance("x + y - z");
    auto e = rotate(expr->getChild(0)); // skip the program node.
    log->at(Level::Info) << to_string(e);

    // TODO this is working as it should (apart from function return type, that's another problem) so create a test.
    //auto out = compiler.enhance("package PackageName\ntype MyType {\n  integer my_x = 5\n  integer my_y=4  public function get_product() -> integer}\n\nfunction MyType::get_product() -> integer {\n  my_x * my_y}");
    //log->at(Level::Info) << to_string(out) << "\n";
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
