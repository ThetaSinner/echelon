#include "spider-monkey-js-generators.hpp"

#ifdef ECHELON_DEBUG
#include <echelon/util/stream-dump.hpp>
#endif

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

      if (astNode -> getChildCount() == 1 && astNode -> getChild(0) -> getType() != AstNodeType::Type) {
        ss << " = ";
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(0) -> getType()) -> generate(astNode -> getChild(0));
      }
      else if (astNode -> getChildCount() > 1) {
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

    Generator *booleanGenerator = new Generator([] (AstNode* astNode) -> std::string {
        std::stringstream ss;
        ss << astNode -> getData(); // TODO map properly
        return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Boolean, booleanGenerator);

    Generator *blockGenerator = new Generator([] (AstNode* astNode) -> std::string {
        return GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(0) -> getType()) -> generate(astNode -> getChild(0));
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Block, blockGenerator);


    Generator* typeGenerator = new Generator([] (AstNode* astNode) -> std::string {
      return "";
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Type, typeGenerator);

    Generator* branchesGenerator = new Generator([] (AstNode* astNode) -> std::string {
        std::stringstream ss;
        for (int i = 0; i < astNode -> getChildCount(); i++) {
          ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(i) -> getType()) -> generate(astNode -> getChild(i));
        }
        return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Branches, branchesGenerator);

    Generator* ifGenerator = new Generator([] (AstNode* astNode) -> std::string {
        std::stringstream ss;
        ss << "if (";
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(0) -> getType()) -> generate(astNode -> getChild(0));
        ss << ") {";
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(1) -> getType()) -> generate(astNode -> getChild(1));
        ss << "}";
        return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::If, ifGenerator);

    Generator *elseGenerator = new Generator([] (AstNode* astNode) -> std::string {
        std::stringstream ss;
        ss << "else ";
        ss << "{";
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(0) -> getType()) -> generate(astNode -> getChild(0));
        ss << "}";
        return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::Else, elseGenerator);

    Generator *booleanBinaryOperatorGenerator = new Generator([] (AstNode* astNode) -> std::string {
        std::stringstream ss;
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(0) -> getType()) -> generate(astNode -> getChild(0));
        ss << " " << astNode -> getData() << " "; // TODO map properly with operator types.
        ss << GeneratorLookup::getInstance() -> getGenerator(astNode -> getChild(1) -> getType()) -> generate(astNode -> getChild(1));

        return ss.str();
    });

    GeneratorLookup::getInstance() -> addGenerator(AstNodeType::BooleanBinaryOperator, booleanBinaryOperatorGenerator);
  }

  loaded = true;
}
