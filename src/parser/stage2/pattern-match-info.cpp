#include <echelon/parser/stage2/pattern-match-info.hpp>

void PatternMatchInfo::setGroupMatchCount(unsigned groupIndex, int count) {
  groupMatchCounts[groupIndex] = count;
}

int PatternMatchInfo::getGroupMatchCount(unsigned groupIndex) {
  return groupMatchCounts[groupIndex];
}

int PatternMatchInfo::increment(unsigned groupIndex) {
  setGroupMatchCount(groupIndex, getGroupMatchCount(groupIndex) + 1);
  return getGroupMatchCount(groupIndex);
}
