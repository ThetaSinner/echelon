#include <cxxtest/TestSuite.h>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/parser/parser-data/parser-stage-1-data-load.hpp>
#include <echelon/parser/parser-data/parser-stage-2-data-load.hpp>
#include <echelon/compiler/echelon-compiler.hpp>
#include <echelon/transform/transform-data/type-rule-data-load.hpp>

class NameResolverTestSuite : public CxxTest::TestSuite {
private:
  EchelonCompiler compiler;

public:
  NameResolverTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));

    loadParserStage1Data();
    loadParserStage2Data();

    loadTypeRuleData();
  }

  void testReturnVariableFromOtherModule() {
    auto model = compiler.enhance("module ModuleOne {\n  integer x = 5\n}\nmodule ModuleTwo {\n  function getX() {\n    ModuleOne::x\n  }\n}")->getEnhancedAstNode();

    TS_ASSERT_EQUALS(2, model->getChildCount());
    auto moduleTwo = model->getChild(1)->getChild(EnhancedAstNodeType::Block);

    TS_ASSERT(moduleTwo->hasChild(EnhancedAstNodeType::Function));
    auto function = moduleTwo->getChild(EnhancedAstNodeType::Function);

    TS_ASSERT(function->getChild(EnhancedAstNodeType::TypeName));
    auto typeName = function->getChild(EnhancedAstNodeType::TypeName);

    TS_ASSERT_EQUALS("integer", typeName->getData());
  }
};