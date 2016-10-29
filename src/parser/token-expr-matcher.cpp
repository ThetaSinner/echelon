#include <echelon/parser/token-expr-matcher.hpp>

void TokenExprMatcher::matchTerminate(MatchData* matchData, TokenExpr* expr, std::string str, int pos) {
  int terminate_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    // Allow the matching to terminate if we reach the end of a line.
    if (expr -> isTerminateOnEol() && encodingQuery.isEol(str.at(pos + k))) {
      matchData -> setTerminate(true);
      // We don't want to extract the eol symbol, so exit immediately.
      return;
    }

    // Keep track of how many characters have been matched in the termination
    // sequence, exit as soon as a character doesn't meed the match condition.
    if (expr -> isTerminateMatch(str.at(pos + k), k)) {
      terminate_len++;
    }
    else {
      break;
    }
  }

  if (expr -> isCompleteTerminate(terminate_len)) {
    matchData -> setTerminateLen(terminate_len);
  }
  else {
    // This is a problem and we need to stop parsing. At this point we've
    // successfully matched a trigger sequence and possibly some content too,
    // but we've hit an incomplete terminate sequence. e.g. "/**"
    // Though I suppose you could just reject this match it's more likely this
    // ought to be reported. It can actually be reported later using a
    // combination of the matchData and expr objects.
    return;
  }
}

void TokenExprMatcher::matchContent(MatchData* matchData, TokenExpr* expr, std::string str, int pos) {
  int content_len = 0;
  for (int i = 0; pos + i < str.size(); i++) {
    // Check if this symbol is a termination symbol (or start of a termiantion
    // sequence).
    matchTerminate(matchData, expr, str, pos + content_len);
    if (matchData -> isTerminate()) {
      return;
    }

    // Not terminating yet, so check if this character meets the content
    // criteria otherwise stop matching.
    if (expr -> isContentMatch(str.at(pos + i), i)) {
      content_len++;
    }
    else {
      break;
    }

    // Need to update each loop because the function may exit before the loop does.
    matchData -> setContentLen(content_len);
  }

  // Nothing to return, all the data we need is in the matchData object.
}

void TokenExprMatcher::matchTrigger(MatchData* matchData, TokenExpr* expr, std::string str, int pos) {


  int trigger_len = 0;
  for (int k = 0; pos + k < str.size(); k++) {
    // Keep track of how many characters match the tigger condition, stop as
    // soon as a character doesn't match.
    if (expr -> isTriggerMatch(str.at(pos + k), k)) {
      trigger_len++;
    }
    else {
      break;
    }
  }

  // Check if the trigger condition has been fully satisfied.
  if (expr -> isCompleteTrigger(trigger_len)) {
    // save the actual number of tigger characters matched.
    matchData -> setTriggerLen(trigger_len);

    // try to match content now.
    matchContent(matchData, expr, str, pos + trigger_len);
  }
}

MatchData* TokenExprMatcher::match(TokenExpr* expr, std::string str, int pos) {
  // create a new match data for this match.
  MatchData* matchData = new MatchData();
  matchTrigger(matchData, expr, str, pos);

  // Matching is done, whether is has been successful or not can be determinted
  // from the matchData object.
  return matchData;
}
