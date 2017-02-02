#include <echelon/util/ast-to-graphviz.hpp>

#include <sstream>

#include <echelon/util/echelon-lookup.hpp>
#include <regex>

// dot compiler-ast-out.gv -Tjpg -ocompiler-ast-out.jpg && compiler-ast-out.jpg

std::string no_whitespace(std::string input) {
  static std::regex regex("\\s");
  return std::regex_replace(input, regex, "_");
}

void _init(std::stringstream &stream) {
  stream << "digraph Program {" << std::endl;
}

void _finalise(std::stringstream &stream) {
  stream << "}" << std::endl;
}

void _toGraphviz(AstNode *currentNode, std::stringstream &stream, std::string tracker) {
  for (int i = 0; i < currentNode->getChildCount(); i++) {
    stream << EchelonLookup::getInstance()->toString(currentNode->getType()) << "_" << tracker;
    stream << " -> ";
    stream << EchelonLookup::getInstance()->toString(currentNode->getChild(i)->getType()) << "_" << tracker << "_" << i;
    if (currentNode->getChild(i)->getData() != "") {
      stream << " [label=\"" << currentNode->getChild(i)->getData() << "\"];";
    } else {
      stream << ";";
    }
    stream << std::endl;

    std::stringstream s;
    s << tracker << "_" << i;
    _toGraphviz(currentNode->getChild(i), stream, s.str());
  }
}

void _toGraphviz(EnhancedAstNode* currentNode, std::stringstream &stream, std::string tracker) {
  for (int i = 0; i < currentNode->getChildCount(); i++) {
    stream << no_whitespace(EchelonLookup::getInstance()->toString(currentNode->getNodeType())) << "_";
    if (currentNode->getNodeSubType() != EnhancedAstNodeSubType::Unspecified) {
      stream << no_whitespace(EchelonLookup::getInstance()->toString(currentNode->getNodeSubType())) << "_";
    }
    stream << tracker;

    stream << " -> ";
    stream << no_whitespace(EchelonLookup::getInstance()->toString(currentNode->getChild(i)->getNodeType())) << "_";
    if (currentNode->getChild(i)->getNodeSubType() != EnhancedAstNodeSubType::Unspecified) {
      stream << no_whitespace(EchelonLookup::getInstance()->toString(currentNode->getChild(i)->getNodeSubType())) << "_";
    }
    stream << tracker << "_"
           << i;

    if (currentNode->getChild(i)->getData() != "") {
      stream << " [label=\"" << currentNode->getChild(i)->getData() << "\"];";
    } else {
      stream << ";";
    }
    stream << std::endl;

    std::stringstream s;
    s << tracker << "_" << i;
    _toGraphviz(currentNode->getChild(i), stream, s.str());
  }
}

std::string toGraphviz(AstNode *program) {
  std::stringstream ss;
  _init(ss);
  _toGraphviz(program, ss, "");
  _finalise(ss);
  return ss.str();
}

std::string toGraphviz(EnhancedAstNode* program) {
  std::stringstream ss;
  _init(ss);
  _toGraphviz(program, ss, "");
  _finalise(ss);
  return ss.str();
}
