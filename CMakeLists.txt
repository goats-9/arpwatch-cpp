cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(arpwatch-cpp VERSION 1.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# add directories to this project
add_subdirectory(src)