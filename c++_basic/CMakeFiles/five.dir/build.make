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
include CMakeFiles/five.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/five.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/five.dir/flags.make

CMakeFiles/five.dir/five.cpp.o: CMakeFiles/five.dir/flags.make
CMakeFiles/five.dir/five.cpp.o: five.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jose/Desktop/c++/c++_basic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/five.dir/five.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/five.dir/five.cpp.o -c /home/jose/Desktop/c++/c++_basic/five.cpp

CMakeFiles/five.dir/five.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/five.dir/five.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jose/Desktop/c++/c++_basic/five.cpp > CMakeFiles/five.dir/five.cpp.i

CMakeFiles/five.dir/five.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/five.dir/five.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jose/Desktop/c++/c++_basic/five.cpp -o CMakeFiles/five.dir/five.cpp.s

# Object files for target five
five_OBJECTS = \
"CMakeFiles/five.dir/five.cpp.o"

# External object files for target five
five_EXTERNAL_OBJECTS =

five: CMakeFiles/five.dir/five.cpp.o
five: CMakeFiles/five.dir/build.make
five: CMakeFiles/five.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jose/Desktop/c++/c++_basic/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable five"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/five.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/five.dir/build: five

.PHONY : CMakeFiles/five.dir/build

CMakeFiles/five.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/five.dir/cmake_clean.cmake
.PHONY : CMakeFiles/five.dir/clean

CMakeFiles/five.dir/depend:
	cd /home/jose/Desktop/c++/c++_basic && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic /home/jose/Desktop/c++/c++_basic/CMakeFiles/five.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/five.dir/depend
