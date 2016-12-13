#ifndef STREAM_DUMP_HPP_INLCUDED
#define STREAM_DUMP_HPP_INLCUDED

#include <vector>
#include <list>
#include <queue>

#include <echelon/parser/stage2/echelon-lookup.hpp>

#include <echelon/parser/token.hpp>
void stream_dump(std::ostream& s, const Token* t);

void stream_dump(std::ostream& s, std::vector<Token> tokens);

void stream_dump(std::ostream& s, std::list<Token*> tokens);

#include <echelon/parser/stage2/token-pattern-element.hpp>
void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement);

#include <echelon/parser/stage2/token-pattern-group.hpp>
void stream_dump(std::ostream& s, TokenPatternGroup* tokenPatternGroup);

#include <echelon/parser/stage2/token-pattern.hpp>
void stream_dump(std::ostream& s, TokenPattern* tokenPattern);

#include <echelon/parser/stage2/enhanced-token.hpp>
void stream_dump(std::ostream& s, EnhancedToken* enhancedToken);

void stream_dump(std::ostream& s, std::list<EnhancedToken*>* enhancedToken);

#include <echelon/ast/ast-node.hpp>
void stream_dump(std::ostream& s, AstNode* node, int level = 1);

void stream_dump(std::ostream& s, std::queue<AstNode*>* astNodeQueue);


#endif
