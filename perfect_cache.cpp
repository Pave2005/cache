#include "perfect.h"

int main ()
{
    perf_cache::cache_interface<int> cache;

    int hits = cache.count_cache_hits();

    std::cout << "hits number = " << hits << std::endl;
}
