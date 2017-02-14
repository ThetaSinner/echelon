#include <cxxtest/TestSuite.h>
#include <echelon/model/ast/ast-node.hpp>
#include <echelon/model/ast/ast-node-type-enum.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/compiler/integrity-check.hpp>

#include <fstream>
#include <sstream>
#include <string>

class AstTestSuite : public CxxTest::TestSuite {
public:
  AstTestSuite() {
    // Log fatal errors only.
    LoggerSharedInstance::get()->setLevel(levelToInt(Level::Fatal));
    IntegrityCheck::StartupCheck();
  }

  void testTypeProperty(void) {
    AstNode node;

    node.setType(AstNodeType::Block);

    TS_ASSERT_EQUALS(AstNodeType::Block, node.getType());
  }

  void testChildren(void) {
    AstNode node;

    TS_ASSERT_EQUALS(0, node.getChildCount());

    AstNode child;
    child.setType(AstNodeType::Block);
    node.putChild(&child);

    TS_ASSERT_EQUALS(1, node.getChildCount());
    TS_ASSERT_EQUALS(AstNodeType::Block, node.getChild(0)->getType());
  }
};
