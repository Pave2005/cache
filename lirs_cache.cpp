#include "lirs.h"

int main ()
{
    lirs_cache::cache<int> cache(std::cin);

    int hits = cache.count_cache_hits();

    std::cout << "hits number = " << hits << std::endl;
}
