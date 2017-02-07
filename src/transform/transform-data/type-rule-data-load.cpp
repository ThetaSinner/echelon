#include <echelon/transform/transform-data/type-rule-data-load.hpp>

void loadInternal();

void loadTypeRuleData() {
  static bool isLoaded = false;

  if (!isLoaded) {
    loadInternal();
  }

  isLoaded = true;
}

void loadInternal() {
  // TODO almost always want symmetric rules, only for custom type rules might we care about asymmetric products etc.

  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Subtract, "integer", "integer", "integer");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "integer", "integer", "integer");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Multiply, "integer", "integer", "integer");

  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "decimal", "integer", "decimal");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "integer", "decimal", "decimal");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Multiply, "decimal", "integer", "decimal");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Multiply, "integer", "decimal", "decimal");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Multiply, "decimal", "decimal", "decimal");
}