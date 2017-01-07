#include <echelon/parser/stage2/parser.hpp>

#include <echelon/parser/stage2/token-pattern-lookup.hpp>

#include <echelon/util/logging/logger-shared-instance.hpp>
#include <echelon/util/echelon-lookup.hpp>
#include <echelon/util/to-string.hpp>

// TODO report which pattern was the closest match and which token failed to match.
// TODO e.g. a function call which is passed a float but expr doesn't allow floats.

// patternMatches, it, tokens.end(), patternMatchInfo, *p, g
bool Parser2::isTerminateForEndOfProgram(bool &patternMatches, std::list<Token *>::iterator current_token,
                                         std::list<Token *>::iterator tokens_end,
                                         PatternMatchInfo *patternMatchInfo, TokenPattern *pattern,
                                         std::vector<TokenPatternGroup *>::iterator &current_group) {
  if (current_token == tokens_end) {
    if (!isAllowMatchAtEndOfProgram(patternMatchInfo, pattern, current_group)) {
      patternMatches = false;
    }

    // The tokens have run out, so whether or not the pattern matches we stop processing.
    return true;
  }

  return false;
}

ParserInternalOutput Parser2::_parse(ParserInternalInput &parserInternalInput) {
  auto log = LoggerSharedInstance::get();

  ParserInternalOutput output;

  AstConstructionManager astConstructionManager;

  std::list<Token *> tokens = *parserInternalInput.getTokens();
  auto i = tokens.begin();
  while (i != tokens.end()) {
    bool somePatternMatches = false;
    auto tokenPatterns = selectPatternList(parserInternalInput);

    log->at(Level::Debug) << "Start processing at token " << to_string(*i) << "\n";

    for (auto p = tokenPatterns->begin(); p != tokenPatterns->end(); p++) {
      log->at(Level::Debug) << "Trying pattern " << to_string(*p) << "\n";

      std::queue<AstNode *> subProcessAstNodes, nestedAstNodes;
      auto it = i;
      bool patternMatches = true;

      // Create a pattern match info to track group matches etc.
      PatternMatchInfo *patternMatchInfo = new PatternMatchInfo((*p)->getGroups()->size());

      // match each group in this pattern against the token.
      for (auto g = (*p)->getGroups()->begin(); g != (*p)->getGroups()->end(); g++) {
        log->at(Level::Debug) << "Process group " << to_string(*g) << "\n";

        /*
         * Handle the case where the tokens run out before the groups in this pattern.
         */
        if (isTerminateForEndOfProgram(patternMatches, it, tokens.end(), patternMatchInfo, *p, g)) {
          break;
        }

        auto itt = it;
        log->at(Level::Debug) << "Current starting token " << to_string(*itt) << "\n";

        int matchCount = 0;
        for (auto element = (*g)->getElements()->begin(); element != (*g)->getElements()->end(); element++) {
          EnhancedToken *enhancedToken = new EnhancedToken(*itt);

          log->at(Level::Debug) << "Element: " << to_string(*element) << "\n";
          log->at(Level::Debug) << "Matches: " << to_string(enhancedToken) << " ? ";

          if ((*element)->isSubProcess()) {
            doSubProcess(tokens, subProcessAstNodes, g, itt, matchCount);
          } else if ((*element)->isUseNestedPatterns()) {
            log->at(Level::Debug) << "Nested pattern.\n";

            ParserInternalInput subInput;
            std::list<Token *> subList(itt, tokens.end());
            subInput.setTokens(&subList);
            subInput.setNestedPatterns((*element)->getNestedPatterns());
            subInput.setUseNestedPatterns(true);

            auto subOutput = _parse(subInput);

            log->at(Level::Debug) << "Nested result.\n" << to_string(subOutput.getAstNode()) << "\n";

            if (!isEmptyProgram(subOutput.getAstNode())) {
              nestedAstNodes.push(subOutput.getAstNode());
              std::advance(itt, subOutput.getTokensConsumedCount());
              matchCount++;
            } else {
              // The level below didn't match, shortcut.
              break;
            }
          } else if ((*element)->getMatcher()->matches(enhancedToken)) {
            // The pattern matches directly using a matcher.

            log->at(Level::Debug) << "Yes\n";
            matchCount++;
            itt++;

            if (itt == tokens.end()) {
              log->at(Level::Debug) << "Ran out of tokens.\n";
              break;
            }
          } else {
            // This patten element can't be processed or matched.
            log->at(Level::Debug) << "No\n";
            break;
          }
        }

        bool completeGroupMatch = matchCount == (*g)->getElements()->size();
        log->at(Level::Debug) << "Complete group match? " << EchelonLookup::toString(completeGroupMatch) << "\n";

        if (completeGroupMatch) {
          int groupMatchCount = patternMatchInfo->increment(std::distance((*p)->getGroups()->begin(), g));

          // Go again if we're under the uppper bound or are allowed unlimited matches.
          if (groupMatchCount < (*g)->getRepeatUpperBound() || (*g)->getRepeatUpperBound() == -1) {
            log->at(Level::Debug) << "Match this group again.\n";
            g--; // repeat this group.
          } else {
            log->at(Level::Debug) << "Finished with this group. Move on.\n";
            // matched but no need to repeat, therefore we just let the loop continue;
          }
        } else {
          // No match.

          int groupMatchCount = patternMatchInfo->getGroupMatchCount(std::distance((*p)->getGroups()->begin(), g));

          if (groupMatchCount >= (*g)->getRepeatLowerBound()) {
            // We've actually matched enough to allow the match even though this one failed.
            log->at(Level::Debug) << "No group match, allowing anyway.\n";

            // However, we need to reset itt to give back the tokens we've used so far.
            itt = it;
          } else {
            patternMatches = false;
            break;
          }
        }

        // This group matches so we want to consume the tokens matched by this group.
        log->at(Level::Debug) << "Consume " << std::distance(it, itt) << " tokens.\n";
        // need to be more careful than this?
        std::advance(it, std::distance(it, itt));
      }

      if (patternMatches) {
        log->at(Level::Debug) << "Pattern matches.\n";
        somePatternMatches = true;

        // We've matched this whole pattern, so we want to consume tokens.
        //log -> at(Level::Debug) << "Confirm consume " << std::distance(i, it) << " tokens\n";
        std::list<EnhancedToken *> matchedTokens;
        for (int k = 0; k < std::distance(i, it); k++) {
          //log -> at(Level::Debug) << k << "\n"; stream_dump(log -> at(Level::Debug), *std::next(i, k)); log -> at(Level::Debug) << "\n";
          matchedTokens.push_back(new EnhancedToken(*std::next(i, k)));
        }

        AstTransformData *td = new AstTransformData();
        td->setSubProcessAstNodes(&subProcessAstNodes);
        td->setNestedAstNodes(&nestedAstNodes);
        td->setPatternMatchInfo(patternMatchInfo);
        td->setTokens(matchedTokens);

        auto transformer = AstTransformLookup::getInstance()->getAstTransform((*p)->getId());

        try {
          auto frag = transformer->transform(td);
          log->at(Level::Debug) << "frag:\n" << to_string(frag) << "\n";

          astConstructionManager.pushFragment(frag);
        }
        catch (std::out_of_range &e) {
          log->at(Level::Fatal) << "Transform to ast failed for pattern [" << (*p)->getId() << "] with error ["
                                << e.what() << "]\n";
          throw std::runtime_error("Transform to ast failed");
        }
        catch (...) {
          log->at(Level::Fatal) << "Transform to ast failed for pattern [" << (*p)->getId()
                                << "] with unknown reason\n";
          throw std::runtime_error("Transform to ast failed");
        }

        // after this point it is not safe to access i without checking against tokens.end()
        std::advance(i, std::distance(i, it));
        break;
      }
    }

    // After each pattern match pass, count the
    output.setTokensConsumedCount(std::distance(tokens.begin(), i));

    if (!somePatternMatches) {
      log->at(Level::Debug) << "No matching patterns for " << to_string(*i) << "\n";

      // This is the case where a sub-process has been requested, check if we can safely return control to the caller.
      if (parserInternalInput.getSubProcessFinishGroup() != nullptr) {
        std::list<Token *> subList(i, tokens.end());
        if (simpleGroupMatch(subList, parserInternalInput.getSubProcessFinishGroup())) {
          //log -> at(Level::Debug) << "Level above should handle this token and further tokens.\n";
          break;
        }
      } else if (parserInternalInput.isUseNestedPatterns()) {
        // TODO Infinite loops are possible here, but I don't know how to fix it right now..
        log->at(Level::Debug) << "Failed but we were using nested patterns so never mind?\n";
        break;
      } else {
        log->at(Level::Fatal) << "Unhandled tokens\n";
        for (int k = 0; k < 5; k++) {
          // TODO handle running off the end of i.
          log->at(Level::Fatal) << to_string(*std::next(i, k)) << "]\n";
        }
        throw std::runtime_error("Unhandled token [" + (*i)->getData() + ", " +
                                 EchelonLookup::getInstance()->toString((*i)->getTokenType()) + "]");
      }
    }
    else {
      // we found a pattern match.

      if (parserInternalInput.isUseNestedPatterns()) {
        // We found a matching pattern for the requested sub process. exit.
        break;
      }
    }
  }

  log->at(Level::Debug) << "built result:\n" << to_string(astConstructionManager.getRoot()) << "\n";
  output.setAstNode(astConstructionManager.getRoot());
  return output;
}

