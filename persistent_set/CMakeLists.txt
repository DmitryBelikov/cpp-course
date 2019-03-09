cmake_minimum_required(VERSION 2.8)

project(persistent_set)

include_directories(${persistent_set_SOURCE_DIR})

add_executable(my_test test.cpp
               gtest/gtest_main.cc
               gtest/gtest-all.cc
               gtest/gtest.h
               persistent_set.h
               persistent_set.cpp
               shared_ptr.h
               linked_ptr.h)

 add_executable(main main.cpp
                persistent_set.h
                persistent_set.cpp
                shared_ptr.h
                linked_ptr.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++14 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()

target_link_libraries(my_test -lpthread)
