# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.19

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Disable VCS-based implicit rules.
% : %,v


# Disable VCS-based implicit rules.
% : RCS/%


# Disable VCS-based implicit rules.
% : RCS/%,v


# Disable VCS-based implicit rules.
% : SCCS/s.%


# Disable VCS-based implicit rules.
% : s.%


.SUFFIXES: .hpux_make_needs_suffix_list


# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:

.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.6432.9/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/211.6432.9/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bobby/morpheus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bobby/morpheus/cmake-build-debug-nds

# Utility rule file for morpheus-nds-input-test.nds.

# Include the progress variables for this target.
include CMakeFiles/morpheus-nds-input-test.nds.dir/progress.make

CMakeFiles/morpheus-nds-input-test.nds: morpheus-nds-input-test.elf
	/opt/devkitpro/tools/bin/ndstool -c morpheus-nds-input-test.nds -9 morpheus-nds-input-test.elf -b /home/bobby/morpheus/tests/nds/input_test/ds_icon.bmp "Morpheus Input Test;Morpheus engine;"

morpheus-nds-input-test.nds: CMakeFiles/morpheus-nds-input-test.nds
morpheus-nds-input-test.nds: CMakeFiles/morpheus-nds-input-test.nds.dir/build.make

.PHONY : morpheus-nds-input-test.nds

# Rule to build all files generated by this target.
CMakeFiles/morpheus-nds-input-test.nds.dir/build: morpheus-nds-input-test.nds

.PHONY : CMakeFiles/morpheus-nds-input-test.nds.dir/build

CMakeFiles/morpheus-nds-input-test.nds.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-nds-input-test.nds.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus-nds-input-test.nds.dir/clean

CMakeFiles/morpheus-nds-input-test.nds.dir/depend:
	cd /home/bobby/morpheus/cmake-build-debug-nds && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bobby/morpheus /home/bobby/morpheus /home/bobby/morpheus/cmake-build-debug-nds /home/bobby/morpheus/cmake-build-debug-nds /home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles/morpheus-nds-input-test.nds.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus-nds-input-test.nds.dir/depend

