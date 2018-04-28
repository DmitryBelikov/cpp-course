cmake_minimum_required(VERSION 2.8)

project(list)

include_directories(${list_SOURCE_DIR})

add_executable(main
               main.cpp
               list.h)

add_executable(list-testing
               list-testing.cpp
               gtest/gtest-all.cc
               gtest/gtest.h
               gtest/gtest_main.cc

               list.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(list-testing -lpthread)
