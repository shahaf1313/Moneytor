cmake_minimum_required(VERSION 3.14)
project(Moneytor CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON) #adds the project root to the include path
set(CMAKE_VERBOSE_MAKEFILE ON)  #Makes the compilation process be verbose (will appear in the Messages)

add_executable(Moneytor src/main.c src/list.c)

set(CUTIE_DIR Cutie)
include(${CUTIE_DIR}/Cutie.cmake)
add_cutie_test_target(TEST test/test_list.cpp SOURCES src/list.c)

set(CMAKE_C_STANDARD 99)

