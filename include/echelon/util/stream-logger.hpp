#ifndef STREAM_LOGGER_HPP_INCLUDED
#define STREAM_LOGGER_HPP_INCLUDED

#include <fstream>
#include <iostream>

enum class StreamLoggerLevel {
    Trace,
    Critical,
};

using Level = StreamLoggerLevel;

class StreamLogger {
  std::ostream* out;

protected:
  StreamLogger() {}
public:
  StreamLogger(std::ostream* os);

  virtual StreamLogger& operator<<(std::string str);
};

class NullStreamLogger : public StreamLogger {
public:
  StreamLogger& operator<<(std::string str);
};

class Logger {
  StreamLogger *streamLogger;
  StreamLogger *nullLogger;
  StreamLoggerLevel loggerLevel;

public:
  Logger(StreamLogger* streamLogger);

  void setLevel(StreamLoggerLevel loggerLevel);

  StreamLogger& at(StreamLoggerLevel loggerLevel);
};

class LoggerFactory {
public:
  static Logger getConsoleLogger();
  static Logger getFileLogger(std::string ref);
};

#endif
