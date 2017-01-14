#include <echelon/transform/type-deducer.hpp>

void TypeDeducer::deduceTypes(AstNode *root) {
  // need a node to work from. expression or bool expr etc.
  // scope pointer,
  // a target to map the type onto

  // register listeners for each type we don't have.
  // if all listeners complete then figure out this type.

  // if no listeners are registered then trigger event for this type completing.
  // it's fine to send the event, anything waiting for this type will complete and anything which hasn't registered a listener yet
  // will just be able to resolve this type immediately. To parallelize all types should resolve and none should be triggered until
  // this method won't be called again.
}