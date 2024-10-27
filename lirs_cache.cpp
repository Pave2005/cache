#include "lirs.h"
#include "utils.h"

int main ()
try
{
    int cache_size = 0;
    int data_size  = 0;

    std::cin >> cache_size >> data_size;
    lirs_cache::cache<int> cache(cache_size, utils::slow_get_page<int>);

    int hits = utils::count_cache_hits<int>(cache, data_size, std::cin);

    std::cout << "hits number = " << hits << std::endl;
}
catch (std::exception& expt)
{
    std::cout << expt.what() << std::endl;
    exit (1);
}
