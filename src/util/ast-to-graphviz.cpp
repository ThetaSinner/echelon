#include <echelon/util/ast-to-graphviz.hpp>

#include <sstream>

#include <echelon/parser/stage2/echelon-lookup.hpp>

void _init(std::stringstream& stream) {
  stream << "digraph Program {" << std::endl;
}

void _finalise(std::stringstream& stream) {
  stream << "}" << std::endl;
}

void _toGraphviz(AstNode* currentNode, std::stringstream& stream, std::string tracker) {
  for (int i = 0; i < currentNode -> getChildCount(); i++) {
    stream << EchelonLookup::getInstance() -> toString(currentNode -> getType()) << "_" << tracker;
    stream << " -> ";
    stream << EchelonLookup::getInstance() -> toString(currentNode -> getChild(i) -> getType()) << "_" << tracker << "_" << i;
    if (currentNode -> getChild(i) -> getData() != "") {
      stream << " [label=\"" << currentNode -> getChild(i) -> getData() << "\"];";
    }
    else {
      stream << ";";
    }
    stream << std::endl;

    std::stringstream s;
    s << tracker << "_" << i;
    _toGraphviz(currentNode -> getChild(i), stream, s.str());
  }
}

std::string toGraphviz(AstNode* program) {
  std::stringstream ss;
  _init(ss);
  _toGraphviz(program, ss, "");
  _finalise(ss);
  return ss.str();
}
