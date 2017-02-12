#include <echelon/runner/runner-data.hpp>

void RunnerData::setScript(std::string script) {
  this ->script = script;
}

std::string RunnerData::getScript() {
  return script;
}