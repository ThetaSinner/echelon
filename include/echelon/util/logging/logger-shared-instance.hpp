#ifndef LOGGER_SHARED_INSTANCE_HPP_INCLUDED
#define LOGGER_SHARED_INSTANCE_HPP_INCLUDED

#include <echelon/util/logging/logger.hpp>
#include <echelon/util/logging/stream-logger.hpp>

#include <fstream>
#include <iostream>

class LoggerSharedInstance {
  static Logger *logger;

public:
  static Logger *get();
};

#endif
