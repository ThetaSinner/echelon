#include <echelon/parser/stage2/parser.hpp>

#include <echelon/parser/stage2/token-pattern-lookup.hpp>

#ifdef ECHELON_DEBUG
#include <iostream>
#include <echelon/util/stream-dump.hpp>
#endif

ParserInternalOutput Parser2::_parse(ParserInternalInput& parserInternalInput) {
  ParserInternalOutput output;

  std::list<Token*> tokens = *parserInternalInput.getTokens();

  AstConstructionManager astConstructionManager;

  auto i = tokens.begin();
  while (i != tokens.end()) {

    #ifdef ECHELON_DEBUG
    std::cout << "Start processing at token "; stream_dump(std::cout, *i); std::cout << "\n";
    #endif

    bool somePatternMatches = false;
    auto tokenPatterns = TokenPatternLookup::getInstance() -> getTokenPatterns();
    // If this is a nested call then use the nested patterns instead.
    if (parserInternalInput.isUseNestedPatterns()) {
      tokenPatterns = parserInternalInput.getNestedPatterns();
    }

    for (auto p = tokenPatterns -> begin(); p != tokenPatterns -> end(); p++) {
      std::queue<AstNode*> subProcessAstNodes;
      std::queue<AstNode*> nestedAstNodes;

      bool patternMatches = true;

      #ifdef ECHELON_DEBUG
      std::cout << "Trying pattern "; stream_dump(std::cout, *p); std::cout << "\n";
      #endif

      auto it = i;

      // Create a pattern match info to track group matches etc.
      PatternMatchInfo *patternMatchInfo = new PatternMatchInfo((*p) -> getGroups() -> size());

      // match each group in this pattern against the token.
      for (auto g = (*p) -> getGroups() -> begin(); g != (*p) -> getGroups() -> end(); g++) {
        #ifdef ECHELON_DEBUG
        std::cout << "Process group "; stream_dump(std::cout, *g); std::cout << std::endl;
        #endif

        if (it == tokens.end()) {
          #ifdef ECHELON_DEBUG
          std::cout << "End of program, but there are more groups." << std::endl;
          #endif
          int groupMatchCount = patternMatchInfo -> getGroupMatchCount(std::distance((*p) -> getGroups() -> begin(), g));
          std::cout << groupMatchCount << std::endl;
          if (groupMatchCount >= (*g) -> getRepeatLowerBound()) {
            #ifdef ECHELON_DEBUG
            std::cout << "Allowing match at EOP." << std::endl;
            #endif
            continue;
          }
          else {
            #ifdef ECHELON_DEBUG
            std::cout << "Not allowing match at EOP." << std::endl;
            #endif
            patternMatches = false;
            break;
          }
        }

        auto itt = it;
        #ifdef ECHELON_DEBUG
        std::cout << "Current starting token "; stream_dump(std::cout, *itt); std::cout << "\n";
        #endif

        int matchCount = 0;
        for (auto element = (*g) -> getElements() -> begin(); element != (*g) -> getElements() -> end(); element++) {
          EnhancedToken *enhancedToken = new EnhancedToken(*itt);

          #ifdef ECHELON_DEBUG
          std::cout << "Matches: "; stream_dump(std::cout, enhancedToken); std::cout << " ? ";
          #endif

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
          else if ((*element) -> isUseNestedPatterns()) {
            #ifdef ECHELON_DEBUG
            std::cout << "Nested pattern.\n";
            #endif

            ParserInternalInput subInput;
            std::list<Token*> subList(itt, tokens.end());
            subInput.setTokens(&subList);
            subInput.setNestedPatterns((*element) -> getNestedPatterns());
            subInput.setUseNestedPatterns(true);

            auto subOutput = _parse(subInput);

            nestedAstNodes.push(subOutput.getAstNode());

            std::advance(itt, subOutput.getTokensConsumedCount());
            matchCount++;
          }
          else if ((*element) -> getMatcher() -> matches(enhancedToken)) {
            // The pattern matches directly using a matcher.

            #ifdef ECHELON_DEBUG
            std::cout << "Yes\n" << std::endl;
            #endif
            matchCount++;
            itt++;

            if (itt == tokens.end()) {
              #ifdef ECHELON_DEBUG
              std::cout << "Ran out of tokens.\n";
              #endif
              break;
            }
          }
          else {
            // This patten element can't be processed or matched.
            #ifdef ECHELON_DEBUG
            std::cout << "No\n";
            #endif
            break;
          }
        }

        bool completeGroupMatch = matchCount == (*g) -> getElements() -> size();
        #ifdef ECHELON_DEBUG
        std::cout << "Complete group match? " << EchelonLookup::toString(completeGroupMatch) << "\n";
        #endif

        if (completeGroupMatch) {
          int groupMatchCount = patternMatchInfo -> increment(std::distance((*p) -> getGroups() -> begin(), g));

          // Go again if we're under the uppper bound or are allowed unlimited matches.
          if (groupMatchCount < (*g) -> getRepeatUpperBound() || (*g) -> getRepeatUpperBound() == -1) {
            #ifdef ECHELON_DEBUG
            std::cout << "Match this group again.\n";
            #endif
            g--; // repeat this group.
          }
          else {
            #ifdef ECHELON_DEBUG
            std::cout << "Finished with this group. Move on.\n";
            #endif
            // matched but no need to repeat, therefore we just let the loop continue;
          }
        }
        else {
          // No match.

          int groupMatchCount = patternMatchInfo -> getGroupMatchCount(std::distance((*p) -> getGroups() -> begin(), g));

          if (groupMatchCount >= (*g) -> getRepeatLowerBound()) {
            // We've actually matched enough to allow the match even though this one failed.
            #ifdef ECHELON_DEBUG
            std::cout << "No group match, allowing anyway.\n";
            #endif

            // However, we need to reset itt to give back the tokens we've used so far.
            itt = it;
          }
          else {
            patternMatches = false;
            break;
          }
        }

        // This group matches so we want to consume the tokens matched by this group.
        #ifdef ECHELON_DEBUG
        std::cout << "Consume " << std::distance(it, itt) << " tokens." << std::endl;
        #endif
        // need to be more careful than this?
        std::advance(it, std::distance(it, itt));
      }

      if (patternMatches) {
        #ifdef ECHELON_DEBUG
        std::cout << "Pattern matches." << std::endl;
        #endif
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
        td -> setNestedAstNodes(&nestedAstNodes);
        td -> setPatternMatchInfo(patternMatchInfo);
        td -> setTokens(matchedTokens);

        auto transformer = AstTransformLookup::getInstance() -> getAstTransform((*p) -> getId());

        auto frag = transformer -> transform(td);
        #ifdef ECHELON_DEBUG
        std::cout << "frag:\n"; stream_dump(std::cout, frag); std::cout << "\n";
        #endif

        astConstructionManager.pushFragment(frag);

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
      else if (parserInternalInput.isUseNestedPatterns()) {
        std::cout << "Failed but we were using nested patterns so never mind?\n";
        break;
      }
      else {
        #ifdef ECHELON_DEBUG
        std::cerr << "ERROR, unhandled token. "; stream_dump(std::cout, *i); std::cout << "\n" ;
        throw std::runtime_error("Unhandled token.");
        #endif
      }
    }
  }

  #ifdef ECHELON_DEBUG
  std::cout << "built result:\n"; stream_dump(std::cout, astConstructionManager.getRoot()); std::cout << std::endl;
  #endif
  std::cout << "done dump." << std::endl;
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
