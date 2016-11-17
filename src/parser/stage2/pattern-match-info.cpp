#include <echelon/parser/stage2/pattern-match-info.hpp>

void PatternMatchInfo::setGroupMatchCount(int groupIndex, int count) {
  groupMatchCounts[groupIndex] = count;
}

int PatternMatchInfo::getGroupMatchCount(int groupIndex) {
  return groupMatchCounts[groupIndex];
}

int PatternMatchInfo::increment(int groupIndex) {
  setGroupMatchCount(groupIndex, getGroupMatchCount(groupIndex) + 1);
  return getGroupMatchCount(groupIndex);
}
