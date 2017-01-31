#include <echelon/transform/type-rule-lookup.hpp>

TypeRuleLookup* TypeRuleLookup::instance = nullptr;

TypeRuleLookup* TypeRuleLookup::getInstance() {
  if (instance == nullptr) {
    instance = new TypeRuleLookup();
  }

  return instance;
}

template <typename E>
constexpr typename std::underlying_type<E>::type TypeRuleLookup::toUnderlying(E e) {
  return static_cast<typename std::underlying_type<E>::type>(e);
}

std::string TypeRuleLookup::makeKey(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType) {
  std::stringstream ss;
  ss << toUnderlying(subType) << ", " << leftType << ", " << rightType;
  return ss.str();
}

void TypeRuleLookup::addRule(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType, std::string resultType) {
  auto pair = std::make_pair(makeKey(subType, leftType, rightType), resultType);
  typeRuleList.push_back(pair);
}

bool TypeRuleLookup::hasRule(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType) {
  std::string key = makeKey(subType, leftType, rightType);
  for (auto& i : typeRuleList) {
    if (i.first == key) {
      return true;
    }
  }

  return false;
}

std::string TypeRuleLookup::lookup(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType) {
  std::string key = makeKey(subType, leftType, rightType);
  for (auto& i : typeRuleList) {
    if (i.first == key) {
      return i.second;
    }
  }

  throw std::runtime_error("No type rule for [" + key + "]");
}