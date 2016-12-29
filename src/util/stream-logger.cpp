#include <echelon/util/stream-logger.hpp>

#include <typeinfo>

int levelNum(StreamLoggerLevel streamLoggerLevel) {
  switch(streamLoggerLevel) {
    case StreamLoggerLevel::Critical:
      return 1;
    case StreamLoggerLevel::Trace:
      return 0;
  }
}

StreamLogger::StreamLogger(std::ostream* os) : out(os) {
}

StreamLogger& StreamLogger::operator<<(std::string str) {
  *(out) << str;
  out -> flush();
  return *this;
}

StreamLogger& NullStreamLogger::operator<<(std::string str) {
  return *this;
}

Logger::Logger(StreamLogger* streamLogger) : streamLogger(streamLogger) {
  nullLogger = new NullStreamLogger();
}

void Logger::setLevel(StreamLoggerLevel loggerLevel) {
  this -> loggerLevel = loggerLevel;
}

StreamLogger& Logger::at(StreamLoggerLevel loggerLevel) {
  if (levelNum(loggerLevel) >= levelNum(this -> loggerLevel)) {
    return *streamLogger;
  }
  else {
    return *nullLogger;
  }
}

// TODO assign level from preprocessor symbols.

Logger LoggerFactory::getConsoleLogger() {
  return Logger(new StreamLogger(&std::cout));
}

Logger LoggerFactory::getFileLogger(std::string ref) {
  // TODO weak pointer for automatic resource release.
  std::ofstream *f = new std::ofstream(ref);
  return Logger(new StreamLogger(f));
}