#include "perfect.h"

int main ()
{
    perf_cache::cache_interface<int> cache("data.txt", 10);

    int hits = cache.count_cache_hits();

    std::cout<<"hits number = "<<hits<<"\n";
}
