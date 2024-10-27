cmake_minimum_required(VERSION 3.14)

project(cache)

enable_testing()
add_subdirectory(unit_tests/lirs)
add_subdirectory(unit_tests/perfect)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

option(BUILD_LIRS "Build the lirs executable" ON)
option(BUILD_PERFECT "Build the perfect executable" ON)
option(ENABLE_EXTRA "Big data extra tests for lirs and perfect" OFF)

# ----- LIRS executable -----
if(BUILD_LIRS)
	add_executable(lirs ./lirs_cache.cpp)
	target_include_directories(lirs PRIVATE
		                       ${CMAKE_CURRENT_SOURCE_DIR}/include
							   ${CMAKE_CURRENT_SOURCE_DIR}/utils)

	target_compile_options(lirs PRIVATE -O2)
endif()

# ----- PERFECT executable -----
if(BUILD_PERFECT)
	add_executable(perfect ./perfect_cache.cpp)
	target_include_directories(perfect PRIVATE
		                       ${CMAKE_CURRENT_SOURCE_DIR}/include
							   ${CMAKE_CURRENT_SOURCE_DIR}/utils)

	target_compile_options(perfect PRIVATE -O2)
endif()
