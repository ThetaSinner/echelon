#include <echelon/util/logging/stream-logger.hpp>

StreamLogger::StreamLogger(std::ostream *os) : out(os) {
}

StreamLogger &StreamLogger::operator<<(std::string str) {
  *(out) << str;
  out->flush(); // this is overkill but if we want logging right up to crashes it is needed...
  return *this;
}

StreamLogger &StreamLogger::operator<<(int i) {
  *(out) << i;
  out->flush();
  return *this;
}