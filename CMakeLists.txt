cmake_minimum_required(VERSION 3.24)

project(MacNut)

add_library(MacNut INTERFACE)

target_include_directories(MacNut INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/")
