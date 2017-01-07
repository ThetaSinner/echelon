#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-type-enum.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node-sub-type-enum.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/stage2/nested-pattern-lookup.hpp>

#include <echelon/util/enum-class-iterator.hpp>

void IntegrityCheck::StartupCheck() {
#ifdef ECHELON_DEBUG
  for (auto tte : Enum < TokenType > ()) {
    EchelonLookup::toString(tte);
  }

  for (auto k : Enum < Keyword > ()) {
    EchelonLookup::toString(k);
  }

  for (auto ant : Enum < AstNodeType > ()) {
    EchelonLookup::toString(ant);
  }

  for (auto eant : Enum < EnhancedAstNodeType > ()) {
    EchelonLookup::toString(eant);
  }

  for (auto eanst : Enum < EnhancedAstNodeSubType > ()) {
    EchelonLookup::toString(eanst);
  }
#endif
}

void postLoadCheckInternal(TokenPattern* tokenPattern, std::set<std::string>& alreadyChecked);

void IntegrityCheck::PostLoadCheck() {
#ifdef ECHELON_DEBUG
  std::set<std::string> alreadyChecked;

  // Check that an AST transform is defined for all pattern identifiers.
  for (auto i : *TokenPatternLookup::getInstance()->getTokenPatterns()) {
    postLoadCheckInternal(i, alreadyChecked);
  }
#endif
}

void postLoadCheckInternal(TokenPattern* tokenPattern, std::set<std::string>& alreadyChecked) {
  AstTransformLookup::getInstance()->getAstTransform(tokenPattern->getId());

  // Unwind each pattern and check that any elements which will be matched with nested patterns has an AST transformer.
  for (auto k : *tokenPattern->getGroups()) {
    for (auto t : *k->getElements()) {
      if (alreadyChecked.find(t->getData()) == alreadyChecked.end()) {
        alreadyChecked.insert(t->getData());

        if (NestedPatternLookup::getInstance()->isNest(t->getData())) {
          for (auto m : *NestedPatternLookup::getInstance()->getNested(t->getData())) {
            postLoadCheckInternal(m, alreadyChecked);
          }
        }
      }
    }
  }
}
