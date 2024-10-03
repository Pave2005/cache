#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <fstream>

#include <gtest/gtest.h>

#include "lirs.h"
#include "perfect.h"
#include "file.h"

namespace test_funcs
{
    void set_requests (const std::string& filename, std::vector<int>& requests,
                       std::unordered_map<int, int>& requests_storage, int& cache_size, int& data_size)
    {
        file::file_t<int> file(filename);
        file.get_file (requests, cache_size, data_size);

        for (int page : requests)
        {
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

    int count_cache_hits (perf_cache::perf_cache_t<int>& cache, const std::vector<int>& requests, int data_size)
    {
        int hits = 0;

        for (int i = 0; i < data_size; i++)
        {
            hits += cache.get_block(requests[i]);
        }

        return hits;
    }

    int count_cache_hits (const std::string& filename)
    {
        int cache_size = 0;
        int data_size = 0;
        file::file_t<int> file(filename);
        std::vector<int> buff;
        file.get_file (buff, cache_size, data_size);

        int hits = 0;
        lirs_cache::cache<int> lirs_cache(cache_size);

        for (int elem : buff)
        {
            hits += lirs_cache.get_block(elem);
        }

        return hits;
    }

	std::string get_result (const std::string& filename, bool perf)
    {
        int hits  = 0;
        if (perf)
        {
            int cache_size = 0;
            int data_size = 0;
            std::vector<int> requests;
            std::unordered_map<int, int> requests_storage;

            set_requests(filename, requests, requests_storage, cache_size, data_size);

            perf_cache::perf_cache_t<int> cache(cache_size, requests, requests_storage);

            hits = count_cache_hits (cache, requests, data_size);
        }
        else
        {
            hits = count_cache_hits (filename);
        }

        return std::to_string(hits);
    }

    std::string get_answer(const std::string& filename)
    {
        std::ifstream answer_file(filename);

        std::string answer;
        answer_file >> answer;

        return answer;
    }

	void run_test (const std::string& test_name, bool perf)
	{
		std::string test_directory = perf ? "/perf_tests/" : "/lirs_tests/";

		std::string test_path = std::string(TEST_DATA_DIR) + test_directory + test_name;
		std::string result = get_result(test_path + ".dat", perf);
		std::string answer = get_answer(test_path + ".ans");

		EXPECT_EQ(result, answer);
	}

}

#endif
