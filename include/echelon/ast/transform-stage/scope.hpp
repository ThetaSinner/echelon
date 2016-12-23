#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <map>
#include <string>

#include <echelon/ast/ast-node.hpp>

class Scope {
  std::map<std::string, AstNode*> variables;
public:
  void addVariable(std::string name, AstNode* astNode) {
    variables.insert({name, astNode});
  }

  bool hasVariable(std::string name) {
    return variables.find(name) != variables.end();
  }
};

#endif