void Parser2::doSubProcess(std::list<Token *> &tokens, std::queue<AstNode *> &subProcessAstNodes,
                           const std::vector<TokenPatternGroup *>::iterator &g, std::list<Token *>::iterator &itt,
                           int &matchCount) {
  // Try to match this pattern element by starting a new parse from the working iterator's position.
  auto subOutput = this->subProcess(itt, tokens.end(), *(next(g, 1)));

  // Queue the sub process result for processing in an ast transformer.
  if (!this->isEmptyProgram(subOutput.getAstNode())) {
    subProcessAstNodes.push(subOutput.getAstNode());
  }

  // Advance the working iterator to skip the tokens which were matched in the sub process.
  advance(itt, subOutput.getTokensConsumedCount());
  // Since the recursive call hasn't thrown an exception the sub process succeeded, so this element is a match.
  matchCount++;
}

bool Parser2::simpleGroupMatch(std::list<Token *> &tokens, TokenPatternGroup *group) {
  auto it = tokens.begin();

  int matchCount = 0;
  for (auto element = group->getElements()->begin(); element != group->getElements()->end(); element++) {
    EnhancedToken *enhancedToken = new EnhancedToken(*it);

    if ((*element)->getMatcher()->matches(enhancedToken)) {
      matchCount++;
      it++;

      if (it == tokens.end()) {
        break;
      }
    } else {
      break;
    }
  }

  return matchCount == group->getElements()->size();
}

