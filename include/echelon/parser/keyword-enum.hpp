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

  #ifdef ECHELON_DEBUG
  First = Package,
  Last = String,
  #endif
};

#endif
