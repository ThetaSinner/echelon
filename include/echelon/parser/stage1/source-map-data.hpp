#ifndef SOURCE_MAP_DATA_HPP_INCLUDED
#define SOURCE_MAP_DATA_HPP_INCLUDED

class SourceMapData {
  long lineNumber = 0;
  long characterNumber = 0;

public:
  void addToLineNumber(long distance) {
    lineNumber += distance;
  }

  void incrementLineNumber() {
    addToLineNumber(1);
  }

  long getLineNumber() {
    return lineNumber;
  }

  void addToCharacterNumber(long distance) {
    characterNumber += distance;
  }

  long getCharacterNumber() {
    return characterNumber;
  }
};

#endif
