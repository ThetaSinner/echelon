#include <echelon/compiler/integrity-check.hpp>

#include <echelon/model/ast/ast-node-type-enum.hpp>
#include <echelon/model/internal/enhanced-ast-node-type-enum.hpp>
#include <echelon/model/internal/enhanced-ast-node-sub-type-enum.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/stage2/ast-transform-lookup.hpp>
#include <echelon/parser/stage2/token-pattern-lookup.hpp>
#include <echelon/parser/stage2/nested-pattern-lookup.hpp>

#include <echelon/util/enum-class-iterator.hpp>
#include <echelon/parser/stage2/matcher-lookup.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>

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

void postLoadCheckInternal(TokenPattern* tokenPattern, std::set<std::string>& patternElements);
void findUnusedTokenMatchers(const std::set<std::string>& patternElements);

void IntegrityCheck::PostLoadCheck() {
#ifdef ECHELON_DEBUG
  std::set<std::string> patternElements;

  // Check that an AST transform is defined for all pattern identifiers.
  for (auto i : *TokenPatternLookup::getInstance()->getTokenPatterns()) {
    postLoadCheckInternal(i, patternElements);
  }

  // Find unused token matchers.
  findUnusedTokenMatchers(patternElements);

#endif
}

void findUnusedTokenMatchers(const std::set<std::string>& patternElements) {
  static auto log = LoggerSharedInstance::get();

  auto tokenMatchers = MatcherLookup::getInstance()->getMatchers();
  auto tokenMatchersCopy = *tokenMatchers;
  for (auto i : patternElements) {
    auto iter = tokenMatchersCopy.find(i);
    if (iter != tokenMatchersCopy.end()) {
      tokenMatchersCopy.erase(iter);
    }
  }

  for (auto& i : tokenMatchersCopy) {
    log->at(StreamLoggerLevel::Warn) << "Unused token matcher [" << i.first << "]\n";
  }
}

void postLoadCheckInternal(TokenPattern* tokenPattern, std::set<std::string>& patternElements) {
  AstTransformLookup::getInstance()->getAstTransform(tokenPattern->getId());

  // Unwind each pattern and check that any elements which will be matched with nested patterns has an AST transformer.
  for (auto k : *tokenPattern->getGroups()) {
    for (auto t : *k->getElements()) {
      if (patternElements.find(t->getData()) == patternElements.end()) {
        patternElements.insert(t->getData());

        if (NestedPatternLookup::getInstance()->isNest(t->getData())) {
          for (auto m : *NestedPatternLookup::getInstance()->getNested(t->getData())) {
            postLoadCheckInternal(m, patternElements);
          }
        }
      }
    }
  }
}
