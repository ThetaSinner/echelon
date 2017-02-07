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
  Decimal,
  True,
  False,
  Behaviour,
  Function,
  In,
  Type,
  Public,

  /*
   * These allow the enum to be iterated.
   * Would ideally prefer this only to be present in debug but it's needed in the stage 2 data.
   */
  First = Package,
  Last = Public,
};

#endif
