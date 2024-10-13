#include "lirs.h"

int main ()
try
{
    int hits = lirs_cache::count_cache_hits<int>(std::cin);

    std::cout << "hits number = " << hits << std::endl;
}
catch (std::exception& expt)
{
    std::cout << expt.what() << std::endl;
    exit (1);
}
