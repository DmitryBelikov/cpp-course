cmake_minimum_required(VERSION 2.8)

project(persistent_set)

include_directories(${BIGINT_SOURCE_DIR})

add_executable(main main.cpp
               persistent_set.h
               persistent_set.cpp
               shared_ptr.h
               linked_ptr.h)

if(CMAKE_COMPILER_IS_GNUCC OR CMAKE_COMPILER_IS_GNUCXX)
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2")
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -std=c++11 -pedantic")
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address,undefined -D_GLIBCXX_DEBUG")
endif()
