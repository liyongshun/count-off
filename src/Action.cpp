#include "Action.h"

std::string shout(int, const std::string& words)
{
    return words;
}

std::string nop(int n, const std::string&)
{
    return std::to_string(n);
}
