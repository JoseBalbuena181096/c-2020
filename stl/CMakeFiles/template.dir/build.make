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
CMAKE_SOURCE_DIR = /home/jose/Desktop/c++/stl

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jose/Desktop/c++/stl

# Include any dependencies generated for this target.
include CMakeFiles/template.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/template.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/template.dir/flags.make

CMakeFiles/template.dir/template.cpp.o: CMakeFiles/template.dir/flags.make
CMakeFiles/template.dir/template.cpp.o: template.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/jose/Desktop/c++/stl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/template.dir/template.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/template.dir/template.cpp.o -c /home/jose/Desktop/c++/stl/template.cpp

CMakeFiles/template.dir/template.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/template.dir/template.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/jose/Desktop/c++/stl/template.cpp > CMakeFiles/template.dir/template.cpp.i

CMakeFiles/template.dir/template.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/template.dir/template.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/jose/Desktop/c++/stl/template.cpp -o CMakeFiles/template.dir/template.cpp.s

# Object files for target template
template_OBJECTS = \
"CMakeFiles/template.dir/template.cpp.o"

# External object files for target template
template_EXTERNAL_OBJECTS =

template: CMakeFiles/template.dir/template.cpp.o
template: CMakeFiles/template.dir/build.make
template: CMakeFiles/template.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/jose/Desktop/c++/stl/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable template"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/template.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/template.dir/build: template

.PHONY : CMakeFiles/template.dir/build

CMakeFiles/template.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/template.dir/cmake_clean.cmake
.PHONY : CMakeFiles/template.dir/clean

CMakeFiles/template.dir/depend:
	cd /home/jose/Desktop/c++/stl && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jose/Desktop/c++/stl /home/jose/Desktop/c++/stl /home/jose/Desktop/c++/stl /home/jose/Desktop/c++/stl /home/jose/Desktop/c++/stl/CMakeFiles/template.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/template.dir/depend

