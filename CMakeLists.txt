cmake_minimum_required(VERSION 2.8.12)
project(BaseSixFour)

set(SOURCES_DIR ${CMAKE_SOURCE_DIR}/src)
set(TEST_DATA_DIR ${CMAKE_SOURCE_DIR}/tests/data)

include_directories(${CMAKE_SOURCE_DIR}/include)
link_directories(/usr/lib/x86_64-linux-gnu)

add_definitions(-DDATA_DIR=${TEST_DATA_DIR})

file(GLOB SOURCES ${SOURCES_DIR}/*cpp)

add_executable(TestBaseSixFour ${SOURCES})

find_library(GTEST gtest)
find_library(PTHREAD pthread)
target_link_libraries(TestBaseSixFour ${GTEST} ${PTHREAD})
