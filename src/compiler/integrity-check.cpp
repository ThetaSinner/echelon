#include <echelon/compiler/integrity-check.hpp>

#include <echelon/ast/AstNodeType.hpp>
#include <echelon/parser/token-type-enum.hpp>
#include <echelon/parser/keyword-enum.hpp>
#include <echelon/parser/stage2/echelon-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <echelon/util/enum-class-iterator.hpp>
#endif

void IntegrityCheck::StartupCheck() {
  #ifdef ECHELON_DEBUG
  for (auto tte : Enum<TokenTypeEnum>()) {
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
