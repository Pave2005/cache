#ifndef PERFECT_H
#define PERFECT_H

#include <iostream>
#include <queue>
#include <list>
#include <unordered_map>
#include <utility>
#include <fstream>
#include <new>

namespace file
{
    template <typename T>
    class file_t
    {
    private:
        const char* filename;

    public:
        file_t(const char* filename)
        {
            this->filename = filename;
        }

        int get_file (std::vector<T>& buff)
        {
            std::ifstream file(filename);
            if (!file) return 1;

            T tmp = 0;
            while (file >> tmp) buff.push_back(tmp);

            file.close();
            return 0;
        }
    };
}

namespace perf_cache
{
    const int default_val = -1;
    enum LOCATION
    {
        in = 1,
        out = 0,
    };

    template <typename T>
    class cache
    {
    private:
        struct node
        {
            int location = out;
            int indx = default_val;
            typename std::list<T>::iterator position;
            std::list<int> positions;
        };

        int size;
        int capacity;
        std::list<T> cache_list;
        std::unordered_map<T, node> storage;

        void cache_insert (int key, int indx)
        {
            cache_list.push_front(key);
            storage[key].location = in;
            storage[key].indx = indx;
            storage[key].position = cache_list.begin();
        }

        T find_worst (int indx)  // разобраться в этом алгоритме
        {
            int worst = default_val;
            int interval = default_val;
            int max = default_val;

            for (T elem : cache_list)
            {
                auto curr = storage[elem].positions.begin();
                auto last = storage[elem].positions.end();

                while (*curr < indx)
                {
                    if (curr == last)
                    {
                        return elem;
                    }
                    curr++;
                }

                interval = *curr - indx; // indx = *(storage[elem].position)

                if (interval > max)
                {
                    max = interval;
                    worst = elem;
                }
            }

            return worst;
        }

        void cache_erase (int indx)
        {
            if (size < capacity)
            {
                size++;
                return;
            }

            T worst = find_worst (indx);

            cache_list.erase(storage[worst].position);
            storage[worst].location = out;
        }

    public:
        cache(int capacity)
        {
            size = 0;
            this->capacity = capacity;
        }

        ~cache()
        {
            storage.clear();;
            cache_list.clear();
        }

        void storage_insert (int indx, int key)
        {
            if (!storage.count(key))
                storage.insert({key, {}});

            storage[key].positions.push_back(indx);
        }

        void get_block (int indx, int* hits, std::vector<T>& data_buff)
        {
            int key = data_buff[indx];

            storage[key].positions.pop_front();

            if (storage[key].location)
            {
                (*hits)++;
                storage[key].indx = indx;
            }
            else
            {
                cache_insert (key, indx);
                cache_erase (indx);
            }
        }
    };

    template <typename T>
    class cache_interface
    {
    private:
        const char* filename;
        int cache_size;

    public:
        cache_interface(const char* filename, int cache_size)
        {
            this->filename = filename;
            this->cache_size = cache_size;
        }

        int count_cache_hits ()
        {
            int hits = 0;

            cache<T> perf_cache(cache_size);

            file::file_t<T> file(filename);
            std::vector<T> buff;
            file.get_file (buff);

            for (int i = 0; i < buff.size(); i++)
            {
                perf_cache.storage_insert(i, buff[i]);
            }

            for (int i = 0; i < buff.size(); i++)
            {
                perf_cache.get_block(i, &hits, buff);
            }

            return hits;
        }
    };
}
