#ifndef KEYWORD_ENUM_HPP_INCLUDED
#define KEYWORD_ENUM_HPP_INCLUDED

enum class Keyword {
  Package,
  Enum,
  For,
  Until,
  While,
  Each,
  Module,
  If,
  Else,

  Integer,
  String,
  True,
  False,
  Behaviour,
  Function,
  In,

  #ifdef ECHELON_DEBUG
  First = Package,
  Last = In,
  #endif
};

#endif
