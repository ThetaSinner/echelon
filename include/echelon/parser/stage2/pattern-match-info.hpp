#ifndef PATTERN_MATCH_INFO_HPP_INCLUDED
#define PATTERN_MATCH_INFO_HPP_INCLUDED

#include <vector>

class PatternMatchInfo {
  std::vector<int> groupMatchCounts;

public:
  PatternMatchInfo(int groupSize) : groupMatchCounts(groupSize) {
  }

  void setGroupMatchCount(unsigned groupIndex, int count);
  int getGroupMatchCount(unsigned groupIndex);
  int increment(unsigned groupIndex);
};

#endif
