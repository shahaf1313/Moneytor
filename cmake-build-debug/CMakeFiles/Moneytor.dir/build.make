# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.14

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
$(VERBOSE).SILENT:


# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake.exe

# The command to remove a file.
RM = /usr/bin/cmake.exe -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/Moneytor.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/Moneytor.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/Moneytor.dir/flags.make

CMakeFiles/Moneytor.dir/main.c.o: CMakeFiles/Moneytor.dir/flags.make
CMakeFiles/Moneytor.dir/main.c.o: ../main.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object CMakeFiles/Moneytor.dir/main.c.o"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/Moneytor.dir/main.c.o   -c /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/main.c

CMakeFiles/Moneytor.dir/main.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/Moneytor.dir/main.c.i"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/main.c > CMakeFiles/Moneytor.dir/main.c.i

CMakeFiles/Moneytor.dir/main.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/Moneytor.dir/main.c.s"
	/usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/main.c -o CMakeFiles/Moneytor.dir/main.c.s

# Object files for target Moneytor
Moneytor_OBJECTS = \
"CMakeFiles/Moneytor.dir/main.c.o"

# External object files for target Moneytor
Moneytor_EXTERNAL_OBJECTS =

Moneytor.exe: CMakeFiles/Moneytor.dir/main.c.o
Moneytor.exe: CMakeFiles/Moneytor.dir/build.make
Moneytor.exe: CMakeFiles/Moneytor.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable Moneytor.exe"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/Moneytor.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/Moneytor.dir/build: Moneytor.exe

.PHONY : CMakeFiles/Moneytor.dir/build

CMakeFiles/Moneytor.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/Moneytor.dir/cmake_clean.cmake
.PHONY : CMakeFiles/Moneytor.dir/clean

CMakeFiles/Moneytor.dir/depend:
	cd /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug /cygdrive/c/Users/Shahaf/CLionProjects/Moneytor/cmake-build-debug/CMakeFiles/Moneytor.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/Moneytor.dir/depend

