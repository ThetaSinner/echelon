#ifndef CUSTOM_TYPE_HANDLER_HPP_INCLUDED
#define CUSTOM_TYPE_HANDLER_HPP_INCLUDED

#include <echelon/model/internal/enhanced-ast-node.hpp>
#include <echelon/model/internal/enhanced-ast-block-node.hpp>
#include <echelon/compiler/echelon-compiler.hpp>

class CustomTypeHandler {
  static EnhancedAstNode* findFunction(EnhancedAstNode* customType, std::string name);

public:
  static void handle(EnhancedAstNode* customType);
};

#endif
