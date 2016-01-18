cmake_minimum_required(VERSION 3.2 FATAL_ERROR)
project(BaseSixFour)

# set project variables
set(HEADERS_DIR ${CMAKE_SOURCE_DIR}/include)
set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)
set(TESTS_DIR ${CMAKE_SOURCE_DIR}/tests)
set(TEST_DATA_DIR ${TESTS_DIR}/data)

# set include directories
include_directories(${HEADERS_DIR})

# set library directories
link_directories(/usr/lib/x86_64-linux-gnu)

# set defines
add_definitions(-DDATA_DIR="${TEST_DATA_DIR}")

# glob project files
file(GLOB HEADERS ${HEADERS_DIR}/*.h)
file(GLOB SOURCES ${SOURCES_DIR}/*.cpp ${TESTS_DIR}/*.cpp)

# set targets
add_executable(TestBaseSixFour ${SOURCES} ${HEADERS})

# add features support
set_property(TARGET TestBaseSixFour PROPERTY CXX_STANDARD 11)
set_property(TARGET TestBaseSixFour PROPERTY CXX_STANDARD_REQUIRED ON)

# find libraries
find_library(GTEST gtest)
find_library(PTHREAD pthread)

# link libraries to targets
target_link_libraries(TestBaseSixFour ${GTEST} ${PTHREAD})
