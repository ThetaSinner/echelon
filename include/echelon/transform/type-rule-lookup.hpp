#ifndef TYPE_RULE_LOOKUP_HPP_INCLUDED
#define TYPE_RULE_LOOKUP_HPP_INCLUDED

#include <list>
#include <string>
#include <sstream>
#include <stdexcept>

#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>

class TypeRuleLookup {
  static TypeRuleLookup* instance;

  std::list<std::pair<std::string, std::string>> typeRuleList;

  template <typename E>
  constexpr typename std::underlying_type<E>::type toUnderlying(E e);

  std::string makeKey(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType);
public:
  static TypeRuleLookup* getInstance();
  
  void addRule(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType, std::string resultType);
  bool hasRule(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType);
  std::string lookup(EnhancedAstNodeSubType subType, std::string leftType, std::string rightType);
};

#endif
