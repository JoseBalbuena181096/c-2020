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
CMAKE_SOURCE_DIR = /home/jose/Desktop/c++/several

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jose/Desktop/c++/several

# Include any dependencies generated for this target.
include CMakeFiles/dbscan.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/dbscan.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/dbscan.dir/flags.make

CMakeFiles/dbscan.dir/dbscan.cpp.o: CMakeFiles/dbscan.dir/flags.make
CMakeFiles/dbscan.dir/dbscan.cpp.o: dbscan.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jose/Desktop/c++/several/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/dbscan.dir/dbscan.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/dbscan.dir/dbscan.cpp.o -c /home/jose/Desktop/c++/several/dbscan.cpp

CMakeFiles/dbscan.dir/dbscan.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/dbscan.dir/dbscan.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jose/Desktop/c++/several/dbscan.cpp > CMakeFiles/dbscan.dir/dbscan.cpp.i

CMakeFiles/dbscan.dir/dbscan.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/dbscan.dir/dbscan.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jose/Desktop/c++/several/dbscan.cpp -o CMakeFiles/dbscan.dir/dbscan.cpp.s

# Object files for target dbscan
dbscan_OBJECTS = \
"CMakeFiles/dbscan.dir/dbscan.cpp.o"

# External object files for target dbscan
dbscan_EXTERNAL_OBJECTS =

dbscan: CMakeFiles/dbscan.dir/dbscan.cpp.o
dbscan: CMakeFiles/dbscan.dir/build.make
dbscan: CMakeFiles/dbscan.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jose/Desktop/c++/several/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable dbscan"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/dbscan.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/dbscan.dir/build: dbscan

.PHONY : CMakeFiles/dbscan.dir/build

CMakeFiles/dbscan.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/dbscan.dir/cmake_clean.cmake
.PHONY : CMakeFiles/dbscan.dir/clean

CMakeFiles/dbscan.dir/depend:
	cd /home/jose/Desktop/c++/several && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jose/Desktop/c++/several /home/jose/Desktop/c++/several /home/jose/Desktop/c++/several /home/jose/Desktop/c++/several /home/jose/Desktop/c++/several/CMakeFiles/dbscan.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/dbscan.dir/depend

