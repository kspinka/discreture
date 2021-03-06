cmake_minimum_required(VERSION 3.0)

project(discreture)
enable_language(C CXX)

if (NOT CMAKE_BUILD_TYPE)
    message("CMAKE_BUILD_TYPE not specified, so setting to Release")
    set(CMAKE_BUILD_TYPE "Release")
endif()

option(BUILD_EXAMPLES "Build examples" ON)
option(BUILD_BENCHMARKS "Build benchmarks" OFF)
option(BUILD_TESTS "Build unit tests" OFF)
option(BUILD_OLD_TESTS "Build old unit tests" OFF)
option(BUILD_OWN_GOOGLETEST "Build own google's unit testing framework, even if found" OFF)

string(TOLOWER "${CMAKE_BUILD_TYPE}" cmake_build_type_tolower)
if (cmake_build_type_tolower STREQUAL "coverage")
    set(BUILD_TESTS ON)
endif()

find_package(Boost REQUIRED)

# Set version number (change as needed). These definitions are available
# by including "exampleConfig.h" in the source. 
# See exampleConfig.h.in for some more details.
set(PROJECT_VERSION_MAJOR 1)
set(PROJECT_VERSION_MINOR 4)


# Include stuff. No change needed.
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
include(ConfigSafeGuards)
include(Colors)


# --------------------------------------------------------------------------------
#                          Compile flags (change as needed).
# --------------------------------------------------------------------------------
# Set the C++ standard you wish to use (will apply to all files).
# If you do not use any features that limits the standard required,
# you could omit this line.
set(CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Things to always include as flags. Change as needed.
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wpedantic")  

# Build-type specific flags. Change as needed.
SET(CMAKE_CXX_FLAGS_RELEASE "-O3 -DNDEBUG")
SET(CMAKE_CXX_FLAGS_DEBUG "-g -O0 -Wall -Wextra -Wpedantic -Wno-unused-parameter -Wno-sign-compare")

# message(STATUS "Building with the following extra flags: ${CMAKE_CXX_FLAGS}")

# --------------------------------------------------------------------------------
#                         Locate files
# --------------------------------------------------------------------------------
# We make sure that CMake sees all the files.
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/Discreture
    ${PROJECT_SOURCE_DIR}/external/googletest
    ${PROJECT_SOURCE_DIR}/external/googletest/include)



# --------------------------------------------------------------------------------
#                         Make Tests (no change needed).
# --------------------------------------------------------------------------------
# Add a make target 'gtest', that runs the tests (and builds all dependencies).
# The setup of Google Test is done at the very end of this file.


if(${BUILD_TESTS})
    find_package(GTest)
    find_package(Threads REQUIRED)
    set(TEST_LIB googletest)
    file(GLOB TESTFILES "tests/*.cpp")
    set(TEST_MAIN unit_tests.x)
	if (${GTEST_FOUND} AND NOT ${BUILD_OWN_GOOGLETEST})
        include_directories(${GTEST_INCLUDE_DIRS})
        set(TEST_LIB ${GTEST_BOTH_LIBRARIES})
        add_executable(${TEST_MAIN} ${TESTFILES})
	else()
		message("Google testing framework not found. Building our own!")
        add_custom_target( git_update
        COMMAND git submodule init
        COMMAND git submodule update
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR} )
        add_library(googletest
            ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc
            ${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc)
        add_dependencies(googletest git_update)
        set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/src/gtest-all.cc  PROPERTIES GENERATED 1)
        set_source_files_properties(${PROJECT_SOURCE_DIR}/external/googletest/src/gtest_main.cc PROPERTIES GENERATED 1)
        add_executable(${TEST_MAIN} ${TESTFILES})
        add_dependencies(${TEST_MAIN} ${TEST_LIB})
    endif()
    
    target_link_libraries(${TEST_MAIN} ${TEST_LIB} ${CMAKE_THREAD_LIBS_INIT})
    add_custom_target(gtest COMMAND "${PROJECT_BINARY_DIR}/${TEST_MAIN}" DEPENDS ${TEST_MAIN})

    # Add a standard make target 'test' that runs the tests under CTest (only as an alt. to gtest).
    include(CTest)
    enable_testing()
    add_test(unit_tests ${PROJECT_BINARY_DIR}/${TEST_MAIN})
endif()

if (${BUILD_OLD_TESTS})
	file(GLOB OLD_TEST_SRC_FILES ${PROJECT_SOURCE_DIR}/tests/OldTests/*.cpp)
	add_executable(discreture_test_old ${OLD_TEST_SRC_FILES})
endif()


# --------------------------------------------------------------------------------
#                            Build!
# --------------------------------------------------------------------------------
if(${BUILD_EXAMPLES})
	add_executable(combinations examples/combinations.cpp)
	add_executable(combinations_reverse examples/combinations_reverse.cpp)
	add_executable(combinations_tree examples/combinations_tree.cpp)
	add_executable(combinations_tree_reverse examples/combinations_tree_reverse.cpp)
	add_executable(dyck examples/dyck.cpp)
	add_executable(motzkin examples/motzkin.cpp)
	add_executable(permutations examples/permutations.cpp)
	add_executable(arithmetic_progression examples/arithmetic_progression.cpp)
	add_executable(setpartitions examples/setpartitions.cpp)
	add_executable(partitions examples/partitions.cpp)
	add_executable(partitions_reverse examples/partitions_reverse.cpp)
	add_executable(multisets examples/multisets.cpp)
	add_executable(playground examples/playground.cpp)
	add_executable(tutorial examples/tutorial.cpp)
    find_package(Threads)
    if (${CMAKE_USE_PTHREADS_INIT})
        add_executable(tutorial_parallel examples/tutorial_parallel.cpp)
        target_link_libraries(tutorial_parallel ${CMAKE_THREAD_LIBS_INIT})
	endif()
endif()

if(BUILD_BENCHMARKS)
	find_package(GSL)
	if (NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
		add_definitions("-mtune=native")
	endif()
	if (${GSL_FOUND})
		add_definitions(-DTEST_GSL_COMBINATIONS)
		include_directories(${GSL_INCLUDE_DIRS})
	endif()
	file(GLOB BENCHMARK_SRC_FILES ${PROJECT_SOURCE_DIR}/benchmarks/*.cpp)
	add_executable(discreture_benchmark ${BENCHMARK_SRC_FILES})
	if (GSL_FOUND)
		target_link_libraries(discreture_benchmark ${GSL_LIBRARIES})
	endif()
endif()

if(${BUILD_OLD_BENCHMARKS})
	find_package(GSL)
	if (${GSL_FOUND})
		add_definitions(-DTEST_GSL_COMBINATIONS)
		include_directories(${GSL_INCLUDE_DIRS})
	endif(${GSL_FOUND})
	file(GLOB OLD_BENCHMARK_SRC_FILES ${PROJECT_SOURCE_DIR}/Benchmarks/old/*.cpp)
	add_executable(discreture_benchmark_old ${OLD_BENCHMARK_SRC_FILES})
	if (GSL_FOUND)
		target_link_libraries(discreture_benchmark_old ${GSL_LIBRARIES})
	endif(GSL_FOUND)
endif()

include(CodeCoverage)

set( CMAKE_EXPORT_COMPILE_COMMANDS 1 )
