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
CMAKE_BINARY_DIR = /home/bobby/morpheus/cmake-build-debug-gba

# Include any dependencies generated for this target.
include CMakeFiles/morpheus-gba-input-test.elf.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/morpheus-gba-input-test.elf.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/morpheus-gba-input-test.elf.dir/flags.make

test4.o:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Generating test4.o"
	/opt/devkitpro/tools/bin/grit ../tests/gba/input_test/test4.png -gB4 -Mw 4 -Mh 4
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-as test4.s -otest4.o

test8.o:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Generating test8.o"
	/opt/devkitpro/tools/bin/grit ../tests/gba/input_test/test8.png -gB8 -Mw 4 -Mh 4
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-as test8.s -otest8.o

CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o: CMakeFiles/morpheus-gba-input-test.elf.dir/flags.make
CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o: ../tests/gba/input_test/gba_input_test.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o -c /home/bobby/morpheus/tests/gba/input_test/gba_input_test.cpp

CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/tests/gba/input_test/gba_input_test.cpp > CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.i

CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/tests/gba/input_test/gba_input_test.cpp -o CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.s

# Object files for target morpheus-gba-input-test.elf
morpheus__gba__input__test_elf_OBJECTS = \
"CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o"

# External object files for target morpheus-gba-input-test.elf
morpheus__gba__input__test_elf_EXTERNAL_OBJECTS = \
"/home/bobby/morpheus/cmake-build-debug-gba/test4.o" \
"/home/bobby/morpheus/cmake-build-debug-gba/test8.o"

morpheus-gba-input-test.elf: CMakeFiles/morpheus-gba-input-test.elf.dir/tests/gba/input_test/gba_input_test.cpp.o
morpheus-gba-input-test.elf: test4.o
morpheus-gba-input-test.elf: test8.o
morpheus-gba-input-test.elf: CMakeFiles/morpheus-gba-input-test.elf.dir/build.make
morpheus-gba-input-test.elf: libgba_morpheus.a
morpheus-gba-input-test.elf: CMakeFiles/morpheus-gba-input-test.elf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Linking CXX executable morpheus-gba-input-test.elf"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/morpheus-gba-input-test.elf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/morpheus-gba-input-test.elf.dir/build: morpheus-gba-input-test.elf

.PHONY : CMakeFiles/morpheus-gba-input-test.elf.dir/build

CMakeFiles/morpheus-gba-input-test.elf.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-gba-input-test.elf.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus-gba-input-test.elf.dir/clean

CMakeFiles/morpheus-gba-input-test.elf.dir/depend: test4.o
CMakeFiles/morpheus-gba-input-test.elf.dir/depend: test8.o
	cd /home/bobby/morpheus/cmake-build-debug-gba && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bobby/morpheus /home/bobby/morpheus /home/bobby/morpheus/cmake-build-debug-gba /home/bobby/morpheus/cmake-build-debug-gba /home/bobby/morpheus/cmake-build-debug-gba/CMakeFiles/morpheus-gba-input-test.elf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus-gba-input-test.elf.dir/depend

