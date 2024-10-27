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
#include <istream>
#include <stdexcept>

namespace lirs_cache
{
    template <typename T>
    class cache
    {
    private:
        enum class status
        {
            lir,
            res_hir,
            non_res_hir,
        };

        enum class loc_status
        {
            out = 0,
            in  = 1,
        };

        struct node
        {
            status st;
            loc_status located_in_history = loc_status::out;
            loc_status located_in_hirs    = loc_status::out;
            std::list<int>::iterator pos_history;
            std::list<int>::iterator pos_hirs;
        };

        int cache_size;

        T (*slow_get_page)(T key);

        int lirsCapacity;
        int hirsCapacity;
        int lirs_size;
        std::list<T> cache_history;
        std::list<T> hirs_cache;
        std::unordered_map<T, node> cache_storage;

        void raise_elem_in_history (const T& key)
        {
            auto& history_pos = cache_storage[key].pos_history;

            cache_history.erase(history_pos);
            cache_history.push_front(key);
            history_pos = cache_history.begin();
        }

        void raise_elem_in_hirs (const T& key)
        {
            auto& hirs_pos = cache_storage[key].pos_hirs;

            hirs_cache.erase(hirs_pos);
            hirs_cache.push_front(key);
            hirs_pos = hirs_cache.begin();
        }

        void renew_hir_cache (const T& key)
        {
            auto& storage_elem = cache_storage[key];

            if (hirs_cache.size() < hirsCapacity)
            {
                hirs_cache.push_front(key);
                storage_elem.pos_hirs = hirs_cache.begin();
                storage_elem.located_in_hirs = loc_status::in;
                return;
            }

            int first_hir = hirs_cache.back();

            auto& storage_first_hir = cache_storage[first_hir];

            loc_status is_in_history = storage_first_hir.located_in_history;

            if (is_in_history == loc_status::in)
            {
                storage_first_hir.st = status::non_res_hir;
                storage_first_hir.located_in_hirs = loc_status::out;
            }
            else
            {
                cache_storage.erase(first_hir);
            }

            hirs_cache.pop_back();
            hirs_cache.push_front(key);
            storage_elem.pos_hirs = hirs_cache.begin();
            storage_elem.located_in_hirs = loc_status::in;
        }

        void cut ()
        {
            while (true)
            {
                int first_history_elem = cache_history.back();
                auto& storage_first_hist_elem = cache_storage[first_history_elem];
                status stat = storage_first_hist_elem.st;

                if (stat != status::lir)
                {
                    int is_res = (stat == status::res_hir);
                    cache_history.pop_back();
                    if (is_res)
                    {
                        storage_first_hist_elem.located_in_history = loc_status::out;
                    }
                    else
                    {
                        cache_storage.erase(first_history_elem);
                    }
                }
                else if (lirs_size > lirsCapacity)
                {
                    renew_hir_cache(first_history_elem);
                    cache_history.pop_back();
                    storage_first_hist_elem.located_in_history = loc_status::out;
                    storage_first_hist_elem.st = status::res_hir;
                    lirs_size --;
                }
                else
                {
                    break;
                }
            }
        }

        void process_lir (const T& key)
        {
            raise_elem_in_history(key);
            cut();
        }

        void process_res_hir (const T& key)
        {
            auto& storage_elem = cache_storage[key];

            loc_status is_in_history = storage_elem.located_in_history;

            if (is_in_history == loc_status::in)
            {
                raise_elem_in_history(key);
                storage_elem.st = status::lir;
                lirs_size ++;
                if (storage_elem.located_in_hirs == loc_status::in)
                {
                    hirs_cache.erase(storage_elem.pos_hirs);
                    storage_elem.located_in_hirs = loc_status::out;
                }

                cut();
            }
            else
            {
                raise_elem_in_hirs(key);
                cache_history.push_front(key);
                storage_elem.pos_history = cache_history.begin();
                storage_elem.located_in_history = loc_status::in;
            }
        }

        void process_new (const T& key)
        {
            T elem = slow_get_page(key);

            cache_history.push_front(elem);
            cache_storage.insert({elem, {status::res_hir, loc_status::in, loc_status::out, cache_history.begin()}});

            renew_hir_cache(key);
        }

        void process_non_res_hir (const T& key)
        {
            raise_elem_in_history(key);
            cache_storage[key].st = status::lir;
            lirs_size++;

            cut();
        }

    public:
        cache(int cache_size, T (*slow_get_page)(T key)) : cache_size(cache_size), slow_get_page(slow_get_page)
        {
            lirs_size = 0;

            if (cache_size > 2)
            {
                hirsCapacity = (int)(cache_size * 0.4) ;
                lirsCapacity = cache_size - hirsCapacity;
            }
            else if (cache_size == 2)
            {
                hirsCapacity = 1;
                lirsCapacity = 1;
            }
            else throw std::runtime_error("Incorrect cache size");
        }

        bool get_block (T key)
        {
            if (!(cache_storage.count(key)))
            {
                process_new (key);
                return false;
            }

            status stat = cache_storage[key].st;

            switch (stat)
            {
                case status::lir:
                    process_lir(key);
                    break;
                case status::res_hir:
                    process_res_hir(key);
                    break;
                case status::non_res_hir:
                    process_non_res_hir(key);
                    break;
            }

            return true;
        }
    };
}

#endif
