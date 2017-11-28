#ifndef INCL_DCM_HC8D37058_8EF1_4870_8EBB_F9CA1BB3FB24
#define INCL_DCM_HC8D37058_8EF1_4870_8EBB_F9CA1BB3FB24

#include <string>
#include <initializer_list>
#include <vector>
#include "Rule.h"

struct CountOffGame
{
    CountOffGame(const std::initializer_list<RuleMap>&);
    std::string countOff(int n) const;

private:
    std::vector<RuleMap> rules;
};

#endif
