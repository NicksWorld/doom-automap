cmake_minimum_required(VERSION 3.1...3.20)

project(
  Doom
  VERSION 1.0
  LANGUAGES CXX)

add_executable(doomsvg src/main.cpp src/wad.cpp)
