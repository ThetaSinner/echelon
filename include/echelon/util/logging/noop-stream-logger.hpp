#ifndef NOOP_STREAM_LOGGER_HPP_INCLUDED
#define NOOP_STREAM_LOGGER_HPP_INCLUDED

#include <echelon/util/logging/stream-logger.hpp>

class NullStreamLogger : public StreamLogger {
public:
  StreamLogger& operator<<(std::string str);
};

#endif
