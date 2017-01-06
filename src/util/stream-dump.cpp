#include <echelon/util/stream-dump.hpp>

auto log = LoggerSharedInstance::get();

std::string to_string(const Token *t) {
  std::stringstream ss;
  ss << "`" << t->getData() << ", " << EchelonLookup::getInstance()->toString(t->getTokenType()) << "`";
  return ss.str();
}

std::string to_string(const std::list<Token *> *tokens) {
  std::stringstream ss;
  for (auto &i : *tokens) {
    ss << EchelonLookup::toString(i->getTokenType()) << " [" << i->getData() << "], ";
  }
  return ss.str();
}

std::string to_string(std::list<EnhancedToken *> *enhancedToken) {
  std::stringstream ss;
  std::list<EnhancedToken *> copy(*enhancedToken);
  while (copy.size() > 0) {
    ss << to_string(copy.back());
    ss << "\n";
    copy.pop_back();
  }

  return ss.str();
}

std::string to_string(TokenPattern *characterPattern) {
  std::stringstream ss;
  for (auto g : *characterPattern->getGroups()) {
    for (auto e : *g->getElements()) {
      ss << e->getData() << " ";
    }
  }
  return ss.str();
}

std::string to_string(TokenPatternGroup *tokenPatternGroup) {
  std::stringstream ss;

  ss << "'";
  int sp = false;
  for (auto &i : *tokenPatternGroup->getElements()) {
    if (sp) {
      ss << " ";
    }
    ss << to_string(i);
    sp = true;
  }
  ss << "'{" << tokenPatternGroup->getRepeatLowerBound() << "," << tokenPatternGroup->getRepeatUpperBound() << "}";

  return ss.str();
}

std::string to_string(TokenPatternElement *tokenPatternElement) {
  std::stringstream ss;
  ss << tokenPatternElement->getData();
  return ss.str();
}

std::string to_string(EnhancedToken *enhancedToken) {
  std::stringstream ss;
  ss << enhancedToken->getData() << ", " << EchelonLookup::getInstance()->toString(enhancedToken->getTokenType());
  return ss.str();
}

std::string to_string(AstNode *node, int to_string_level) {
  std::stringstream ss;
  ss << "Level " << to_string_level << "\n";

  ss << EchelonLookup::toString(node->getType()) << ", ";
  ss << node->getData();
  ss << "\n";

  for (int i = 0; i < node->getChildCount(); i++) {
    ss << to_string(node->getChild(i), to_string_level + 1);
    ss << "\n";
  }

  return ss.str();
}

void stream_dump(StreamLoggerLevel level, TokenPatternElement *tokenPatternElement) {
  log->at(level) << tokenPatternElement->getData();
}

void stream_dump(StreamLoggerLevel level, TokenPatternGroup *tokenPatternGroup) {
  log->at(level) << "'";
  int sp = false;
  for (auto &i : *tokenPatternGroup->getElements()) {
    if (sp) {
      log->at(level) << " ";
    }
    stream_dump(level, i);
    sp = true;
  }
  log->at(level) << "'{" << tokenPatternGroup->getRepeatLowerBound() << "," << tokenPatternGroup->getRepeatUpperBound()
                 << "}";
}

void stream_dump(StreamLoggerLevel level, TokenPattern *tokenPattern) {
  for (auto &i : *tokenPattern->getGroups()) {
    stream_dump(level, i);
    log->at(level) << " ";
  }
}

void stream_dump(StreamLoggerLevel level, EnhancedToken *enhancedToken) {
  log->at(level) << enhancedToken->getData() << ", "
                 << EchelonLookup::getInstance()->toString(enhancedToken->getTokenType());
}

void stream_dump(StreamLoggerLevel level, std::list<EnhancedToken *> *enhancedToken) {
  std::list<EnhancedToken *> copy(*enhancedToken);
  while (copy.size() > 0) {
    stream_dump(level, *copy.end());
    log->at(level) << "\n";
    copy.pop_back();
  }
}

void stream_dump(StreamLoggerLevel level, AstNode *node, int dump_level) {
  log->at(level) << "Level " << dump_level << "\n";

  log->at(level) << EchelonLookup::toString(node->getType()) << ", ";
  log->at(level) << node->getData();
  log->at(level) << "\n";

  for (int i = 0; i < node->getChildCount(); i++) {
    stream_dump(level, node->getChild(i), dump_level + 1);
    log->at(level) << "\n";
  }
}

void stream_dump(StreamLoggerLevel level, std::queue<AstNode *> *astNodeQueue) {
  std::queue<AstNode *> queueCopy(*astNodeQueue);
  while (!queueCopy.empty()) {
    stream_dump(level, queueCopy.front());
    log->at(level) << "\n";
    queueCopy.pop();
  }
}

void stream_dump(StreamLoggerLevel level, CharacterPattern *characterPattern) {
  for (auto g : *characterPattern->getGroups()) {
    for (auto e : *g->getElements()) {
      log->at(level) << e->getData() << " ";
    }
  }
}

void stream_dump(StreamLoggerLevel level, CharacterPatternGroup *characterPatternGroup) {
  for (auto e : *characterPatternGroup->getElements()) {
    log->at(level) << e->getData() << " ";
  }
}

void stream_dump(StreamLoggerLevel level, EnhancedAstNode *enhancedAstNode, int dump_level) {
  log->at(level) << "Level " << dump_level << "\n";

  log->at(level) << "type=[" << EchelonLookup::toString(enhancedAstNode->getNodeType()) << "], ";
  log->at(level) << "data=[" << enhancedAstNode->getData() << "], ";
  log->at(level) << "sub_type=[" << EchelonLookup::toString(enhancedAstNode->getNodeSubType()) << "]";
  log->at(level) << "\n";

  for (auto child : *enhancedAstNode->getChildList()) {
    stream_dump(level, child, dump_level + 1);
    log->at(level) << "\n";
  }
}