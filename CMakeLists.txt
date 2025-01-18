cmake_minimum_required(VERSION 3.10)
project(B1Emulator)
set(CMAKE_CXX_STANDARD 11)
include_directories(${CMAKE_SOURCE_DIR}/include)
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)
add_executable(emulator ${SOURCES})
# If you have any specific compiler flags, you can add them here
# For example:
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
