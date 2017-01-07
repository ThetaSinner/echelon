#include <echelon/parser/stage1/tokenizer.hpp>

#include <algorithm> // std::min

#include <echelon/parser/stage1/character-pattern-lookup.hpp>
#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/to-string.hpp>

int matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig,
                     std::string::iterator &input_end);

bool matchSequenceGroup(std::list<CharacterPatternGroup *>::iterator &group,
                        std::string::iterator &ig);

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig,
                    std::string::iterator &input_end);

bool checkUpperBound(int val, int upperBound) {
  return upperBound == -1 || val < upperBound;
}

std::list<Token *> Tokenizer::tokenize(std::string input) {
  auto log = LoggerSharedInstance::get();

  std::list<Token *> tokens;

  auto i = input.begin();
  auto input_end = input.end();

  while (i != input.end()) {
    if (*i == ' ' || *i == '\n') {
      i++;
      continue;
    }

    auto i_progress_check = i;

    auto patternList = CharacterPatternLookup::getInstance()->getCharacterPatternList();
    for (auto pattern : *patternList) {
      log->at(Level::Debug) << to_string(pattern) << "\n";

      auto ip = i;
      bool patternMatches = true;
      auto groups = pattern->getGroups();
      std::list<unsigned> groupMatchInfo;
      for (auto group = groups->begin(); group != groups->end(); group++) {
        log->at(Level::Debug) << to_string(*group) << "\n";

        auto ig = ip;
        bool groupMatches = false;
        if ((*group)->getType() == CharacterPatternGroupType::Union) {
          int elementMatchCount = matchUnionGroup(groups, group, ig, input_end);

          if (elementMatchCount >= (*group)->getRepeatLowerBound()) {
            groupMatches = true;
          }
        } else {
          groupMatches = matchSequenceGroup(group, ig);
        }

        if (groupMatches) {
          log->at(Level::Debug) << "Group consume: " << std::distance(ip, ig) << "\n";
          groupMatchInfo.push_back(static_cast<unsigned>(std::distance(ip, ig)));
          std::advance(ip, ig - ip);
        } else {
          patternMatches = false;
          break;
        }
      }

      if (patternMatches) {
        unsigned long beginOffset = 0;
        if (pattern->getGroups()->front()->isDoNotConsumeConsume()) {
          beginOffset = 1 * (pattern->getGroups()->front()->getType() == CharacterPatternGroupType::Sequence
                             ? pattern->getGroups()->front()->getElements()->size() : 1);
        }

        unsigned long endOffset = 0;
        // Only check the offset if the back group actually matched something.
        if (groupMatchInfo.back() != 0) {
          if (pattern->getGroups()->back()->isDoNotConsumeConsume()) {
            endOffset = 1 * (pattern->getGroups()->back()->getType() == CharacterPatternGroupType::Sequence
                             ? pattern->getGroups()->back()->getElements()->size() : 1);
          }
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
      // TODO std::min takes negative numbers, if the tokenizer fails then this may too.
      int error_chars = std::min((int) (input.size() - (i - input.begin())), 10);
      std::string failed_chars = input.substr((unsigned) error_begin, (unsigned) error_chars);
      std::string message = "Unrecognised character sequence [" + failed_chars + "]";
      throw std::runtime_error(message);
    }
  }

  return tokens;
}

int matchUnionGroup(std::list<CharacterPatternGroup *> *groups,
                     std::list<CharacterPatternGroup *>::iterator &group,
                     std::string::iterator &ig,
                     std::string::iterator& input_end) {

  auto elements = (*group)->getElements();
  int elementMatchCount = 0;
  while (true) {

    bool noProgress = true;
    for (auto element = elements->begin(); element != elements->end(); element++) {
      LoggerSharedInstance::get()->at(Level::Debug) << to_string(*element) << "\n";
      auto matcher = (*element)->getMatcher();

      if (checkUpperBound(elementMatchCount, (*group)->getRepeatUpperBound()) &&
          matcher(*ig) &&
          !matchLookahead(element, group, groups, ig, input_end)) {
        ig++;
        elementMatchCount++;

        while (ig != input_end &&
               checkUpperBound(elementMatchCount, (*group)->getRepeatUpperBound()) &&
               matcher(*ig) &&
               !matchLookahead(element, group, groups, ig, input_end)) {
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

    if (ig == input_end) {
      break;
    }
  }

  return elementMatchCount;
}

bool matchLookahead(std::list<CharacterPatternElement *>::iterator &element,
                    std::list<CharacterPatternGroup *>::iterator &group,
                    std::list<CharacterPatternGroup *> *groups,
                    std::string::iterator &ig,
                    std::string::iterator &input_end) {

  bool nextGroupMatches = false;
  if ((*element)->isUseLookahead()) {
    auto next_group = std::next(group, 1);
    std::string::iterator ig_copy = ig;
    if ((*next_group)->getType() == CharacterPatternGroupType::Union) {
      int groupMatchCount = matchUnionGroup(groups, next_group, ig_copy, input_end);

      // If the next group has a non-zero lower bound then the condition must be met.
      if ((*next_group)->getRepeatLowerBound() > 0) {
        if (groupMatchCount > (*next_group)->getRepeatLowerBound()) {
          nextGroupMatches = true;
        }
      }
      else if (groupMatchCount > 0) {
        // If the next group has a zero lower bound then only allow match if something was actually matched.
        // This behaviour exists because a lookahead shouldn't match unless something was actually matched.
        nextGroupMatches = true;
      }
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