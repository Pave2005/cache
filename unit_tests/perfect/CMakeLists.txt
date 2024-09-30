cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(TEST_DATA_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../")
add_definitions(-DTEST_DATA_DIR=\"${TEST_DATA_DIR}\")

find_package(GTest REQUIRED)

set(COMPILE_OPTIONS
	-O2
	-fstack-protector
	-fPIE
	-fstrict-overflow
	-Wformat-security
	-Wformat=2
	-Wall
	-Wextra
	-Wnon-virtual-dtor
	-Woverloaded-virtual
	-Wcast-align
	-Wpointer-arith
	-Wunreachable-code
)

add_executable(perf_tests
			   perf_tests.cpp
			   main.cpp)

target_compile_options(perf_tests PRIVATE ${COMPILE_OPTIONS})

target_link_libraries(perf_tests
					  GTest::GTest
					  GTest::Main)

target_include_directories(perf_tests PRIVATE
	                       ${CMAKE_SOURCE_DIR}/unit_tests/funcs
	                       ${CMAKE_SOURCE_DIR}/include
)

if(ENABLE_EXTRA)
    target_compile_definitions(perf_tests PRIVATE EXTRA)
endif()

enable_testing()
add_test(NAME PerfectTests COMMAND perf_tests)
