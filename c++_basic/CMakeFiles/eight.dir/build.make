# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.16

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/jose/Desktop/c++/c++_basic

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jose/Desktop/c++/c++_basic

# Include any dependencies generated for this target.
include CMakeFiles/eight.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/eight.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/eight.dir/flags.make

CMakeFiles/eight.dir/eight.cpp.o: CMakeFiles/eight.dir/flags.make
CMakeFiles/eight.dir/eight.cpp.o: eight.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jose/Desktop/c++/c++_basic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/eight.dir/eight.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/eight.dir/eight.cpp.o -c /home/jose/Desktop/c++/c++_basic/eight.cpp

CMakeFiles/eight.dir/eight.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/eight.dir/eight.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jose/Desktop/c++/c++_basic/eight.cpp > CMakeFiles/eight.dir/eight.cpp.i

CMakeFiles/eight.dir/eight.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/eight.dir/eight.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jose/Desktop/c++/c++_basic/eight.cpp -o CMakeFiles/eight.dir/eight.cpp.s

# Object files for target eight
eight_OBJECTS = \
"CMakeFiles/eight.dir/eight.cpp.o"

# External object files for target eight
eight_EXTERNAL_OBJECTS =

eight: CMakeFiles/eight.dir/eight.cpp.o
eight: CMakeFiles/eight.dir/build.make
eight: CMakeFiles/eight.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jose/Desktop/c++/c++_basic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable eight"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/eight.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/eight.dir/build: eight

.PHONY : CMakeFiles/eight.dir/build

CMakeFiles/eight.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/eight.dir/cmake_clean.cmake
.PHONY : CMakeFiles/eight.dir/clean

CMakeFiles/eight.dir/depend:
	cd /home/jose/Desktop/c++/c++_basic && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic/CMakeFiles/eight.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/eight.dir/depend

