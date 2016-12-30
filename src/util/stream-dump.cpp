#include <echelon/util/stream-dump.hpp>

auto log = LoggerSharedInstance::get();

void stream_dump(StreamLoggerLevel level, const Token* t) {
  log->at(level) << "`" << t -> getData() << ", " << EchelonLookup::getInstance() -> toString(t -> getTokenType()) << "`";
}

void stream_dump(StreamLoggerLevel level, std::vector<Token> tokens) {
  for (auto& i : tokens) {
    log->at(level) << EchelonLookup::toString(i.getTokenType()) << " ["<< i.getData() << "], ";
  }
}

void stream_dump(StreamLoggerLevel level, std::list<Token*> tokens) {
  for (auto& i : tokens) {
    log->at(level) << EchelonLookup::toString(i -> getTokenType()) << " ["<< i -> getData() << "], ";
  }
}

void stream_dump(StreamLoggerLevel level, TokenPatternElement* tokenPatternElement) {
  log->at(level) << tokenPatternElement -> getData();
}

void stream_dump(StreamLoggerLevel level, TokenPatternGroup* tokenPatternGroup) {
  log->at(level) << "'";
  int sp = false;
  for (auto& i : *tokenPatternGroup -> getElements()) {
    if (sp) {
      log->at(level) << " ";
    }
    stream_dump(level, i);
    sp = true;
  }
  log->at(level) << "'{" << tokenPatternGroup -> getRepeatLowerBound() << "," << tokenPatternGroup -> getRepeatUpperBound() << "}";
}

void stream_dump(StreamLoggerLevel level, TokenPattern* tokenPattern) {
  for (auto& i : *tokenPattern -> getGroups()) {
    stream_dump(level, i);
    log->at(level) << " ";
  }
}

void stream_dump(StreamLoggerLevel level, EnhancedToken* enhancedToken) {
  log->at(level) << enhancedToken -> getData() << ", " << EchelonLookup::getInstance() -> toString(enhancedToken -> getTokenType());
}

void stream_dump(StreamLoggerLevel level, std::list<EnhancedToken*>* enhancedToken) {
  std::list<EnhancedToken*> copy(*enhancedToken);
  while (copy.size() > 0) {
    stream_dump(level, * copy.end());
    log->at(level) << "\n";
    copy.pop_back();
  }
}

void stream_dump(StreamLoggerLevel level, AstNode* node, int dump_level) {
  log->at(level) << "Level " << dump_level << "\n";

  log->at(level) << EchelonLookup::toString(node -> getType()) << ", ";
  log->at(level) << node -> getData();
  log->at(level) << "\n";

  for (int i = 0; i < node -> getChildCount(); i++) {
    stream_dump(level, node -> getChild(i), dump_level + 1);
    log->at(level) << "\n";
  }
}

void stream_dump(StreamLoggerLevel level, std::queue<AstNode*>* astNodeQueue) {
  std::queue<AstNode*> queueCopy(*astNodeQueue);
  while (!queueCopy.empty()) {
    stream_dump(level, queueCopy.front());
    log->at(level) << "\n";
    queueCopy.pop();
  }
}

void stream_dump(StreamLoggerLevel level, CharacterPattern* characterPattern) {
  for (auto g : * characterPattern -> getGroups()) {
    for (auto e : * g -> getElements()) {
      log->at(level) << e -> getData() << " ";
    }
  }
}

void stream_dump(StreamLoggerLevel level, CharacterPatternGroup* characterPatternGroup) {
  for (auto e : * characterPatternGroup -> getElements()) {
    log->at(level) << e -> getData() << " ";
  }
}

void stream_dump(StreamLoggerLevel level, EnhancedAstNode* enhancedAstNode, int dump_level) {
  log->at(level) << "Level " << dump_level << "\n";

  log->at(level) << EchelonLookup::toString(enhancedAstNode -> getNodeType()) << ", ";
  log->at(level) << EchelonLookup::toString(enhancedAstNode -> getNodeSubType()) << ", ";
  log->at(level) << enhancedAstNode -> getData();
  log->at(level) << "\n";

  for (auto child : *enhancedAstNode -> getChildList()) {
    stream_dump(level, child, dump_level + 1);
    log->at(level) << "\n";
  }
}