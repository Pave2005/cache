#ifndef PERFECT_H
#define PERFECT_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <vector>
#include <algorithm>
#include <utility>
#include <istream>
#include <queue>

namespace perf_cache
{
    template <typename T>
    class perf_cache_t
    {
    private:
        using ListIt = typename std::list<T>::iterator;

        int cache_size;
        int request_counter = 0;

        std::list<T> cache;
        std::vector<T> requests;

        std::unordered_map<T, ListIt> cache_storage;
        std::unordered_map<T, std::queue<int>> requests_storage;

        void replace_farthest_elem (const T& key)
        {
            int farthest_dist = -1;
            ListIt candidate_to_rm = cache.begin();

            for (ListIt cache_elem = cache.begin(); cache_elem != cache.end(); cache_elem++)
            {
                auto& storage_elem = requests_storage[*cache_elem];

                if (storage_elem.empty())
                {
                    candidate_to_rm = cache_elem;
                    break;
                }

                int cached_request = storage_elem.front();
                int distance = cached_request - request_counter;

                if (distance > farthest_dist)
                {
                    farthest_dist = distance;
                    candidate_to_rm = cache_elem;
                }
            }

            cache_storage.erase(*candidate_to_rm);
            cache.erase(candidate_to_rm);

            cache.push_front(key);
            cache_storage.emplace(key, cache.begin());
        }

    public:
        perf_cache_t(int cache_size, std::vector<T> buff) : cache_size(cache_size), requests(buff)
        {
            if (cache_size <= 0) throw std::runtime_error("Incorrect cache size");

            for (int i = 0; i < requests.size(); i++)
            {
                auto page = requests[i];

                if (!requests_storage[page].empty())
                {
                    requests_storage[page].push(i);
                }
                else
                {
                    requests_storage.insert({page, {}});
                    requests_storage[page].push(i);
                }
            }
        }

        bool get_block (T key)
        {
            request_counter++;
            requests_storage[key].pop();

            if (cache_storage.count(key))           return true;
            else if (requests_storage[key].empty()) return false;

            if (cache.size() < cache_size)
            {
                cache.push_front(key);
                cache_storage.emplace(key, cache.begin());

                return false;
            }

            replace_farthest_elem(key);

            return false;
        }
    };

    template <typename T>
    int count_cache_hits (std::istream& stream)
    {
        int cache_size = 0;
        int data_size  = 0;

        std::vector<T> requests;

        stream >> cache_size >> data_size;

        requests.reserve(data_size);
        for (int i = 0; i < data_size; i++)
        {
            T page = 0;
            stream >> page;
            requests.push_back(page);
        }

        perf_cache_t<T> cache(cache_size, requests);

        int hits = 0;

        for (auto page : requests)
        {
            hits += cache.get_block(page);
        }

        return hits;
    }
}

#endif
