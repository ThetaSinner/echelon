#ifndef ECHELON_LOOKUP_HPP_INCLUDED
#define ECHELON_LOOKUP_HPP_INCLUDED

#include <set>
#include <string>

class EchelonLookup {
private:
  static EchelonLookup *self;

  std::set<std::string> dataTypeKeywordSet;
  std::set<std::string> keywordSet;

  EchelonLookup() {}

  EchelonLookup(const EchelonLookup &_) {}

  void operator=(const EchelonLookup &_) {}

public:
  static EchelonLookup *getInstance();

  template<typename T>
  static std::string toString(T t);

  void addDataTypeKeyword(std::string dataTypeKeyword);

  bool isDataTypeKeyword(std::string str);

  void addKeyword(std::string keyword);

  bool isKeyword(std::string str);
};

#endif
