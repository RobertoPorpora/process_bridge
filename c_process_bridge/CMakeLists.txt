cmake_minimum_required(VERSION 3.10)

project(process_bridge)

# Add the main library
add_library(PB STATIC src/process_bridge.c)

# Include directories
target_include_directories(PB PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)