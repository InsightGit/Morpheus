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
CMAKE_COMMAND = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/203.6682.181/bin/cmake/linux/bin/cmake

# The command to remove a file.
RM = /home/bobby/.local/share/JetBrains/Toolbox/apps/CLion/ch-0/203.6682.181/bin/cmake/linux/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/bobby/morpheus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bobby/morpheus/cmake-build-debug-gba

# Include any dependencies generated for this target.
include CMakeFiles/morpheus-gba-tileset-test.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/morpheus-gba-tileset-test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/morpheus-gba-tileset-test.elf.dir/flags.make

region_map.c:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating region_map.c, region_map.h"
	/usr/bin/python3 /home/bobby/morpheus/buildtools/bintileconvert/bintileconvert.py /home/bobby/morpheus/tests/tileset_test/region_map.bin /home/bobby/morpheus/cmake-build-debug-gba 0 /home/bobby/morpheus/tests/tileset_test/region_map.png 8

region_map.h: region_map.c
	@$(CMAKE_COMMAND) -E touch_nocreate region_map.h

CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o: CMakeFiles/morpheus-gba-tileset-test.elf.dir/flags.make
CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o: ../tests/tileset_test/tileset_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o -c /home/bobby/morpheus/tests/tileset_test/tileset_test.cpp

CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/tests/tileset_test/tileset_test.cpp > CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.i

CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/tests/tileset_test/tileset_test.cpp -o CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.s

CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o: CMakeFiles/morpheus-gba-tileset-test.elf.dir/flags.make
CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o: region_map.c
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building C object CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -o CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o   -c /home/bobby/morpheus/cmake-build-debug-gba/region_map.c

CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/bobby/morpheus/cmake-build-debug-gba/region_map.c > CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.i

CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-gcc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/bobby/morpheus/cmake-build-debug-gba/region_map.c -o CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.s

# Object files for target morpheus-gba-tileset-test.elf
morpheus__gba__tileset__test_elf_OBJECTS = \
"CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o" \
"CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o"

# External object files for target morpheus-gba-tileset-test.elf
morpheus__gba__tileset__test_elf_EXTERNAL_OBJECTS =

morpheus-gba-tileset-test.elf: CMakeFiles/morpheus-gba-tileset-test.elf.dir/tests/tileset_test/tileset_test.cpp.o
morpheus-gba-tileset-test.elf: CMakeFiles/morpheus-gba-tileset-test.elf.dir/region_map.c.o
morpheus-gba-tileset-test.elf: CMakeFiles/morpheus-gba-tileset-test.elf.dir/build.make
morpheus-gba-tileset-test.elf: libgba_morpheus.a
morpheus-gba-tileset-test.elf: CMakeFiles/morpheus-gba-tileset-test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable morpheus-gba-tileset-test.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/morpheus-gba-tileset-test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/morpheus-gba-tileset-test.elf.dir/build: morpheus-gba-tileset-test.elf

.PHONY : CMakeFiles/morpheus-gba-tileset-test.elf.dir/build

CMakeFiles/morpheus-gba-tileset-test.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-gba-tileset-test.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus-gba-tileset-test.elf.dir/clean

CMakeFiles/morpheus-gba-tileset-test.elf.dir/depend: region_map.c
CMakeFiles/morpheus-gba-tileset-test.elf.dir/depend: region_map.h
	cd /home/bobby/morpheus/cmake-build-debug-gba && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bobby/morpheus /home/bobby/morpheus /home/bobby/morpheus/cmake-build-debug-gba /home/bobby/morpheus/cmake-build-debug-gba /home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles/morpheus-gba-tileset-test.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus-gba-tileset-test.elf.dir/depend

