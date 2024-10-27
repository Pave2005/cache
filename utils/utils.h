#pragma once

#include "lirs.h"
#include "perfect.h"

namespace utils
{
    template <typename T>
    T slow_get_page (T key)
    {
        return key;
    }

    template <typename T>
    int count_cache_hits (lirs_cache::cache<T>& cache, int data_size, std::istream& stream)
    {
        int hits = 0;

        for (int i = 0; i < data_size; i++)
        {
            T elem = 0;
            stream >> elem;
            hits += cache.get_block(elem);
        }

        return hits;
    }

    template <typename T>
    int count_cache_hits (perf_cache::perf_cache_t<T>& cache, std::vector<T>& requests, std::istream& stream)
    {
        int hits = 0;

        for (auto page : requests)
        {
            hits += cache.get_block(page);
        }

        return hits;
    }
}
