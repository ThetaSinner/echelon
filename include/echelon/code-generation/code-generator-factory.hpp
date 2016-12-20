#ifndef CODE_GENERATOR_FACTORY_HPP_INCLUDED
#define CODE_GENERATOR_FACTORY_HPP_INCLUDED

#include <echelon/code-generation/code-generator.hpp>

class CodeGeneratorFactory {
public:
    static CodeGenerator* newCodeGenerator();
};

#endif
