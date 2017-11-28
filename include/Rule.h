#ifndef INCL_DCM_H9031BF21_2C04_4AA1_BDBD_9C4271BCDD08
#define INCL_DCM_H9031BF21_2C04_4AA1_BDBD_9C4271BCDD08

#include <string>

struct RuleMap
{
    int n;
    std::string words;
};

typedef bool (*Predicate)(int, int);
typedef std::string (*Action)(int, const std::string& );

template<Predicate predicate, Action action>
std::string rule(int n, const RuleMap& map = {})
{
    if(predicate(n, map.n))
    {
        return action(n, map.words);
    }

    return std::string("");
}

template<typename Head>
Head allof(Head head)
{
    return head;
}
template<typename Head, typename... Tail>
Head allof(Head head, Tail... tail)
{
    return head + allof<Head>(tail...);
}

template<typename Head>
Head anyof(Head head)
{
    if(!head.empty()) return head;
    return std::string("");
}

template<typename Head, typename... Tail>
Head anyof(Head head, Tail... tail)
{
    if(!head.empty()) return head;
    return anyof<Head>(tail...);
}

#endif
