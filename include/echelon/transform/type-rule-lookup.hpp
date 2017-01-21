#ifndef TYPE_RULE_LOOKUP_HPP_INCLUDED
#define TYPE_RULE_LOOKUP_HPP_INCLUDED

#include <map>
#include <tuple>
#include <string>
#include <sstream>
#include <stdexcept>

#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

class TypeRuleLookup {
  std::map<std::string, std::string> typeRuleMap;

  template <typename E>
  static constexpr typename std::underlying_type<E>::type toUnderlying(E e) {
    return static_cast<typename std::underlying_type<E>::type>(e);
  }

  static std::string makeKey(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType) {
    std::stringstream ss;
    ss << toUnderlying(subType) << ", " << leftType << ", " << rightType;
    return ss.str();
  }
public:
  static void addRule(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType, std::string resultType) {
    typeRuleMap.insert(makeKey(subType, leftType, rightType), resultType);
  }

  static std::string lookup(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType) {
    std::string key = makeKey(subType, leftType, rightType);
    if (typeRuleMap.find(key) != typeRuleMap.end()) {
      return typeRuleMap.at(key);
    }
    else {
      throw std::runtime_error("No type rule for [" + key + "]");
    }
  }
};

#endif
