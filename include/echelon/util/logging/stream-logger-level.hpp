#ifndef STREAM_LOGGER_LEVEL_HPP_INCLUDED
#define STREAM_LOGGER_LEVEL_HPP_INCLUDED

enum class StreamLoggerLevel {
  Trace,
  Debug,
  Info,
  Warn,
  Error,
  Fatal,

  Off,
};

int levelToInt(StreamLoggerLevel streamLoggerLevel);

using Level = StreamLoggerLevel;

#endif
