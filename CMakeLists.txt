cmake_minimum_required(VERSION 2.6)

project(nt)
add_definitions("-std=c++11 -Wall -O0 -g")

find_library(SDL2_MAIN, SDL2main)
find_library(SDL2_LIB SDL2)
find_package(GLEW REQUIRED)
find_library(GL_LIB GL)

add_subdirectory(ext_libs)

include_directories(
  ${GLEW_INCLUDE}
  ${gl_utils_SOURCE_DIR}/src
  )


add_subdirectory(src)
