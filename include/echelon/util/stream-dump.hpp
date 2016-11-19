#ifndef STREAM_DUMP_HPP_INLCUDED
#define STREAM_DUMP_HPP_INLCUDED

#include <echelon/parser/token.hpp>
void stream_dump(std::ostream& s, const Token* t) {
  s << "`" << t -> getData() << ", " << EchelonLookup::getInstance() -> toString(t -> getTokenType()) << "`";
}

void stream_dump(std::ostream& s, std::vector<Token> tokens) {
  for (auto& i : tokens) {
    s << toString(i.getTokenType()) << " ["<< i.getData() << "], ";
  }
}

void stream_dump(std::ostream& s, std::list<Token*> tokens) {
  for (auto& i : tokens) {
    s << toString(i -> getTokenType()) << " ["<< i -> getData() << "], ";
  }
}

#include <echelon/parser/stage2/token-pattern-element.hpp>
void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement) {
  s << tokenPatternElement -> getData();
}

#include <echelon/parser/stage2/token-pattern-group.hpp>
void stream_dump(std::ostream& s, TokenPatternGroup* tokenPatternGroup) {
  s << "'";
  int sp = false;
  for (auto& i : *tokenPatternGroup -> getElements()) {
    if (sp) {
      s << " ";
    }
    stream_dump(s, i);
    sp = true;
  }
  s << "'{" << tokenPatternGroup -> getRepeatLowerBound() << "," << tokenPatternGroup -> getRepeatUpperBound() << "}";
}

#include <echelon/parser/stage2/token-pattern.hpp>
void stream_dump(std::ostream& s, TokenPattern* tokenPattern) {
  for (auto& i : *tokenPattern -> getGroups()) {
    stream_dump(s, i);
    s << " ";
  }
}

#include <echelon/parser/stage2/enhanced-token.hpp>
void stream_dump(std::ostream& s, EnhancedToken* enhancedToken) {
  s << enhancedToken -> getData() << ", " << EchelonLookup::getInstance() -> toString(enhancedToken -> getTokenType());
}

#include <echelon/ast/AstNode.hpp>
void stream_dump(std::ostream& s, AstNode* node, int level = 1) {
  s << "Level " << level << "\n";

  s << EchelonLookup::toString(node -> getType()) << ", ";
  s << node -> getData();
  s << "\n";

  for (int i = 0; i < node -> getChildCount(); i++) {
    stream_dump(s, node -> getChild(i), level + 1);
    s << "\n";
  }
}

#endif
