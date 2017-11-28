#include "Predicate.h"

bool times(int n, int times)
{
    return n % times == 0;
}

bool contains(int n, int contained)
{
    int units = 0;
    while(n > 0)
    {
        units = n % 10;
        n = n /10;
        if(units == contained) return true;
    }

    return false;
}

bool always_true(int,int)
{
    return true;
}
