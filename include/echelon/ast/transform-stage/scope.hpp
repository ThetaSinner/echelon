#ifndef SCOPE_HPP_INCLUDED
#define SCOPE_HPP_INCLUDED

#include <map>
#include <list>
#include <string>

#include <echelon/ast/transform-stage/enhanced-ast-node.hpp>

class Scope {
  std::map<std::string, EnhancedAstNode*> variables;
  std::map<std::string, EnhancedAstNode*> paramDefinitions;
  std::map<std::string, EnhancedAstNode*> modules;
  std::map<std::string, EnhancedAstNode*> types;
  std::map<std::string, std::list<EnhancedAstNode*>> functions;
  std::map<std::string, EnhancedAstNode*> functionImplementations;
  std::map<std::string, std::list<EnhancedAstNode*>> prototypes;
public:
  void addVariable(std::string name, EnhancedAstNode *enhancedAstNode) {
    variables.insert({name, enhancedAstNode});
  }

  bool hasVariable(std::string name) {
    return variables.find(name) != variables.end();
  }

  EnhancedAstNode* getVariable(std::string name) {
    return variables.at(name);
  }

  void addType(std::string name, EnhancedAstNode* enhancedAstNode) {
    types.insert(std::make_pair(name, enhancedAstNode));
  }

  bool hasType(std::string name) {
    return types.find(name) != types.end();
  }

  EnhancedAstNode* getType(std::string name) {
    return types.at(name);
  }

  void addFunction(std::string name, EnhancedAstNode *enhancedAstNode) {
    if (!hasFunction(name)) {
      functions.insert({name, std::list<EnhancedAstNode*>()});
    }

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

  EnhancedAstNode* getModule(std::string name) {
    return modules.at(name);
  }

  bool hasPrototype(std::string name) {
    return prototypes.find(name) != prototypes.end();
  }

  void addPrototype(std::string name, EnhancedAstNode* enhancedAstNode) {
    if (!hasPrototype(name)) {
      prototypes.insert({name, std::list<EnhancedAstNode*>()});
    }

    prototypes.at(name).push_back(enhancedAstNode);
  }

  std::list<EnhancedAstNode*>* getPrototypes(std::string name) {
    return &prototypes.at(name);
  }

  void addParamDefinition(std::string name, EnhancedAstNode* enhancedAstNode) {
    paramDefinitions.insert(std::make_pair(name, enhancedAstNode));
  }

  bool hasParamDefinition(std::string name) {
    return paramDefinitions.find(name) != paramDefinitions.end();
  }

  EnhancedAstNode* getParamDefinition(std::string name) {
    return paramDefinitions.at(name);
  }

  // TODO [[deprecated]]
  bool hasFunctionImplementation(std::string functionHash) {
    return functionImplementations.find(functionHash) != functionImplementations.end();
  }

  // TODO [[deprecated]]
  void addFunctionImplementation(std::string functionHash, EnhancedAstNode* enhancedAstNode) {
    // TODO do the other function types need lists or would hashing work nicely?
    functionImplementations.insert({functionHash, enhancedAstNode});
  }
};

#endif
