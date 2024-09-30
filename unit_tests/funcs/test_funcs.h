#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <string>
#include <fstream>

#include <gtest/gtest.h>

#include "lirs.h"
#include "perfect.h"

namespace test_funcs
{
	std::string get_result (const std::string& filename, bool perf)
    {
        int hits  = 0;
        if (perf)
        {
            perf_cache::cache_interface<int> cache(filename);
            hits = cache.count_cache_hits();
        }
        else
        {
            lirs_cache::cache_interface<int> cache(filename);
            hits = cache.count_cache_hits();
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
