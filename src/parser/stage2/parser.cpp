#include <echelon/parser/stage2/parser.hpp>

#include <iostream>
#include <echelon/parser/stage2/pattern-translator.hpp>

ParserInternalOutput Parser2::_parse(ParserInternalInput& parserInternalInput) {
  ParserInternalOutput output;

  std::list<Token*> tokens = *parserInternalInput.getTokens();

  AstConstructionManager astConstructionManager;

  auto i = tokens.begin();
  while (i != tokens.end()) {

    //std::cout << "Start processing at token "; stream_dump(std::cout, *i); std::cout << "\n";

    bool somePatternMatches = false;
    for (auto p = tokenPatterns.begin(); p != tokenPatterns.end(); p++) {
      std::queue<AstNode*> subProcessAstNodes;

      bool patternMatches = true;

      //std::cout << "Trying pattern "; stream_dump(std::cout, *p); std::cout << "\n";

      auto it = i;

      // Create a pattern match info to track group matches etc.
      PatternMatchInfo *patternMatchInfo = new PatternMatchInfo((*p) -> getGroups() -> size());

      // match each group in this pattern against the token.
      for (auto g = (*p) -> getGroups() -> begin(); g != (*p) -> getGroups() -> end(); g++) {
        //std::cout << "Process group "; stream_dump(std::cout, *g); std::cout << "\n";

        auto itt = it;
        //std::cout << "Current starting token "; stream_dump(std::cout, *itt); std::cout << "\n";

        int matchCount = 0;
        for (auto element = (*g) -> getElements() -> begin(); element != (*g) -> getElements() -> end(); element++) {
          EnhancedToken *enhancedToken = new EnhancedToken(*itt);

          //std::cout << "Matches: {"; stream_dump(std::cout, enhancedToken); std::cout << "} ? ";

          if ((*element) -> isSubProcess()) {
            // Try to match this pattern element by starting a new parse from the working iterator's position.
            auto subOutput = subProcess(itt, tokens.end(), *(std::next(g, 1)));

            // Queue the sub process result for processing in an ast transformer.
            subProcessAstNodes.push(subOutput.getAstNode());
            // Advance the working iterator to skip the tokens which were matched in the sub process.
            std::advance(itt, subOutput.getTokensConsumedCount());
            // Since the recursive call hasn't thrown an exception the sub process suceeded, so this element is a match.
            matchCount++;
          }
          else if ((*element) -> getMatcher() -> matches(enhancedToken)) {
            // The pattern matches directly using a matcher.

            std::cout << "Yes\n" << std::endl;
            matchCount++;
            itt++;

            if (itt == tokens.end()) {
              //std::cout << "Ran out of tokens.\n";
              break;
            }
          }
          else {
            // This patten element can't be processed or matched.
            std::cout << "No\n";
            break;
          }
        }

        bool completeGroupMatch = matchCount == (*g) -> getElements() -> size();
        //std::cout << "Complete group match? " << toString(completeGroupMatch) << "\n";

        // doesn't match but is optional or repeating
        // does match and can't match again
        // does match and can match again

        if (completeGroupMatch) {
          int groupMatchCount = patternMatchInfo -> increment(std::distance((*p) -> getGroups() -> begin(), g));

          // Go again if we're under the uppper bound or are allowed unlimited matches.
          if (groupMatchCount < (*g) -> getRepeatUpperBound() || (*g) -> getRepeatUpperBound() == -1) {
            //std::cout << "Match this group again.\n";
            g--; // repeat this group.
          }
          else {
            //std::cout << "Finished with this group. Move on.\n";
            // matched but no need to repeat, therefore we just let the loop continue;
          }
        }
        else {
          // No match.

          int groupMatchCount = patternMatchInfo -> getGroupMatchCount(std::distance((*p) -> getGroups() -> begin(), g));

          if (groupMatchCount >= (*g) -> getRepeatLowerBound()) {
            // We've actually matched enough to allow the match even though this one failed.
            //std::cout << "No group match, allowing anyway.\n";

            // However, we need to reset itt to give back the tokens we've used so far.
            itt = it;
          }
          else {
            patternMatches = false;
            break;
          }
        }

        // This group matches so we want to consume the tokens matched by this group.
        //std::cout << "Consume " << std::distance(it, itt) << " tokens.\n";
        // need to be more careful than this?
        std::advance(it, std::distance(it, itt));
      }

      if (patternMatches) {
        //std::cout << "Pattern matches.\n";
        somePatternMatches = true;

        // We've matched this whole pattern, so we want to consume tokens.
        //std::cout << "Confirm consume " << std::distance(i, it) << " tokens\n";
        std::list<EnhancedToken*> matchedTokens;
        for (int k = 0; k < std::distance(i, it); k++) {
          //std::cout << k << "\n"; stream_dump(std::cout, *std::next(i, k)); std::cout << "\n";
          matchedTokens.push_back(new EnhancedToken(*std::next(i, k)));
        }

        AstTransformData *td = new AstTransformData();
        td -> setSubProcessAstNodes(&subProcessAstNodes);
        td -> setPatternMatchInfo(patternMatchInfo);
        td -> setTokens(matchedTokens);

        auto transformer = AstTransformLookup::getInstance() -> getAstTransform((*p) -> getId());

        // TODO
        auto frag = transformer -> transform(td);
        //std::cout << "frag:\n"; stream_dump(std::cout, frag); std::cout << "\n";
        astConstructionManager.pushFragment(frag);
        //stream_dump(std::cout, transformer -> transform(td));

        // after this point it is not safe to access i without checking against tokens.end()
        std::advance(i, std::distance(i, it));
        break;
      }
    }

    // After each pattern match pass, count the
    output.setTokensConsumedCount(std::distance(tokens.begin(), i));

    if (!somePatternMatches) {
      //std::cout << "No matching patterns for "; stream_dump(std::cout, *i); std::cout << "\n";

      // This is the case where a sub-process has been requested, check if we can safely return control to the caller.
      if (parserInternalInput.getSubProcessFinishGroup() != nullptr) {
        std::list<Token*> subList(i, tokens.end());
        if (simpleGroupMatch(subList, parserInternalInput.getSubProcessFinishGroup())) {
          //std::cout << "Level above should handle this token and further tokens.\n";
          break;
        }
      }
      else {
        //std::cerr << "ERROR, unhandled token. "; stream_dump(std::cout, *i); std::cout << "\n" ;
      }
    }
  }

  //std::cout << "built result:\n"; stream_dump(std::cout, astConstructionManager.getRoot()); std::cout << "\n";
  output.setAstNode(astConstructionManager.getRoot());
  return output;
}

