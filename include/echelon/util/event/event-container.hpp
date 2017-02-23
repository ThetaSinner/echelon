#ifndef EVENT_CONTAINER_HPP_INCLUDED
#define EVENT_CONTAINER_HPP_INCLUDED

#include <map>
#include <list>
#include <queue>
#include <functional>

#include <echelon/util/event/event-listener-result.hpp>

typedef std::function<EventListenerResult(void*)> EventListener;
typedef std::list<EventListener>::iterator EventListenerIterator;

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

  void triggerEvent(std::string eventId, void* eventData) {
    if (eventListeners.find(eventId) != eventListeners.end()) {
      auto& events = eventListeners.at(eventId);
      auto numEvents = events.size();

      std::queue<int> toRemove;
      for (int i = 0; i < numEvents; i++) {
        // TODO catch exceptions here? Not really necessary but might be nice to log here.
        auto iter = std::next(events.begin(), i);
        auto eventListenerResult = (*iter)(eventData);

        if (eventListenerResult.isRemoveListener()) {
          toRemove.push(i);
        }
      }

      int offset = 0;
      while (toRemove.size()) {
        events.erase(std::next(events.begin(), toRemove.front() - offset));
        offset--;
        toRemove.pop();
      }
    }
  }

  bool hasListeners() {
    return !eventListeners.empty();
  }
};

#endif
