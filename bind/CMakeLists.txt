project(bind)
cmake_minimum_required(VERSION 2.8)

include_directories(${bind_SOURCE_DIR})

add_executable(bind_test
                gtest/gtest_main.cc
                gtest/gtest-all.cc
                gtest/gtest.h
                bind.h
                test.cpp
                )

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++17 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(bind_test -lpthread)
