#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <stack>
#include <new>
#include <utility>
#include <string>
#include <ios>
#include <vector>

#include "file.h"

namespace lirs_cache
{
    template <typename T>
    class cache
    {
    private:
        enum status
        {
            lir,
            res_hir,
            non_res_hir,
        };

        enum location_st
        {
            out = 0,
            in = 1,
        };

        struct node
        {
            status st;
            int located_in_history = out;
            int located_in_hirs = out;
            std::list<int>::iterator pos_history;
            std::list<int>::iterator pos_hirs;
        };

        int lirsCapacity;
        int hirsCapacity;
        int lirs_size;
        std::list<T> cache_history;
        std::list<T> hirs_cache;
        std::unordered_map<T, node> cache_storage;

        void raise_elem_in_history (T elem)
        {
            cache_history.erase(cache_storage[elem].pos_history);
            cache_history.push_front(elem);
            cache_storage[elem].pos_history = cache_history.begin();
        }

        void raise_elem_in_hirs (T elem)
        {
            hirs_cache.erase(cache_storage[elem].pos_hirs);
            hirs_cache.push_front(elem);
            cache_storage[elem].pos_hirs = hirs_cache.begin();
        }

        void renew_hir_cache (T elem)
        {
            if (hirs_cache.size() < hirsCapacity)
            {
                hirs_cache.push_front(elem);
                cache_storage[elem].pos_hirs = hirs_cache.begin();
                cache_storage[elem].located_in_hirs = in;
                return;
            }

            int first_hir = hirs_cache.back();
            int is_in_history = cache_storage[first_hir].located_in_history;

            if (is_in_history)
            {
                cache_storage[first_hir].st = non_res_hir;
                cache_storage[first_hir].located_in_hirs = out;
            }
            else
            {
                cache_storage.erase(first_hir);
            }

            hirs_cache.pop_back();
            hirs_cache.push_front(elem);
            cache_storage[elem].pos_hirs = hirs_cache.begin();
            cache_storage[elem].located_in_hirs = in;
        }

        void cut ()
        {
            int first_history_elem = cache_history.back();
            int status = cache_storage[first_history_elem].st;

            while (1)
            {
                if (status != lir)
                {
                    int is_res = (status == res_hir);
                    cache_history.pop_back();
                    if (is_res)
                    {
                        cache_storage[first_history_elem].located_in_history = out;
                    }
                    else
                    {
                        cache_storage.erase(first_history_elem);
                    }
                }
                else if (lirs_size > lirsCapacity)
                {
                    renew_hir_cache (first_history_elem);
                    cache_history.pop_back();
                    cache_storage[first_history_elem].located_in_history = out;
                    cache_storage[first_history_elem].st = res_hir;
                    lirs_size --;
                }
                else
                {
                    break;
                }

                first_history_elem = cache_history.back();
                status = cache_storage[first_history_elem].st;
            }
        }

        void process_lir (T elem)
        {
            raise_elem_in_history (elem);
            cut ();
        }

        void process_res_hir (T elem)
        {
            int is_in_history = cache_storage[elem].located_in_history;

            if (is_in_history)
            {
                raise_elem_in_history (elem);
                cache_storage[elem].st = lir;
                lirs_size ++;
                if (cache_storage[elem].located_in_hirs) // можно убрать
                {
                    hirs_cache.erase(cache_storage[elem].pos_hirs);
                    cache_storage[elem].located_in_hirs = out;
                }

                cut ();
            }
            else
            {
                raise_elem_in_hirs (elem);
                cache_history.push_front(elem);
                cache_storage[elem].pos_history = cache_history.begin();
                cache_storage[elem].located_in_history = in;
            }
        }

        void process_new (T elem)
        {
            cache_history.push_front(elem);
            cache_storage.insert({elem, {res_hir, in, out, cache_history.begin()}});

            renew_hir_cache (elem);
        }

        void process_non_res_hir (T elem)
        {
            raise_elem_in_history(elem);
            cache_storage[elem].st = lir;
            lirs_size++;

            cut ();
        }

    public:
        cache(int cache_len)
        {
            lirs_size = 0;

            if (cache_len > 2)
            {
                hirsCapacity = (int)(cache_len * 0.4) ;
                lirsCapacity = cache_len - hirsCapacity;
            }
            else if (cache_len < 2)
            {
                std::cout << "Incorrect cache size" << std::endl;
                exit(1);
            }
            else
            {
                hirsCapacity = 1;
                lirsCapacity = 1;
            }
        }

        ~cache()
        {
            cache_storage.clear();;
            hirs_cache.clear();
            cache_history.clear();
        }

        int get_block (int elem)
        {
            if (!(cache_storage.count(elem)))
            {
                process_new (elem);
                return 0;
            }

            int stat = cache_storage[elem].st;

            if (stat == lir)
            {
                process_lir(elem);
            }
            else if (stat == res_hir)
            {
                process_res_hir(elem);
            }
            else
            {
                process_non_res_hir(elem);
            }

            return 1;
        }
    };

    template <typename T>  // создать отдельное пространство имен
    class cache_interface
    {
    private:
        const std::string filename;
        int cache_size;
        int data_size;

    public:
        cache_interface(const std::string& filename) : filename(filename) {}

        int count_cache_hits ()
        {
            file::file_t<T> file(filename);
            std::vector<T> buff;
            file.get_file (buff, cache_size, data_size);

            //std::cout << cache_size << " " << data_size << std::endl;

            int hits = 0;
            cache<T> lirs_cache(cache_size);

            for (T elem : buff)
            {
                hits += lirs_cache.get_block(elem);
            }

            return hits;
        }
    };
}

#endif
