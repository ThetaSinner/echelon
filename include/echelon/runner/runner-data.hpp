#ifndef RUNNER_DATA_HPP_INCLUDED
#define RUNNER_DATA_HPP_INCLUDED

#include <string>

class RunnerData {
  std::string script;

public:
  void setScript(std::string script);
  std::string getScript();
};

#endif
