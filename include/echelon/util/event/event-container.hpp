#ifndef EVENT_CONTAINER_HPP_INCLUDED
#define EVENT_CONTAINER_HPP_INCLUDED

#include <map>
#include <list>
#include <functional>

class EventKey;
typedef std::function<void(EventKey&, void*)> EventListener;
typedef std::list<EventListener>::iterator EventListenerIterator;

#include <echelon/util/event/event-key.hpp>

class EventContainer {
  std::map<std::string, std::list<EventListener>> eventListeners;

public:
  void addEventListener(std::string eventId, EventListener eventListener) {
    if (eventListeners.find(eventId) == eventListeners.end()) {
      eventListeners.insert({eventId, {eventListener}});
    }
    else {
      eventListeners.at(eventId).push_back(eventListener);
    }
  }

  void removeEventListener(EventKey& eventKey) {
    eventListeners.at(eventKey.getEventId()).erase(eventKey.getEventListenerIterator());
    // Iterator is now invalid.
  }

  void triggerEvent(std::string eventId, void* eventData) {
    if (eventListeners.find(eventId) != eventListeners.end()) {
      std::list<EventListener>& list = eventListeners.at(eventId);
      for (auto i = list.begin(); i != list.end(); i++) {
        // TODO catch exceptions here? Not really necessary but might be nice to log here.
        EventKey eventKey(eventId, i);
        (*i)(eventKey, eventData);
      }
    }
  }
};

#endif