bool Parser2::simpleGroupMatch(std::list<Token*>& tokens, TokenPatternGroup* group) {
  //std::cout << "Simple group match.\n";
  auto it = tokens.begin();

  int matchCount = 0;
  for (auto element = group -> getElements() -> begin(); element != group -> getElements() -> end(); element++) {
    EnhancedToken *enhancedToken = new EnhancedToken(*it);

    //std::cout << "Matches: {"; stream_dump(std::cout, enhancedToken); std::cout << "} ? ";

    if ((*element) -> getMatcher() -> matches(enhancedToken)) {
      //std::cout << "Yes\n" << std::endl;
      matchCount++;
      it++;

      if (it == tokens.end()) {
        //std::cout << "Ran out of tokens.\n";
        break;
      }
    }
    else {
      //std::cout << "No\n";
      break;
    }
  }

  return matchCount == group -> getElements() -> size();
}

ParserInternalOutput Parser2::subProcess(std::list<Token*>::iterator start, std::list<Token*>::iterator end, TokenPatternGroup* nextGroup) {
  ParserInternalInput subInput;

  std::list<Token*> subList(start, end);
  subInput.setTokens(&subList);
  subInput.setSubProcessFinishGroup(nextGroup);

  return _parse(subInput);
}

AstNode* Parser2::parse(std::list<Token*> tokens) {
  ParserInternalInput parserInternalInput;
  parserInternalInput.setTokens(&tokens);

  auto out = _parse(parserInternalInput);

  return out.getAstNode();
}

void Parser2::addTokenPattern(TokenPattern* tokenPattern) {
  tokenPatterns.push_back(tokenPattern);
}

void Parser2::addTokenPattern(std::string id, std::string tokenPattern) {
  static PatternTranslator patternTranslator;
  auto pattern = patternTranslator.translate(tokenPattern);
  pattern -> setId(id);
  tokenPatterns.push_back(pattern);
}
