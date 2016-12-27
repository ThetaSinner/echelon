#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/ast-node-type-enum.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/parser/echelon-lookup.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/stage2/nested-pattern-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <echelon/util/enum-class-iterator.hpp>
#endif

void IntegrityCheck::StartupCheck() {
  #ifdef ECHELON_DEBUG
  for (auto tte : Enum<TokenType>()) {
    EchelonLookup::toString(tte);
  }

  for (auto k : Enum<Keyword>()) {
    EchelonLookup::toString(k);
  }

  for (auto ant : Enum<AstNodeType>()) {
    EchelonLookup::toString(ant);
  }
  #endif
}

void IntegrityCheck::PostLoadCheck() {
  #ifdef ECHELON_DEBUG
  // Check that an AST transform is defined for all pattern identifiers.
  for (auto i : *TokenPatternLookup::getInstance() -> getTokenPatterns()) {
    AstTransformLookup::getInstance() -> getAstTransform(i -> getId());

    // Unwind each pattern and check that any elements which will be matched with nested patterns has an AST transformer.
    for (auto k : *i -> getGroups()) {
      for (auto t : *k -> getElements()) {
        if (NestedPatternLookup::getInstance() -> isNest(t -> getData())) {
          for (auto m : *NestedPatternLookup::getInstance() -> getNested(t -> getData())) {
            AstTransformLookup::getInstance() -> getAstTransform(m -> getId());
          }
        }
      }
    }
  }
  #endif
}
