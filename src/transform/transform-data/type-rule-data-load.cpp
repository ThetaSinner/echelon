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
  TypeRuleLookup::addRule(EnhancedAstNodeSubType::Subtract, "integer", "integer", "integer");
  TypeRuleLookup::addRule(EnhancedAstNodeSubType::Add, "integer", "integer", "integer");
}