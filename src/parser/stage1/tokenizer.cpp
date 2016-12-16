#include <echelon/parser/stage1/tokenizer.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#endif

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig);

bool matchSequenceGroup(std::list<CharacterPatternGroup *>::iterator &group,
                        std::string::iterator &ig);

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig);

void tokenize(std::string input, std::list<CharacterPattern*> patternList) {

  auto i = input.begin();

  while (i != input.end()) {
    if (*i == ' ' || *i == '\n') {
      i++;
      continue;
    }

    for (auto pattern : patternList) {

      auto ip = i;
      bool patternMatches = true;
      int groupMatchCount = 0;
      auto groups = pattern -> getGroups();
      for (auto group = groups -> begin(); group != groups -> end(); group++) {

        auto ig = ip;
        bool groupMatches;
        if ((*group) -> getType() == CharacterPatternGroupType::Union) {
          groupMatches = matchUnionGroup(groups, group, ig);
        }
        else {
          groupMatches = matchSequenceGroup(group, ig);
        }

        if (groupMatches) {
          std::advance(ip, ig - ip);

          if ((*group) -> isRepeatable()) {
            groupMatchCount++;
            group--;
          }
          else {
            groupMatchCount = 0;
          }
        }
        else {
          if (groupMatchCount == 0) {
            patternMatches = false;
          }

          break;
        }
      }

      if (patternMatches) {
        #ifdef ECHELON_DEBUG
        std::cout << input.substr(i - input.begin(), ip - i) << std::endl;
        #endif
        std::advance(i, ip - i);
      }

      if (i == input.end()) {
        break;
      }
    }
  }

}

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig) {
  bool groupMatches = false;

  auto elements = (*group) -> getElements();
  for (auto element = elements -> begin(); element != elements -> end(); element++) {
    auto matcher = (*element) -> getMatcher();

    if (matcher(*ig) && !matchLookahead(element, group, groups, ig)) {
      ig++;
      groupMatches = true;

      if ((*element) -> isRepeatable()) {
        while (matcher(*ig) && !matchLookahead(element, group, groups, ig)) {
          ig++;
        }
      }

      break;
    }
  }

  return groupMatches;
}

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig) {

  bool nextGroupMatches = false;
  if ((*element) -> isUseLookahead()) {
    auto next_group = std::next(group, 1);
    std::string::iterator ig_copy = ig;
    if ((*next_group) -> getType() == CharacterPatternGroupType::Union) {
      nextGroupMatches = matchUnionGroup(groups, next_group, ig_copy);
    }
    else {
      nextGroupMatches = matchSequenceGroup(next_group, ig_copy);
    }
  }

  return nextGroupMatches;
}

bool matchSequenceGroup(std::list<CharacterPatternGroup *>::iterator &group,
                        std::string::iterator &ig) {
  bool groupMatches = true;

  auto elements = (*group) -> getElements();
  for (auto element = elements -> begin(); element != elements -> end(); element++) {
    auto matcher = (*element) -> getMatcher();

    if (matcher(*ig)) {
      ig++;
    }
    else {
      groupMatches = false;
      break;
    }
  }

  return groupMatches;
}