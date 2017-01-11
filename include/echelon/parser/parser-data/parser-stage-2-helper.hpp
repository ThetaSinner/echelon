#ifndef PARSER_STAGE_2_HELPER_HPP_INCLUDED
#define PARSER_STAGE_2_HELPER_HPP_INCLUDED

#include <echelon/parser/stage2/ast-transform-data.hpp>
#include <echelon/ast/ast-node.hpp>

class ParserStage2Helper {
public:
  static void mapSubProcessAsBlock(AstTransformData* astTransformData, AstNode* target);
};

#endif
