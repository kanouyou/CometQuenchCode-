# cmakelists to compile the quench code
# author: Y.Yang (Kyushu University)
# date: 1st Aug 2016

cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
project(QuenchCode)

set(TARGET SimQuench.exe)

# find FindROOT.cmake and append it
set(libs)
set(CMAKE_MODULE_PATH $ENV{ROOTSYS}/etc/root/cmake)
find_package(ROOT REQUIRED)
list(APPEND libs ${ROOT_LIBRARIES})
include_directories(${ROOT_INCLUDE_DIR})

# include quench inculde files
include_directories(include)
add_subdirectory(src)

add_executable(${TARGET} main.cpp)
target_link_libraries(${TARGET} ${libs} Quench)
