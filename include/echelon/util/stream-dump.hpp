#ifndef STREAM_DUMP_HPP_INLCUDED
#define STREAM_DUMP_HPP_INLCUDED

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

std::string to_string(const Token* t);
std::string to_string(const std::list<Token*>* tokens);
std::string to_string(EnhancedToken* enhancedToken);

std::string to_string(TokenPattern* characterPattern);
std::string to_string(TokenPatternGroup* tokenPatternGroup);
std::string to_string(TokenPatternElement* tokenPatternElement);

std::string to_string(AstNode* node, int to_string_level = 1);

// TODO switch from stream_dump to to_string everywhere.
void stream_dump(StreamLoggerLevel level, std::vector<Token> tokens);
void stream_dump(StreamLoggerLevel level, std::list<Token*> tokens);
void stream_dump(StreamLoggerLevel level, TokenPatternElement* tokenPatternElement);
void stream_dump(StreamLoggerLevel level, TokenPatternGroup* tokenPatternGroup);
void stream_dump(StreamLoggerLevel level, TokenPattern* tokenPattern);
void stream_dump(StreamLoggerLevel level, EnhancedToken* enhancedToken);
void stream_dump(StreamLoggerLevel level, std::list<EnhancedToken*>* enhancedToken);
void stream_dump(StreamLoggerLevel level, AstNode* node, int dump_level = 1);
void stream_dump(StreamLoggerLevel level, std::queue<AstNode*>* astNodeQueue);
void stream_dump(StreamLoggerLevel level, CharacterPattern* characterPattern);
void stream_dump(StreamLoggerLevel level, CharacterPatternGroup* characterPatternGroup);
void stream_dump(StreamLoggerLevel level, EnhancedAstNode* enhancedAstNode, int dump_level = 1);

#endif
