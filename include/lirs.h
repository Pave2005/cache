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
            in = 1,
        };

        struct node
        {
            status st;
            loc_status located_in_history = loc_status::out;
            loc_status located_in_hirs = loc_status::out;
            std::list<int>::iterator pos_history;
            std::list<int>::iterator pos_hirs;
        };

        std::istream& stream;
        int cache_size;
        int data_size;

        int lirsCapacity;
        int hirsCapacity;
        int lirs_size;
        std::list<T> cache_history;
        std::list<T> hirs_cache;
        std::unordered_map<T, node> cache_storage;

        void raise_elem_in_history (const T& elem)
        {
            auto& history_pos = cache_storage[elem].pos_history;

            cache_history.erase(history_pos);
            cache_history.push_front(elem);
            history_pos = cache_history.begin();
        }

        void raise_elem_in_hirs (const T& elem)
        {
            auto& hirs_pos = cache_storage[elem].pos_hirs;

            hirs_cache.erase(hirs_pos);
            hirs_cache.push_front(elem);
            hirs_pos = hirs_cache.begin();
        }

        void renew_hir_cache (const T& elem)
        {
            if (hirs_cache.size() < hirsCapacity)
            {
                hirs_cache.push_front(elem);
                cache_storage[elem].pos_hirs = hirs_cache.begin();
                cache_storage[elem].located_in_hirs = loc_status::in;
                return;
            }

            int first_hir = hirs_cache.back();
            loc_status is_in_history = cache_storage[first_hir].located_in_history;

            if (is_in_history == loc_status::in)
            {
                cache_storage[first_hir].st = status::non_res_hir;
                cache_storage[first_hir].located_in_hirs = loc_status::out;
            }
            else
            {
                cache_storage.erase(first_hir);
            }

            hirs_cache.pop_back();
            hirs_cache.push_front(elem);
            cache_storage[elem].pos_hirs = hirs_cache.begin();
            cache_storage[elem].located_in_hirs = loc_status::in;
        }

        void cut ()
        {
            int first_history_elem = cache_history.back();
            status stat = cache_storage[first_history_elem].st;

            while (1)
            {
                if (stat != status::lir)
                {
                    int is_res = (stat == status::res_hir);
                    cache_history.pop_back();
                    if (is_res)
                    {
                        cache_storage[first_history_elem].located_in_history = loc_status::out;
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
                    cache_storage[first_history_elem].located_in_history = loc_status::out;
                    cache_storage[first_history_elem].st = status::res_hir;
                    lirs_size --;
                }
                else
                {
                    break;
                }

                first_history_elem = cache_history.back();
                stat = cache_storage[first_history_elem].st;
            }
        }

        void process_lir (const T& elem)
        {
            raise_elem_in_history (elem);
            cut ();
        }

        void process_res_hir (const T& elem)
        {
            loc_status is_in_history = cache_storage[elem].located_in_history;

            if (is_in_history == loc_status::in)
            {
                raise_elem_in_history (elem);
                cache_storage[elem].st = status::lir;
                lirs_size ++;
                if (cache_storage[elem].located_in_hirs == loc_status::in)
                {
                    hirs_cache.erase(cache_storage[elem].pos_hirs);
                    cache_storage[elem].located_in_hirs = loc_status::out;
                }

                cut ();
            }
            else
            {
                raise_elem_in_hirs (elem);
                cache_history.push_front(elem);
                cache_storage[elem].pos_history = cache_history.begin();
                cache_storage[elem].located_in_history = loc_status::in;
            }
        }

        void process_new (const T& elem)
        {
            cache_history.push_front(elem);
            cache_storage.insert({elem, {status::res_hir, loc_status::in, loc_status::out, cache_history.begin()}});

            renew_hir_cache (elem);
        }

        void process_non_res_hir (const T& elem)
        {
            raise_elem_in_history(elem);
            cache_storage[elem].st = status::lir;
            lirs_size++;

            cut ();
        }

    public:
        cache(std::istream& stream) : stream(stream)
        {
            stream >> cache_size >> data_size;

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
            else
            {
                throw "Incorrect cache size";
            }
        }

        bool get_block (T elem)
        {
            if (!(cache_storage.count(elem)))
            {
                process_new (elem);
                return false;
            }

            status stat = cache_storage[elem].st;

            if (stat == status::lir)
            {
                process_lir(elem);
            }
            else if (stat == status::res_hir)
            {
                process_res_hir(elem);
            }
            else
            {
                process_non_res_hir(elem);
            }

            return true;
        }

        int count_cache_hits ()
        {
            int hits = 0;
            try
            {
                for (int i = 0; i < data_size; i++)
                {
                    T elem = 0;
                    stream >> elem;
                    hits = get_block(elem) ? hits + 1 : hits;
                }
            }
            catch (const char* error_message)
            {
                std::cout << error_message << std::endl;
                exit (1);
            }

            return hits;
        }
    };
}

#endif
