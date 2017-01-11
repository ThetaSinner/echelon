#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <map>
#include <list>
#include <string>

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class Scope {
  std::map<std::string, EnhancedAstNode*> variables;
  std::map<std::string, EnhancedAstNode*> modules;
  std::map<std::string, std::list<EnhancedAstNode*>> functions;
public:
  void addVariable(std::string name, EnhancedAstNode *enhancedAstNode) {
    variables.insert({name, enhancedAstNode});
  }

  bool hasVariable(std::string name) {
    return variables.find(name) != variables.end();
  }

  void addFunction(std::string name, EnhancedAstNode *enhancedAstNode) {
    functions.at(name).push_back(enhancedAstNode);
  }

  bool hasFunction(std::string name) {
    return functions.find(name) != functions.end();
  }

  std::list<EnhancedAstNode*>& getFunctions(std::string name) {
    return functions.at(name);
  }

  void addModule(std::string name, EnhancedAstNode* enhancedAstNode) {
    modules.insert({name, enhancedAstNode});
  }

  bool hasModule(std::string name) {
    return modules.find(name) != modules.end();
  }
};

#endif
