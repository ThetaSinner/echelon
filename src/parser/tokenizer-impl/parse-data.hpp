#ifndef PARSE_DATA_INCLUDED
#define PARSE_DATA_INCLUDED

#include <string>

class ParseData {
private:
  std::string parse_str;
  int parse_pos = 0;
  int parse_len = 0;
public:
  ParseData(std::string parse_str, int parse_pos);

  std::string getParseStr();

  int getParsePos();
  void setParsePos(int parse_pos);

  int getParseLen();

  bool hasNext();

  char getCurrent();

  char getNext();
};

#endif
