#include "perfect.h"

int main ()
{
    int hits = perf_cache::count_cache_hits<int>(std::cin);

    std::cout << "hits number = " << hits << std::endl;
}
