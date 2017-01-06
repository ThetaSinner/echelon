#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <map>
#include <list>
#include <string>

#include <echelon/ast/ast-node.hpp>

class Scope {
  std::map<std::string, AstNode *> variables;
  std::map<std::string, std::list<EnhancedAstNode *> *> functions;
public:
  void addVariable(std::string name, AstNode *astNode) {
    variables.insert({name, astNode});
  }

  bool hasVariable(std::string name) {
    return variables.find(name) != variables.end();
  }

  void addFunction(std::string name, EnhancedAstNode *enhancedAstNode) {
    if (!hasFunction(name)) {
      functions.insert({name, new std::list<EnhancedAstNode *>});
    }

    functions.at(name)->push_back(enhancedAstNode);
  }

  bool hasFunction(std::string name) {
    return functions.find(name) != functions.end();
  }

  std::list<EnhancedAstNode *> *getFunctions(std::string name) {
    return functions.at(name);
  }
};

#endif
