#include <echelon/util/iterator-util.hpp>

bool safe_advance(std::string::iterator &it, int n, std::string &s) {
  while (n) {
    if (it != s.end()) {
      it++;
    } else {
      return false;
    }
    n--;
  }

  return true;
}
