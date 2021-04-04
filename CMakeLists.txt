cmake_minimum_required(VERSION 3.1...3.20)

project(
  Doom
  VERSION 1.0
  LANGUAGES CXX)

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(doom main.cpp wad.cpp)
target_link_libraries(doom ${SDL2_LIBRARIES})
