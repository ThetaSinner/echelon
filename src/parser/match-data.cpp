#include <echelon/parser/match-data.hpp>

bool MatchData::isTerminate() {
  return terminate || terminateLen > 0;
}
