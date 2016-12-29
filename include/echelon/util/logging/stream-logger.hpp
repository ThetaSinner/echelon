#ifndef STREAM_LOGGER_HPP_INCLUDED
#define STREAM_LOGGER_HPP_INCLUDED

#include <fstream>
#include <iostream>

class StreamLogger {
  std::ostream* out;

protected:
  StreamLogger() {}
public:
  StreamLogger(std::ostream* os);

  virtual StreamLogger& operator<<(std::string str);
};


#endif
