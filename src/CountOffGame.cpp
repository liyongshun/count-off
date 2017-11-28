#include "CountOffGame.h"
#include "Predicate.h"
#include "Action.h"

CountOffGame::CountOffGame(const std::initializer_list<RuleMap>& rules) : rules(rules)
{
}

std::string CountOffGame::countOff(int n) const
{
    auto r1_1 = rule<times, shout>(n, rules[0]);
    auto r1_2 = rule<times, shout>(n, rules[1]);
    auto r1_3 = rule<times, shout>(n, rules[2]);
    auto r1 = allof(r1_1, r1_2, r1_3);

    auto r2 = rule<contains, shout>(n, rules[0]);
    auto rd = rule<always_true, nop>(n);

    return anyof(r2, r1, rd);
}
