#ifndef LIRS_H
#define LIRS_H

#include <iostream>
#include <unordered_map>
#include <list>
#include <stack>
#include <new>
#include <utility>
#include <fstream>
#include <string>
#include <ios>
#include <vector>

enum status
{
    lir = 0,
    r_hir = 1,
    n_hir = 2,
};

enum location
{
    in = 1,
    out = 0,
};

template <typename T>
struct block_t
{
    T key;
    int status;
    int location;
};

template <typename T>
class file_t
{
public:
    int get_file (const char* filename, std::vector<T>& buff)
    {
        std::ifstream file(filename);
        if (!file) return 1; // не нашел файл

        T tmp = 0;
        while (file >> tmp) buff.push_back(tmp);

        file.close();
        return 0;
    }
};

template <typename T>
class lirs_cache_t
{
private:
    int lirCapacity;
    int hirCapacity;
    int size;

    std::list<block_t<T>*> cache_history;
    std::list<block_t<T>*> cache_hir;

    std::unordered_map<T, block_t<T>*> cache_storage;

    void cut ()
    {
        block_t<T>* front = cache_history.front();
        while (front->status != lir)
        {
            front->location = out;
            cache_history.pop_front();

            if (front->status == n_hir)
            {
                cache_storage.erase(front->key);
                delete front;
            }
            front = cache_history.front();
        }
    }

    void renew_cache_hir (block_t<T>* block_ptr)
    {
        cache_hir.push_back(block_ptr);

        if (cache_hir.size() > hirCapacity)  // в списке не меняем статус хира
        {
            cache_hir.front()->status = n_hir;
            cache_hir.pop_front();
        }
    }

    block_t<T>* create_elem (T key, int status, int location)
    {
        block_t<T>* ptr = new block_t<T>;
        ptr->key = key;
        ptr->status = status;
        ptr->location = location;
        return ptr;
    }

public:
    lirs_cache_t(int lirCapacity, int hirCapacity, int size)
    {
        this->lirCapacity = lirCapacity;
        this->hirCapacity = hirCapacity;
        this->size = size;
    }

    int get_block (T key)
    {
        if (cache_storage.size() < lirCapacity)
        {
            if (!cache_storage.count(key)) // почему то выдает ошибку, если ставлю contains
            {
                block_t<T>* elem = create_elem (key, lir, in);

                cache_history.push_back(elem);
                cache_storage.insert({key, elem});
                return 1;
            }
            else // содержит ключ
            {
                block_t<T>* elem = cache_storage.at(key);
                cache_history.remove(elem);
                cache_history.push_back(elem);
                return 0;
            }
        }

        if (!cache_storage.count(key)) // так же выдает ошибку на contains
        {
            block_t<T>* elem = create_elem (key, r_hir, in);

            cache_storage.insert({key, elem});
            cache_history.push_back(elem);

            renew_cache_hir (elem);

            return 1;
        }
        else
        {
            block_t<T>* elem = cache_storage.at(key);
            int status = elem->status;  // не уверен

            if (status == lir)
            {
                cache_history.remove(elem);
                cache_history.push_back(elem);
                cut ();
            }
            else if (status == r_hir)
            {
                int location = elem->location;

                if (location == in)
                {
                    block_t<T>* list_front = cache_history.front();
                    while (list_front->key != key)
                    {
                        if (list_front->status == lir)
                        {
                            list_front->status = r_hir;
                            list_front->location = out;
                            renew_cache_hir (list_front);
                        }
                        else if (list_front->status == n_hir)
                        {
                            cache_storage.erase(list_front->key);
                        }

                        cache_history.pop_front();
                        list_front = cache_history.front();
                    }

                    block_t<T>* front = cache_history.front();
                    front->status = lir;

                    cache_history.pop_front();
                    cache_history.push_back(front);

                    cut ();
                }
                else
                {
                    cache_hir.remove(elem);
                    cache_hir.push_back(elem);

                    elem->location = in;
                    cache_history.push_back(elem);
                }
            }
            else if (status == n_hir)
            {
                cache_history.remove(elem);
                cache_history.push_back(elem);
                elem->status = lir;

                block_t<T>* front = cache_history.front();
                front->status = r_hir;
                front->location = out;
                renew_cache_hir (front);
                cache_history.pop_front();

                cut ();
            }
            return 0;
        }
        return 0;
    }

    int count_cache_misses (const char* filename)
    {
        file_t<T> file;
        std::vector<T> buff;
        file.get_file (filename, buff);

        // int buff[] = {1, 2, 3, 4, 5, 2, 5, 7, 4, 9, 1, 0, 4, 1, 6, 4, 1, 7, 8, 2, 5, 10, 20, 2, 5, 10, 2, 1, 5};
        // int size = 29; // пример

        int misses_count = 0;
        for (T buff_elem : buff)
        {
            //std::cout<<"elem = "<<buff_elem<<"\n";
            misses_count += get_block (buff_elem);
        }

        return misses_count;
    }
};

#endif
