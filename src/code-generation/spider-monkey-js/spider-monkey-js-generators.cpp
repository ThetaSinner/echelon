#include "spider-monkey-js-generators.hpp"

#ifdef ECHELON_DEBUG

#include <echelon/util/to-string.hpp>

#endif

void loadGenerators() {
  static bool loaded = false;
  if (!loaded) {
    Generator *programGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      for (unsigned i = 0; i < astNode->getChildCount(); i++) {
        ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(i)->getType())->generate(
            astNode->getChild(i));
      }
      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Program, programGenerator);

    Generator *variableGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << "var ";
      ss << astNode->getData();

      if (astNode->getChildCount() == 1 && astNode->getChild(0)->getType() != AstNodeType::TypeName) {
        ss << " = ";
        ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
            astNode->getChild(0));
      } else if (astNode->getChildCount() > 1) {
        ss << " = ";
        ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(1)->getType())->generate(
            astNode->getChild(1));
      }

      ss << ";";

      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Variable, variableGenerator);

    Generator *expressionGenerator = new Generator([](AstNode *astNode) -> std::string {
      return GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(astNode->getChild(0));
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Expression, expressionGenerator);

    Generator *stringGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << "\"";
      ss << astNode->getData() << "\"";
      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::String, stringGenerator);

    Generator *booleanTrueGenerator = new Generator([](AstNode *astNode) -> std::string {
      return "true";
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::BooleanTrue, booleanTrueGenerator);

    Generator *booleanFalseGenerator = new Generator([](AstNode *astNode) -> std::string {
      return "false";
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::BooleanFalse, booleanFalseGenerator);

    Generator *blockGenerator = new Generator([](AstNode *astNode) -> std::string {
      return GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
          astNode->getChild(0));
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Block, blockGenerator);


    Generator *typeGenerator = new Generator([](AstNode *astNode) -> std::string {
      return "";
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::TypeName, typeGenerator);

    Generator *branchesGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      for (unsigned i = 0; i < astNode->getChildCount(); i++) {
        ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(i)->getType())->generate(
            astNode->getChild(i));
      }
      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Branches, branchesGenerator);

    Generator *ifGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << "if (";
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
          astNode->getChild(0));
      ss << ") {";
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(1)->getType())->generate(
          astNode->getChild(1));
      ss << "}";
      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::If, ifGenerator);

    Generator *elseGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << "else ";
      ss << "{";
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
          astNode->getChild(0));
      ss << "}";
      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::Else, elseGenerator);

    Generator *logicalConjunctionGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
          astNode->getChild(0));
      ss << " && ";
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(1)->getType())->generate(
          astNode->getChild(1));

      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::LogicalConjunction, logicalConjunctionGenerator);

    Generator *logicalDisjunctionGenerator = new Generator([](AstNode *astNode) -> std::string {
      std::stringstream ss;
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(0)->getType())->generate(
          astNode->getChild(0));
      ss << " || ";
      ss << GeneratorLookup::getInstance()->getGenerator(astNode->getChild(1)->getType())->generate(
          astNode->getChild(1));

      return ss.str();
    });

    GeneratorLookup::getInstance()->addGenerator(AstNodeType::LogicalDisjunction, logicalDisjunctionGenerator);
  }

  loaded = true;
}
