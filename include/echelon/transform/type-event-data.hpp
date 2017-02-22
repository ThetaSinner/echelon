#ifndef TYPE_EVENT_GROUP_DATA_HPP_INCLUDED
#define TYPE_EVENT_GROUP_DATA_HPP_INCLUDED

/**
 * Data object to be used by type events.
 */
class TypeEventData {
  int eventGroupSize = 0;
public:
  /**
   * Set the event group size.
   *
   * @param eventGroupSize the event group size.
   */
  void setEventGroupSize(int eventGroupSize) {
    this->eventGroupSize = eventGroupSize;
  }

  /**
   * Get the event group size.
   *
   * @return the event group size.
   */
  int getEventGroupSize() {
    return eventGroupSize;
  }
};

#endif
