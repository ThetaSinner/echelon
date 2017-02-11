#ifndef SPIDERMONKEY_RUNNER_HPP_INCLUDED
#define SPIDERMONKEY_RUNNER_HPP_INCLUDED

#ifdef WITH_SPIDER_MONKEY_RUNNER
#include <js/Initialization.h>
int runner();
#else
// TODO no runner.
int runner() {
  return 1;
}
#endif

#endif
