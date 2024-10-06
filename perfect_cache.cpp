#include "perfect.h"

int main ()
{
    perf_cache::perf_cache_t<int> cache(std::cin);

    int hits = cache.count_cache_hits();

    std::cout << "hits number = " << hits << std::endl;
}
