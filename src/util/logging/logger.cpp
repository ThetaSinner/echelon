#include <echelon/util/logging/logger.hpp>

Logger::Logger(StreamLogger* streamLogger) : streamLogger(streamLogger) {
  nullLogger = new NullStreamLogger();
}

void Logger::setLevel(int loggerLevel) {
  this -> loggerLevel = loggerLevel;
}

StreamLogger& Logger::at(StreamLoggerLevel loggerLevel) {
  if (levelToInt(loggerLevel) >= this -> loggerLevel) {
    return *streamLogger;
  }
  else {
    return *nullLogger;
  }
}
