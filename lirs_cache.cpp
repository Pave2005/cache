#include "lirs.h"

int main ()
{
    lirs_cache::cache_interface<int> cache("../data.txt");

    int hits = cache.count_cache_hits();

    std::cout << "hits number = " << hits << std::endl;
}
