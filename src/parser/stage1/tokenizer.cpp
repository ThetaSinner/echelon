#include <echelon/parser/stage1/tokenizer.hpp>

#include <algorithm> // std::min

#include <echelon/parser/stage1/character-pattern-lookup.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig);

bool matchSequenceGroup(std::list<CharacterPatternGroup *>::iterator &group,
                        std::string::iterator &ig);

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig);

bool checkUpperBound(int val, int upperBound) {
  return upperBound == -1 || val < upperBound;
}

std::list<Token *> Tokenizer::tokenize(std::string input) {
  auto log = LoggerSharedInstance::get();

  std::list<Token *> tokens;

  auto i = input.begin();

  while (i != input.end()) {
    if (*i == ' ' || *i == '\n') {
      i++;
      continue;
    }

    auto i_progress_check = i;

    auto patternList = CharacterPatternLookup::getInstance()->getCharacterPatternList();
    for (auto pattern : *patternList) {
      auto ip = i;
      bool patternMatches = true;
      auto groups = pattern->getGroups();
      for (auto group = groups->begin(); group != groups->end(); group++) {

        auto ig = ip;
        bool groupMatches;
        if ((*group)->getType() == CharacterPatternGroupType::Union) {
          groupMatches = matchUnionGroup(groups, group, ig);
        } else {
          groupMatches = matchSequenceGroup(group, ig);
        }

        if (groupMatches) {
          std::advance(ip, ig - ip);
        } else {
          patternMatches = false;
          break;
        }
      }

      if (patternMatches) {
        int beginOffset = 0;
        if (pattern->getGroups()->front()->isDoNotConsumeConsume()) {
          beginOffset = 1 * (pattern->getGroups()->front()->getType() == CharacterPatternGroupType::Sequence
                             ? pattern->getGroups()->front()->getElements()->size() : 1);
        }

        int endOffset = 0;
        if (pattern->getGroups()->back()->isDoNotConsumeConsume()) {
          endOffset = 1 * (pattern->getGroups()->back()->getType() == CharacterPatternGroupType::Sequence
                           ? pattern->getGroups()->back()->getElements()->size() : 1);
        }

        std::string token_data = input.substr((i - input.begin()) + beginOffset, (ip - i) - beginOffset - endOffset);
#ifdef ECHELON_DEBUG
        log->at(Level::Debug) << "Token data [" << token_data << "]\n";
#endif
        tokens.push_back(new Token(token_data, pattern->getTokenType()));
        std::advance(i, ip - i);
      }

      if (i == input.end()) {
        break;
      }
    }

    if (i == i_progress_check) {
      int error_begin = (int) (i - input.begin());
      int error_chars = std::min((int) (input.size() - (i - input.begin())), 10);
      std::string failed_chars = input.substr((unsigned) error_begin, (unsigned) error_chars);
      std::string message = "Unrecognised character sequence [" + failed_chars + "]";
      throw std::runtime_error(message);
    }
  }

  return tokens;
}

bool matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig) {
  bool groupMatches = false;

  auto elements = (*group)->getElements();
  int elementMatchCount = 0;
  while (true) {

    bool noProgress = true;
    for (auto element = elements->begin(); element != elements->end(); element++) {
      auto matcher = (*element)->getMatcher();

      if (checkUpperBound(elementMatchCount, (*group)->getRepeatUpperBound()) &&
          matcher(*ig) &&
          !matchLookahead(element, group, groups, ig)) {
        ig++;
        elementMatchCount++;

        while (checkUpperBound(elementMatchCount, (*group)->getRepeatUpperBound()) &&
               matcher(*ig) &&
               !matchLookahead(element, group, groups, ig)) {
          ig++;
          elementMatchCount++;
        }

        noProgress = false;
        break;
      }
    }

    if (noProgress) {
      break;
    }
  }

  if (elementMatchCount >= (*group)->getRepeatLowerBound()) {
    groupMatches = true;
  }

  return groupMatches;
}

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig) {

  bool nextGroupMatches = false;
  if ((*element)->isUseLookahead()) {
    auto next_group = std::next(group, 1);
    std::string::iterator ig_copy = ig;
    if ((*next_group)->getType() == CharacterPatternGroupType::Union) {
      nextGroupMatches = matchUnionGroup(groups, next_group, ig_copy);
    } else {
      nextGroupMatches = matchSequenceGroup(next_group, ig_copy);
    }
  }

  return nextGroupMatches;
}

bool matchSequenceGroup(std::list<CharacterPatternGroup *>::iterator &group,
                        std::string::iterator &ig) {
  bool groupMatches = true;

  auto elements = (*group)->getElements();
  for (auto element = elements->begin(); element != elements->end(); element++) {
    auto matcher = (*element)->getMatcher();

    if (matcher(*ig)) {
      ig++;
    } else {
      groupMatches = false;
      break;
    }
  }

  return groupMatches;
}