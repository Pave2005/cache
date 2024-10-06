#ifndef PERFECT_H
#define PERFECT_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include <istream>

namespace perf_cache
{
    template <typename T>
    class perf_cache_t
    {
    private:
        using ListIt = typename std::list<T>::iterator;
        using VecIt  = typename std::vector<T>::iterator;
        using ItDiff = typename std::vector<T>::iterator::difference_type;

        std::istream& stream;

        int cache_size;
        int data_size;
        int request_counter = 0;

        std::list<T> cache;
        std::vector<T> requests;

        std::unordered_map<T, ListIt> cache_storage;
        std::unordered_map<T, int> requests_storage;

        void replace_farthest_elem (const T& key)
        {
            ItDiff farthest_dist = -1;
            ListIt candidate_to_rm = cache.begin();

            for (ListIt cache_elem = cache.begin(); cache_elem != cache.end(); cache_elem++)
            {
                VecIt cached_request = std::find(requests.begin() + request_counter, requests.end(), *cache_elem);

                if (cached_request != requests.end())
                {
                    ItDiff distance = std::distance(requests.begin(), cached_request);

                    if (distance > farthest_dist)
                    {
                        farthest_dist = distance;
                        candidate_to_rm = cache_elem;
                    }
                }
                else
                {
                    candidate_to_rm = cache_elem;
                    break;
                }
            }

            cache_storage.erase(*candidate_to_rm);
            cache.erase(candidate_to_rm);

            cache.push_front(key);
            cache_storage.emplace(key, cache.begin());
        }

    public:
        perf_cache_t(std::istream& stream) : stream(stream)
        {
            stream >> cache_size >> data_size;

            requests.reserve(data_size);

            for (int i = 0; i < data_size; i++)
            {
                T page = 0;
                stream >> page;
                requests.push_back(page);

                if (requests_storage.count(page))
                {
                    requests_storage[page] ++;
                }
                else
                {
                    requests_storage.insert({page, 1});
                }
            }
        }

        bool get_block (T key)
        {
            request_counter++;
            requests_storage[key] --;

            if (cache_storage.count(key)) return true;
            else if (requests_storage[key] == 0) return false;

            if (cache.size() < cache_size)
            {
                cache.push_front(key);
                cache_storage.emplace(key, cache.begin());

                return false;
            }

            replace_farthest_elem (key);

            return false;
        }

        int count_cache_hits ()
        {
            int hits = 0;

            for (int i = 0; i < data_size; i++)
            {
                hits = get_block(requests[i]) ? hits + 1 : hits;
            }

            return hits;
        }
    };
}

#endif
