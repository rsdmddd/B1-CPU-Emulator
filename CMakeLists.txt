cmake_minimum_required(VERSION 3.10)

# Project name
project(MyProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)

# Add include directories
include_directories(${CMAKE_SOURCE_DIR}/include)

# Add source files
file(GLOB SOURCES ${CMAKE_SOURCE_DIR}/src/*.cpp)

# Create the executable from the source files
add_executable(MyExecutable ${SOURCES})

# If you have any specific compiler flags, you can add them here
# For example:
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
