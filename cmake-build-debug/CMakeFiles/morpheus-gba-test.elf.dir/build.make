# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.17

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
CMAKE_COMMAND = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/203.5981.166/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/203.5981.166/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/morpheus-gba-test.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/morpheus-gba-test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/morpheus-gba-test.elf.dir/flags.make

CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o: CMakeFiles/morpheus-gba-test.elf.dir/flags.make
CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o: ../tests/gba_input_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o -c /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/tests/gba_input_test.cpp

CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/tests/gba_input_test.cpp > CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.i

CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/tests/gba_input_test.cpp -o CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.s

# Object files for target morpheus-gba-test.elf
morpheus__gba__test_elf_OBJECTS = \
"CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o"

# External object files for target morpheus-gba-test.elf
morpheus__gba__test_elf_EXTERNAL_OBJECTS =

morpheus-gba-test.elf: CMakeFiles/morpheus-gba-test.elf.dir/tests/gba_input_test.cpp.o
morpheus-gba-test.elf: CMakeFiles/morpheus-gba-test.elf.dir/build.make
morpheus-gba-test.elf: libmorpheus.a
morpheus-gba-test.elf: CMakeFiles/morpheus-gba-test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable morpheus-gba-test.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/morpheus-gba-test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/morpheus-gba-test.elf.dir/build: morpheus-gba-test.elf

.PHONY : CMakeFiles/morpheus-gba-test.elf.dir/build

CMakeFiles/morpheus-gba-test.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-gba-test.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus-gba-test.elf.dir/clean

CMakeFiles/morpheus-gba-test.elf.dir/depend:
	cd /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles/morpheus-gba-test.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus-gba-test.elf.dir/depend

