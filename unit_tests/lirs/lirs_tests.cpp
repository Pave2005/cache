#include <fstream>
#include <string>

#include <gtest/gtest.h>

#include "lirs.h"
#include "test_funcs.h"

TEST(test1, cache2_data13)
{
	test_funcs::run_test("/test1/cache2_data13", false);
}

TEST(test2, cache10_data101)
{
	test_funcs::run_test("/test2/cache10_data101", false);
}

TEST(test3, cache2_data5)
{
	test_funcs::run_test("/test3/cache2_data5", false);
}

TEST(test4, cache20_data10000)
{
	test_funcs::run_test("/test4/cache20_data10000", false);
}

#ifdef EXTRA

TEST(extra_test, cache10_data10000000)
{
	test_funcs::run_test("/extra_test/cache10_data10000000", false);
}

#endif
