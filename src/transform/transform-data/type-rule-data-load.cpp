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
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Subtract, "integer", "integer", "integer");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "integer", "integer", "integer");

  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "decimal", "integer", "decimal");
  TypeRuleLookup::getInstance()->addRule(EnhancedAstNodeSubType::Add, "integer", "decimal", "decimal");
}