#include <echelon/util/logging/noop-stream-logger.hpp>

StreamLogger& NullStreamLogger::operator<<(std::string str) {
  return *this;
}