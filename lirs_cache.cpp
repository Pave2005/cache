#include "lirs.h"

int main ()
{
    lirs_cache_t<int> lirs_cache{10, 1, 11};

    int misses_count = lirs_cache.count_cache_misses ("data.txt");

    std::cout<<"misses number = "<<misses_count<<"\n";
}
