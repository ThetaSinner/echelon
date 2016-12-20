#ifndef SPIDER_MONKEY_JS_CODE_GENERATOR_HPP_INCLUDED
#define SPIDER_MONKEY_JS_CODE_GENERATOR_HPP_INCLUDED

#include <echelon/code-generation/code-generator.hpp>

class SpiderMonkeyJSCodeGenerator : public CodeGenerator {
public:
  std::string generate(AstNode* root);
};

#endif
