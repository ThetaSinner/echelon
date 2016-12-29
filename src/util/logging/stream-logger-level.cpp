#include <echelon/util/logging/stream-logger-level.hpp>

int levelToInt(StreamLoggerLevel streamLoggerLevel) {
  switch(streamLoggerLevel) {
    case StreamLoggerLevel::Off:
      return 7;
    case StreamLoggerLevel::Fatal:
      return 6;
    case StreamLoggerLevel::Error:
      return 5;
    case StreamLoggerLevel::Warn:
      return 4;
    case StreamLoggerLevel::Info:
      return 3;
    case StreamLoggerLevel::Debug:
      return 2;
    case StreamLoggerLevel::Trace:
      return 1;
  }
}