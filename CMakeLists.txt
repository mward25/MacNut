cmake_minimum_required(VERSION 3.24)

project(MacNut)

add_library(${CMAKE_PROJECT_NAME} INTERFACE)

target_include_directories(${CMAKE_PROJECT_NAME}
                           INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}/src/")
