#ifndef STREAM_DUMP_HPP_INLCUDED
#define STREAM_DUMP_HPP_INLCUDED

#include <echelon/util/logging/logger-shared-instance.hpp>

#include <vector>
#include <list>
#include <queue>

#include <echelon/parser/echelon-lookup.hpp>

#include <echelon/parser/stage1/token.hpp>
void stream_dump(StreamLoggerLevel level, const Token *t);

void stream_dump(StreamLoggerLevel level, std::vector<Token> tokens);

void stream_dump(StreamLoggerLevel level, std::list<Token*> tokens);

#include <echelon/parser/stage2/token-pattern-element.hpp>
void stream_dump(StreamLoggerLevel level, TokenPatternElement* tokenPatternElement);

#include <echelon/parser/stage2/token-pattern-group.hpp>
void stream_dump(StreamLoggerLevel level, TokenPatternGroup* tokenPatternGroup);

#include <echelon/parser/stage2/token-pattern.hpp>
void stream_dump(StreamLoggerLevel level, TokenPattern* tokenPattern);

#include <echelon/parser/stage2/enhanced-token.hpp>
void stream_dump(StreamLoggerLevel level, EnhancedToken* enhancedToken);

void stream_dump(StreamLoggerLevel level, std::list<EnhancedToken*>* enhancedToken);

#include <echelon/ast/ast-node.hpp>
void stream_dump(StreamLoggerLevel level, AstNode* node, int dump_level = 1);

void stream_dump(StreamLoggerLevel level, std::queue<AstNode*>* astNodeQueue);

#include <echelon/parser/stage1/character-pattern.hpp>
void stream_dump(StreamLoggerLevel level, CharacterPattern* characterPattern);

#include <echelon/parser/stage1/character-pattern-group.hpp>
void stream_dump(StreamLoggerLevel level, CharacterPatternGroup* characterPatternGroup);

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>
void stream_dump(StreamLoggerLevel level, EnhancedAstNode* enhancedAstNode, int dump_level = 1);

#endif
