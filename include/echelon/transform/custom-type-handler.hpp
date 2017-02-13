#ifndef CUSTOM_TYPE_HANDLER_HPP_INCLUDED
#define CUSTOM_TYPE_HANDLER_HPP_INCLUDED

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-block-node.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

class CustomTypeHandler {
  static EnhancedAstNode* findFunction(EnhancedAstNode* customType, std::string name);

public:
  static void handle(EnhancedAstNode* customType);
};

#endif
