#include "spider-monkey-js-generators.hpp"

void loadGenerators() {
  static bool loaded = false;
  if (!loaded) {
    Generator* programGenerator = new Generator([] (AstNode* astNode) -> std::string {
      std::stringstream ss;
      for (int i = 0; i < astNode -> getChildCount(); i++) {
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(i) -> getType()) -> generate(astNode -> getChild(i));
      }
      return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Program, programGenerator);

    Generator* variableGenerator = new Generator([] (AstNode* astNode) -> std::string {
      std::stringstream ss;
      ss << "var ";
      ss << astNode -> getData();

      if (astNode -> getChildCount() > 1) {
        ss << " = ";
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(1) -> getType()) -> generate(astNode -> getChild(1));
      }

      ss << ";";

      return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Variable, variableGenerator);

    Generator* stringGenerator = new Generator([] (AstNode* astNode) -> std::string {
      std::stringstream ss;
      ss << "\"" << astNode -> getData() << "\"";
      return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::String, stringGenerator);

    Generator* typeGenerator = new Generator([] (AstNode* astNode) -> std::string {
      return "";
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Type, typeGenerator);
  }

  loaded = true;
}
