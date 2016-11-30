#include <echelon/util/stream-dump.hpp>

// otherwise c++ claims there's no match for operator << on std::ostream.
#include <iostream>

void stream_dump(std::ostream& s, const Token* t) {
  s << "`" << t -> getData() << ", " << EchelonLookup::getInstance() -> toString(t -> getTokenType()) << "`";
}

void stream_dump(std::ostream& s, std::vector<Token> tokens) {
  for (auto& i : tokens) {
    s << EchelonLookup::toString(i.getTokenType()) << " ["<< i.getData() << "], ";
  }
}

void stream_dump(std::ostream& s, std::list<Token*> tokens) {
  for (auto& i : tokens) {
    s << EchelonLookup::toString(i -> getTokenType()) << " ["<< i -> getData() << "], ";
  }
}

void stream_dump(std::ostream& s, TokenPatternElement* tokenPatternElement) {
  s << tokenPatternElement -> getData();
}

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

void stream_dump(std::ostream& s, TokenPattern* tokenPattern) {
  for (auto& i : *tokenPattern -> getGroups()) {
    stream_dump(s, i);
    s << " ";
  }
}

void stream_dump(std::ostream& s, EnhancedToken* enhancedToken) {
  s << enhancedToken -> getData() << ", " << EchelonLookup::getInstance() -> toString(enhancedToken -> getTokenType());
}

void stream_dump(std::ostream& s, AstNode* node, int level) {
  s << "Level " << level << "\n";

  s << EchelonLookup::toString(node -> getType()) << ", ";
  s << node -> getData();
  s << "\n";

  s << "Children: " << node -> getChildCount() << "\n";
  for (int i = 0; i < node -> getChildCount(); i++) {
    stream_dump(s, node -> getChild(i), level + 1);
    s << "\n";
  }
  std::cout << "Finished level " << level << "\n";
}

void stream_dump(std::ostream& s, std::queue<AstNode*>* enhancedToken) {
  std::queue<AstNode*> queueCopy(*enhancedToken);
  while (!queueCopy.empty()) {
    stream_dump(s, queueCopy.front());
    std::cout << std::endl;
    queueCopy.pop();
  }
}
