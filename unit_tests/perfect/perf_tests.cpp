#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "perfect.h"
#include "test_funcs.h"

TEST(test1, cache2_data13)
{
	test_funcs::run_test("/test1/cache2_data13", true);
}

TEST(test2, cache10_data100)
{
	test_funcs::run_test("/test2/cache10_data100", true);
}

TEST(test3, cache2_data5)
{
	test_funcs::run_test("/test3/cache2_data5", true);
}

TEST(test4, cache20_data10000)
{
	test_funcs::run_test("/test4/cache20_data10000", true);
}

#ifdef EXTRA

TEST(extra_test, cache10_data10000000)
{
	test_funcs::run_test("/extra_test/cache10_data10000000", true);
}

#endif
