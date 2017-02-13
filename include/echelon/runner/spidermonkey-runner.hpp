#ifndef SPIDERMONKEY_RUNNER_HPP_INCLUDED
#define SPIDERMONKEY_RUNNER_HPP_INCLUDED

#ifdef WITH_SPIDER_MONKEY_RUNNER
#include <jsapi.h>
#include <js/Initialization.h>
#endif

#include <stdexcept>
#include <iostream>
#include <echelon/runner/runner-data.hpp>

void runner(RunnerData& runnerData);

#endif
