#include "parse-data.hpp"

ParseData::ParseData(std::string parse_str, int parse_pos) {
  this->parse_str = parse_str;
  this->parse_pos = parse_pos;
  this->parse_len = parse_str.size();
}

std::string ParseData::getParseStr() {
  return parse_str;
}

int ParseData::getParsePos() {
  return parse_pos;
}

void ParseData::setParsePos(int parse_pos) {
  this->parse_pos = parse_pos;
}

int ParseData::getParseLen() {
  return parse_len;
}

bool ParseData::hasNext() {
  return getParsePos() + 1 < getParseLen();
}

char ParseData::getCurrent() {
  return parse_str.at(parse_pos);
}

char ParseData::getNext() {
  return parse_str.at(parse_pos + 1);
}
