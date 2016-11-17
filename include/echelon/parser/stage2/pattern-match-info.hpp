#ifndef PATTERN_MATCH_INFO_HPP_INCLUDED
#define PATTERN_MATCH_INFO_HPP_INCLUDED

#include <vector>

class PatternMatchInfo {
  std::vector<int> groupMatchCounts;

public:
  PatternMatchInfo(int groupSize) : groupMatchCounts(groupSize) {
  }

  void setGroupMatchCount(int groupIndex, int count);
  int getGroupMatchCount(int groupIndex);
  int increment(int groupIndex);
};

#endif
