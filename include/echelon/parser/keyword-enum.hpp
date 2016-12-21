#ifndef KEYWORD_ENUM_HPP_INCLUDED
#define KEYWORD_ENUM_HPP_INCLUDED

enum class Keyword {
  Package,
  For,
  Module,
  If,
  Else,

  Integer,
  String,
  True,
  False,

  #ifdef ECHELON_DEBUG
  First = Package,
  Last = False,
  #endif
};

#endif
