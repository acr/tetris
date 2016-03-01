cmake_minimum_required(VERSION 2.6)

project(gl_utils)
add_definitions("-std=c++11 -Wall -O0 -g")
set(BUILD_SHARED_LIBS ON)
find_package(GLEW REQUIRED)
include_directories(
  ${GLEW_INCLUDE}
  )
add_subdirectory(src)
