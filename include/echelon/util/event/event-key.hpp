#ifndef EVENT_KEY_HPP_INCLUDED
#define EVENT_KEY_HPP_INCLUDED

#include <echelon/util/event/event-container.hpp>

class EventKey {
  EventListenerIterator it;
  std::string eventId;
public:
  EventKey(std::string eventId, EventListenerIterator it) : eventId(eventId), it(it) {}

  EventListenerIterator getEventListenerIterator() {
    return it;
  }

  std::string getEventId() {
    return eventId;
  }
};

#endif
