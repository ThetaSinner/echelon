#include <echelon/parser/stage2/ast-transform-lookup.hpp>

AstTransformLookup* AstTransformLookup::self = nullptr;

AstTransformLookup* AstTransformLookup::getInstance() {
  if (self == nullptr) {
    self = new AstTransformLookup();
  }

  return self;
}

void AstTransformLookup::addAstTransform(std::string key, AstTransform* astTransform) {
  astTransformHash.insert({key, astTransform});
}

AstTransform* AstTransformLookup::getAstTransform(std::string key) {
  #ifdef ECHELON_DEBUG
  if (astTransformHash.find(key) == astTransformHash.end()) {
    std::cout << key << "Missing ast transformer [" << key << "]";
    throw std::runtime_error("Request for missing ast transformer");
  }
  #endif

  return astTransformHash.at(key);
}
