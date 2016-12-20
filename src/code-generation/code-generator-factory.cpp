#include <echelon/code-generation/code-generator-factory.hpp>

#ifdef CODE_GENERATOR_SPIDER_MONKEY_JS
#include "spider-monkey-js/spider-monkey-js-code-generator.hpp"
#elif CODE_GENERATOR_ECHELON
#include "echelon/echelon-code-generator.hpp"
#endif

CodeGenerator* CodeGeneratorFactory::newCodeGenerator() {
  #ifdef CODE_GENERATOR_SPIDER_MONKEY_JS
  return new SpiderMonkeyJSCodeGenerator;
  #elif CODE_GENERATOR_ECHELON
  return new EchelonCodeGenerator();
  #endif
}