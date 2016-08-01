# cmakelists to compile the quench code
# author: Y.Yang (Kyushu University)
# date: 1st Aug 2016

cmake_minimum_required(VERSION 2.8)

set(QUENCHLIB Quench)
file(GLOB QUENCHSRCS "*.cpp")

add_library(${QUENCHLIB} STATIC ${QUENCHSRCS})
