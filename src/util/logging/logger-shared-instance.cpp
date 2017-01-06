#include <echelon/util/logging/logger-shared-instance.hpp>

Logger *LoggerSharedInstance::logger = nullptr;

// TODO assign level from preprocessor symbols.

Logger *LoggerSharedInstance::get() {
  if (logger == nullptr) {
#ifdef LOG_FILE
    std::ofstream *f = new std::ofstream(LOG_FILE);
    logger = new Logger(new StreamLogger(f));
#else
    logger = new Logger(new StreamLogger(&std::cout));
#endif

#if LOG_LEVEL > 0
    logger -> setLevel(LOG_LEVEL);
#else
    logger->setLevel(levelToInt(Level::Trace));
#endif
  }

  return logger;
}