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
include CMakeFiles/morpheus-gba.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/morpheus-gba.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/morpheus-gba.dir/flags.make

CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o: CMakeFiles/morpheus-gba.dir/flags.make
CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o: ../morpheus/core/main_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o -c /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/main_loop.cpp

CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/main_loop.cpp > CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.i

CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/main_loop.cpp -o CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.s

CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o: CMakeFiles/morpheus-gba.dir/flags.make
CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o: ../morpheus/core/gfx/palette_manager.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o -c /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/gfx/palette_manager.cpp

CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/gfx/palette_manager.cpp > CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.i

CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/core/gfx/palette_manager.cpp -o CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.s

CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o: CMakeFiles/morpheus-gba.dir/flags.make
CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o: ../morpheus/gba/gba_main_loop.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o -c /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gba_main_loop.cpp

CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gba_main_loop.cpp > CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.i

CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gba_main_loop.cpp -o CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.s

CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o: CMakeFiles/morpheus-gba.dir/flags.make
CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o: ../morpheus/gba/gfx/sprite.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o -c /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gfx/sprite.cpp

CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.i"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gfx/sprite.cpp > CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.i

CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.s"
	/opt/devkitpro/devkitARM/bin/arm-none-eabi-g++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/morpheus/gba/gfx/sprite.cpp -o CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.s

# Object files for target morpheus-gba
morpheus__gba_OBJECTS = \
"CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o" \
"CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o" \
"CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o" \
"CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o"

# External object files for target morpheus-gba
morpheus__gba_EXTERNAL_OBJECTS =

libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/morpheus/core/main_loop.cpp.o
libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/morpheus/core/gfx/palette_manager.cpp.o
libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/morpheus/gba/gba_main_loop.cpp.o
libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/morpheus/gba/gfx/sprite.cpp.o
libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/build.make
libgba_morpheus-gba.a: CMakeFiles/morpheus-gba.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX static library libgba_morpheus-gba.a"
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-gba.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/morpheus-gba.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/morpheus-gba.dir/build: libgba_morpheus-gba.a

.PHONY : CMakeFiles/morpheus-gba.dir/build

CMakeFiles/morpheus-gba.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/morpheus-gba.dir/cmake_clean.cmake
.PHONY : CMakeFiles/morpheus-gba.dir/clean

CMakeFiles/morpheus-gba.dir/depend:
	cd /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug /mnt/2D835F2A6AFEFA7F/BobbyNewNextCloud/morpheus/cmake-build-debug/CMakeFiles/morpheus-gba.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/morpheus-gba.dir/depend

