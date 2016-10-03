#ifndef ELM_LIBPARSE_P_INCLUDED
#define ELM_LIBPARSE_P_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <stdexcept>
#include <map>

class Parser
{
public:
  std::vector<std::string> parse(std::string in);
};

#endif