bool Parser2::isEmptyProgram(AstNode *program) {
  if (program == nullptr) {
    return true;
  }

  return program->getChildCount() == 0;
}

bool Parser2::isAllowMatchAtEndOfProgram(PatternMatchInfo *patternMatchInfo, TokenPattern *pattern,
                                         std::vector<TokenPatternGroup *>::iterator &current_group) {
  static auto log = LoggerSharedInstance::get();
  log->at(Level::Debug) << "End of program, but there may be more groups.\n";

  bool allowMatchAtEndOfProgram = true;
  for (auto group = current_group; group != pattern->getGroups()->end(); group++) {
    // TODO this isn't ideal.
    unsigned groupIndex = static_cast<unsigned> (std::distance(pattern->getGroups()->begin(), group));
    int groupMatchCount = patternMatchInfo->getGroupMatchCount(groupIndex);
    if (groupMatchCount < (*group)->getRepeatLowerBound()) {
      log->at(Level::Debug) << "Not allowing match at EOP.\n";
      allowMatchAtEndOfProgram = false;
      break;
    }
  }

  return allowMatchAtEndOfProgram;
}

std::list<TokenPattern *> *Parser2::selectPatternList(ParserInternalInput &parserInternalInput) {
  // If this is a nested call then use the nested patterns instead.
  if (parserInternalInput.isUseNestedPatterns()) {
    return parserInternalInput.getNestedPatterns();
  }

  return TokenPatternLookup::getInstance()->getTokenPatterns();
}

ParserInternalOutput Parser2::subProcess(std::list<Token *>::iterator start, std::list<Token *>::iterator end,
                                         TokenPatternGroup *nextGroup) {
  ParserInternalInput subInput;

  std::list<Token *> subList(start, end);
  subInput.setTokens(&subList);
  subInput.setSubProcessFinishGroup(nextGroup);

  return _parse(subInput);
}

AstNode *Parser2::parse(std::list<Token *> tokens) {
  ParserInternalInput parserInternalInput;
  parserInternalInput.setTokens(&tokens);

  auto out = _parse(parserInternalInput);

  return out.getAstNode();
}
