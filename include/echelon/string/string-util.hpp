#ifndef STRING_UTIL_HPP_INCLUDED
#define STRING_UTIL_HPP_INCLUDED

#include <string>

class StringUtil {
public:
  static std::string extractSymbol(std::string sym, std::string& source);
};

#endif
