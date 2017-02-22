#ifndef EVENT_LISTENER_RESULT_HPP_INCLUDED
#define EVENT_LISTENER_RESULT_HPP_INCLUDED

class EventListenerResult {
  bool removeListener = false;

public:
  void setRemoveListener(bool removeListener) {
    this->removeListener = removeListener;
  }

  bool isRemoveListener() {
    return removeListener;
  }
};

#endif
