# cmakelists to compile the quench code
# author: Y.Yang (Kyushu University)
# date: 1st Aug 2016

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(QuenchCode)

set(TARGET SimQuench.exe)

# find FindROOT.cmake and append it
#list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS}/etc/root/cmake)
#find_package(ROOT REQUIRED)
#include(${ROOT_USE_FILE})

# include quench inculde files
include_directories(include)
add_subdirectory(src)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} Quench)
