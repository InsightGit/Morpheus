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
CMAKE_SOURCE_DIR = /home/bobby/morpheus

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/bobby/morpheus/cmake-build-debug-nds

# Include any dependencies generated for this target.
include CMakeFiles/morpheus.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/morpheus.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/morpheus.dir/flags.make

CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o: CMakeFiles/morpheus.dir/flags.make
CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o: ../morpheus/core/main_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o -c /home/bobby/morpheus/morpheus/core/main_loop.cpp

CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/morpheus/core/main_loop.cpp > CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.i

CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/morpheus/core/main_loop.cpp -o CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.s

CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o: CMakeFiles/morpheus.dir/flags.make
CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o: ../morpheus/core/gfx/palette_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o -c /home/bobby/morpheus/morpheus/core/gfx/palette_manager.cpp

CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/morpheus/core/gfx/palette_manager.cpp > CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.i

CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/morpheus/core/gfx/palette_manager.cpp -o CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.s

CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o: CMakeFiles/morpheus.dir/flags.make
CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o: ../morpheus/nds/nds_main_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o -c /home/bobby/morpheus/morpheus/nds/nds_main_loop.cpp

CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/morpheus/nds/nds_main_loop.cpp > CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.i

CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/morpheus/nds/nds_main_loop.cpp -o CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.s

CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o: CMakeFiles/morpheus.dir/flags.make
CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o: ../morpheus/nds/gfx/sprite.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o -c /home/bobby/morpheus/morpheus/nds/gfx/sprite.cpp

CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/bobby/morpheus/morpheus/nds/gfx/sprite.cpp > CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.i

CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/bobby/morpheus/morpheus/nds/gfx/sprite.cpp -o CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.s

# Object files for target morpheus
morpheus_OBJECTS = \
"CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o" \
"CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o" \
"CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o" \
"CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o"

# External object files for target morpheus
morpheus_EXTERNAL_OBJECTS =

libnds_morpheus.a: CMakeFiles/morpheus.dir/morpheus/core/main_loop.cpp.o
libnds_morpheus.a: CMakeFiles/morpheus.dir/morpheus/core/gfx/palette_manager.cpp.o
libnds_morpheus.a: CMakeFiles/morpheus.dir/morpheus/nds/nds_main_loop.cpp.o
libnds_morpheus.a: CMakeFiles/morpheus.dir/morpheus/nds/gfx/sprite.cpp.o
libnds_morpheus.a: CMakeFiles/morpheus.dir/build.make
libnds_morpheus.a: CMakeFiles/morpheus.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libnds_morpheus.a"
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/morpheus.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/morpheus.dir/build: libnds_morpheus.a

.PHONY : CMakeFiles/morpheus.dir/build

CMakeFiles/morpheus.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus.dir/clean

CMakeFiles/morpheus.dir/depend:
	cd /home/bobby/morpheus/cmake-build-debug-nds && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/bobby/morpheus /home/bobby/morpheus /home/bobby/morpheus/cmake-build-debug-nds /home/bobby/morpheus/cmake-build-debug-nds /home/bobby/morpheus/cmake-build-debug-nds/CMakeFiles/morpheus.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus.dir/depend

