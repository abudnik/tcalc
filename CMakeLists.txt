project(tcalc)

cmake_minimum_required(VERSION 2.6)

include_directories("${CMAKE_CURRENT_SOURCE_DIR}/include")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-enum-compare")

add_executable(test "${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp")
add_executable(prolog "${CMAKE_CURRENT_SOURCE_DIR}/src/test.cpp")
