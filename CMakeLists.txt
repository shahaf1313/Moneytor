cmake_minimum_required(VERSION 3.14)
project(Moneytor CXX C)

add_executable(Moneytor main.c list.c)

set(CUTIE_DIR Cutie)
include(${CUTIE_DIR}/Cutie.cmake)
add_cutie_test_target(TEST test/test_list.cpp SOURCES list.c)

set(CMAKE_C_STANDARD 99)

