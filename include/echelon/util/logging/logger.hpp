#ifndef LOGGER_HPP_INCLUDED
#define LOGGER_HPP_INCLUDED

#include <echelon/util/logging/stream-logger.hpp>
#include <echelon/util/logging/noop-stream-logger.hpp>
#include <echelon/util/logging/stream-logger-level.hpp>

class Logger {
  StreamLogger *streamLogger;
  StreamLogger *nullLogger;
  int loggerLevel;

public:
  Logger(StreamLogger* streamLogger);

  void setLevel(int loggerLevel);

  StreamLogger& at(StreamLoggerLevel loggerLevel);
};

#endif
