#ifndef TRANSFORM_WORKING_DATA_HPP_INCLUDED
#define TRANSFORM_WORKING_DATA_HPP_INCLUDED

#include <echelon/util/event/event-container.hpp>

/**
 * Data specific to compilation of a single program.
 */
class TransformWorkingData {
  EventContainer eventContainer;
  
public:
  /**
   * Get the event container instance.
   *
   * @return the event container.
   */
  EventContainer& getEventContainer() {
    return eventContainer;
  }
};

#endif
