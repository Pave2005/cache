cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
add_definitions(-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\")

find_package(GTest REQUIRED)

add_executable(lirs_tests
			   lirs_tests.cpp
			   main.cpp)

target_compile_options(lirs_tests PRIVATE -O2)

target_link_libraries(lirs_tests
					  GTest::GTest
					  GTest::Main)

target_include_directories(lirs_tests PRIVATE
	                       ${CMAKE_SOURCE_DIR}/unit_tests/funcs
	                       ${CMAKE_SOURCE_DIR}/include
						   ${CMAKE_SOURCE_DIR}/utils)

if(ENABLE_EXTRA)
    target_compile_definitions(lirs_tests PRIVATE EXTRA)
endif()

enable_testing()
add_test(NAME LirsTests COMMAND lirs_tests)
