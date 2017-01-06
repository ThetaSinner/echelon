#ifndef AST_TRANSFORM_LOOKUP_HPP_INCLUDED
#define AST_TRANSFORM_LOOKUP_HPP_INCLUDED

#include <map>
#include <string>

#include <echelon/parser/stage2/ast-transform.hpp>

class AstTransformLookup {
  static AstTransformLookup *self;

  std::map<std::string, AstTransform *> astTransformHash;

  AstTransformLookup() {};

  AstTransformLookup(const AstTransformLookup &_) {}

  void operator=(const AstTransformLookup &_) {}

public:
  static AstTransformLookup *getInstance();

  void addAstTransform(std::string key, AstTransform *astTransform);

  AstTransform *getAstTransform(std::string key);
};

#endif
