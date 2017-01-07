#ifndef STREAM_DUMP_HPP_INCLUDED
#define STREAM_DUMP_HPP_INCLUDED

#include <echelon/util/logging/logger-shared-instance.hpp>

#include <vector>
#include <list>
#include <queue>
#include <string>
#include <sstream>

#include <echelon/util/echelon-lookup.hpp>
#include <echelon/parser/stage1/token.hpp>
#include <echelon/parser/stage1/character-pattern.hpp>
#include <echelon/parser/stage2/token-pattern.hpp>
#include <echelon/parser/stage2/token-pattern-group.hpp>
#include <echelon/parser/stage2/token-pattern-element.hpp>
#include <echelon/parser/stage2/enhanced-token.hpp>
#include <echelon/ast/ast-node.hpp>
#include <echelon/parser/stage1/character-pattern-group.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

std::string to_string(const Token *t);
std::string to_string(const std::list<Token *> *tokens);

std::string to_string(EnhancedToken *enhancedToken);
std::string to_string(std::list<EnhancedToken *> *enhancedToken);

std::string to_string(TokenPattern *characterPattern);

std::string to_string(TokenPatternGroup *tokenPatternGroup);
std::string to_string(TokenPatternElement *tokenPatternElement);

std::string to_string(AstNode *node);
std::string to_string(EnhancedAstNode *enhancedAstNode);

#endif
