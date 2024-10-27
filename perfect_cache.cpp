#include "perfect.h"
#include "utils.h"

int main ()
try
{
    int cache_size = 0;
    int data_size  = 0;

    std::vector<int> requests;

    std::cin >> cache_size >> data_size;

    requests.reserve(data_size);
    for (int i = 0; i < data_size; i++)
    {
        int page = 0;
        std::cin >> page;
        requests.push_back(page);
    }

    perf_cache::perf_cache_t<int> cache(cache_size, utils::slow_get_page<int>, requests);

    int hits = utils::count_cache_hits<int>(cache, requests, std::cin);

    std::cout << "hits number = " << hits << std::endl;
}
catch (std::exception& expt)
{
    std::cout << expt.what() << std::endl;
    exit (1);
}
