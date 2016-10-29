#ifndef TOKEN_EXPR_MATCHER_HPP
#define TOKEN_EXPR_MATCHER_HPP

#include <echelon/parser/encoding-query.hpp>
#include <echelon/parser/match-data.hpp>
#include <echelon/parser/token-expr.hpp>

class TokenExprMatcher {
private:
  EncodingQuery encodingQuery;

  void matchTerminate(MatchData* matchData, TokenExpr* expr, std::string str, int pos);
  void matchContent(MatchData* matchData, TokenExpr* expr, std::string str, int pos);
  void matchTrigger(MatchData* matchData, TokenExpr* expr, std::string str, int pos);
public:
  MatchData* match(TokenExpr* expr, std::string str, int pos);
};

#endif
