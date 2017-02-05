#include <echelon/util/to-string.hpp>
#include <echelon/ast/transform-stage/enhanced-ast-function-prototype-node.hpp>

std::string to_string(const Token *t) {
  std::stringstream ss;
  // clion code analysis bug.
  ss << "`" << t->getData() << ", " << EchelonLookup::getInstance()->toString(t->getTokenType()) << "`";
  return ss.str();
}

std::string to_string(const std::list<Token *> *tokens) {
  std::stringstream ss;
  for (auto& i : *tokens) {
    ss << EchelonLookup::toString(i->getTokenType()) << " [" << i->getData() << "] " << "line: [" << i->getSourceMapData().getLineNumber() << "], char: [" << i->getSourceMapData().getCharacterNumber() << "]\n";
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
  ss << characterPattern->getId() << ": ";
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

std::string to_string(CharacterPattern* characterPattern) {
  std::stringstream ss;
  ss << EchelonLookup::toString(characterPattern->getTokenType()) << " : ";
  for (auto g : *characterPattern->getGroups()) {
    ss << to_string(g);
  }
  ss << "\n";
  return ss.str();
}

std::string to_string(CharacterPatternGroup* characterPatternGroup) {
  std::stringstream ss;
  ss << "[";
  ss << EchelonLookup::getInstance()->toString(characterPatternGroup->getType());
  ss << ": ";
  for (auto e : *characterPatternGroup->getElements()) {
    ss << to_string(e);
    ss << ", ";
  }

  ss << "]{" << characterPatternGroup->getRepeatLowerBound() << "," << characterPatternGroup->getRepeatUpperBound() << "} ";

  return ss.str();
}

std::string to_string(CharacterPatternElement* characterPatternElement) {
  std::stringstream ss;
  ss << characterPatternElement->getData();
  return ss.str();
}

std::string to_string_internal(AstNode *node, int to_string_level) {
  std::stringstream ss;
  ss << "Level " << to_string_level << "\n";

  ss << EchelonLookup::toString(node->getType()) << ", ";
  ss << node->getData();
  ss << "\n";

  for (unsigned i = 0; i < node->getChildCount(); i++) {
    ss << to_string_internal(node->getChild(i), to_string_level + 1);
    ss << "\n";
  }

  return ss.str();
}

std::string to_string(AstNode *node) {
  return to_string_internal(node, 1);
}

std::string to_string_internal(EnhancedAstNode *enhancedAstNode, int dump_level) {
  std::stringstream ss;

  ss << "Level " << dump_level << "\n";

  // clion overloading analysis bug.
  ss << "type=[" << EchelonLookup::toString(enhancedAstNode->getNodeType()) << "], "
     << "data=[" << enhancedAstNode->getData() << "], "
     << "sub_type=[" << EchelonLookup::toString(enhancedAstNode->getNodeSubType()) << "]";

  if (enhancedAstNode->getNodeType() == EnhancedAstNodeType::Function && enhancedAstNode->getNodeSubType() == EnhancedAstNodeSubType::Prototype) {
    auto impl = ((EnhancedAstFunctionPrototypeNode*) enhancedAstNode)->getImpl();

    if (impl == nullptr) {
      ss << ", no impl";
    }
    else {
      ss << ", has impl ";
    }
  }

  ss << "\n";

  for (auto child : *enhancedAstNode->getChildList()) {
    ss << to_string_internal(child, dump_level + 1) << "\n";
  }

  return ss.str();
}

std::string to_string(EnhancedAstNode *enhancedAstNode) {
  return to_string_internal(enhancedAstNode, 1);
}