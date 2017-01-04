#ifndef KEYWORD_ENUM_HPP_INCLUDED
#define KEYWORD_ENUM_HPP_INCLUDED

enum class Keyword {
  Package,
  Enum,
  For,
  Until,
  While,
  Module,
  If,
  Else,

  Integer,
  String,
  True,
  False,
  Behaviour,
  Function,

  #ifdef ECHELON_DEBUG
  First = Package,
  Last = Function,
  #endif
};

#endif
