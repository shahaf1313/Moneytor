cmake_minimum_required(VERSION 3.14)
project(Moneytor CXX C)

set(CMAKE_INCLUDE_CURRENT_DIR ON) #adds the project root to the include path
set(CMAKE_VERBOSE_MAKEFILE ON)  #Makes the compilation process be verbose (will appear in the Messages)

add_executable(Moneytor src/common.c src/list.c src/file_info.c src/dir_info.c src/utills.c src/file_system_scanner.c src/main.c)

set(CUTIE_DIR Cutie)
include(${CUTIE_DIR}/Cutie.cmake)
add_cutie_test_target(TEST test/test_list.cpp SOURCES src/common.c src/list.c)
add_cutie_test_target(TEST test/test_listDataHandlers.cpp SOURCES src/common.c src/file_info.c src/dir_info.c src/list.c)
add_cutie_test_target(TEST test/test_utills.cpp SOURCES src/common.c src/list.c src/file_info.c src/dir_info.c src/utills.c src/file_system_scanner.c)
add_cutie_all_tests_target()

set(CMAKE_C_STANDARD 99)

