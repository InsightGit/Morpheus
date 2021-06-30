# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

include(cmake/morpheus_build_utils.cmake)
string(TOLOWER "$ENV{PLATFORM}" platform_lower)

set(BASE_INCLUDE_DIRS ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../../morpheus
        ${CMAKE_CURRENT_SOURCE_DIR}/../../assets)
#set(CMAKE_VERBOSE_MAKEFILE true)

if(WIN32)
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++.exe")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld.exe")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar.exe" CACHE STRING "")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm.exe" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib.exe" CACHE STRING "")
else()
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar" CACHE STRING "")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib" CACHE STRING "")
endif()

enable_language(ASM)

if(platform_lower STREQUAL "gba")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")

    add_compile_definitions(_GBA)

    message(STATUS "Configuring for the Game Boy Advance...")

    include_directories(puzzler PRIVATE ${BASE_INCLUDE_DIRS} /opt/devkitpro/libtonc/include
            /opt/devkitpro/libgba/include/)
    link_directories($ENV{DEVKITPRO}/libtonc/lib $ENV{DEVKITPRO}/libgba/lib)
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9 -specs=ds_arm9.specs")
    set(CMAKE_EXE_LINKER_FLAGS "")

    add_compile_definitions(_NDS)

    message(STATUS "Configuring for the Nintendo DS...")

    include_directories(puzzler PRIVATE ${BASE_INCLUDE_DIRS} /opt/devkitpro/libnds/include)
    link_directories($ENV{DEVKITPRO}/libnds/lib)
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

project(puzzler)


set(MAXMOD_SOUNDBANK_LIST "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/gemcompleted.wav"
                          "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/gemplaced.wav"
                          "${CMAKE_CURRENT_SOURCE_DIR}/assets/music/main_theme3.xm"
                          "${CMAKE_CURRENT_SOURCE_DIR}/assets/music/gameover.xm")
set(MAXMOD_SOUNDBANK_NAME "soundbank")

set(SOURCE_FILES
        src/main.cpp
        "${CMAKE_CURRENT_BINARY_DIR}/maingamescreen.c"
        "${CMAKE_CURRENT_BINARY_DIR}/maingamescreen.h"
        "${CMAKE_CURRENT_BINARY_DIR}/mainmenuscreen.c"
        "${CMAKE_CURRENT_BINARY_DIR}/mainmenuscreen.h"
        "${CMAKE_CURRENT_BINARY_DIR}/smallergamescreen.c"
        "${CMAKE_CURRENT_BINARY_DIR}/smallergamescreen.h"
        "${CMAKE_CURRENT_BINARY_DIR}/smallestgamescreen.c"
        "${CMAKE_CURRENT_BINARY_DIR}/smallestgamescreen.h"
        "${CMAKE_CURRENT_BINARY_DIR}/${MAXMOD_SOUNDBANK_NAME}.bin.o"
        "${CMAKE_CURRENT_BINARY_DIR}/${MAXMOD_SOUNDBANK_NAME}_bin.h"
        "${CMAKE_CURRENT_BINARY_DIR}/${MAXMOD_SOUNDBANK_NAME}.h"
        src/puzzler/main_game_scene.cpp
        src/puzzler/main_game_scene.hpp
        src/puzzler/scene.hpp
        src/puzzler/jewel.cpp
        src/puzzler/jewel.hpp
        src/puzzler/action_timer.hpp
        src/puzzler/main_menu_scene.cpp
        src/puzzler/main_menu_scene.hpp
        src/puzzler/scene_manager.cpp
        src/puzzler/scene_manager.hpp)

if(NOT GRIT)
    # message(STATUS "Looking for bin2s...")
    find_program(GRIT grit ${DEVKITPRO}/tools)
    if(GRIT)
        message(STATUS "grit: ${GRIT} - found")
    else()
        message(FATAL_ERROR "grit - not found")
    endif()
endif()

# Background conversion

convert_tilemap_bin_image_file("assets/backgrounds/maingamescreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                               "assets/backgrounds/maingamescreen.png" false "")
convert_tilemap_bin_image_file("assets/backgrounds/mainmenuscreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 10
                               "assets/backgrounds/mainmenuscreen.png" false "")

convert_tilemap_bin("assets/backgrounds/smallergamescreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0)
convert_tilemap_bin("assets/backgrounds/smallestgamescreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0)

# OBJ (Sprite) conversion

set(OBJ_PNG_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/circlejewel.png"
                  "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/diamondjewel.png"
                  "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/squarejewel.png"
                  "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/trianglejewel.png"
                    "${CMAKE_CURRENT_SOURCE_DIR}/assets/sprites/menucursor.png")
set(OBJ_PNG_SIZES "16X16" "16X16" "16X16" "16X16" "16X16")
set(OBJ_PNG_ARGUMENTS "" "-p!" "-p!" "-p!" "-p!") # tells to omit palette cause all sprites share the same palette

execute_grit_sprites(OBJ_PNG_FILES OBJ_PNG_SIZES OBJ_PNG_ARGUMENTS true)

foreach(obj_png_file ${OBJ_PNG_FILES})
    get_filename_component(obj_png_file ${obj_png_file} NAME_WLE)

    list(APPEND SOURCE_FILES ${obj_png_file}.o)
endforeach()

if(platform_lower STREQUAL "gba")
    if(NOT GBAFIX)
        find_program(GBAFIX gbafix $ENV{DEVKITPRO}/tools/bin/)
        if(GBAFIX)
            message(STATUS "gbafix: ${GBAFIX} - found")
        else()
            message(FATAL_ERROR "gbafix - not found")
        endif()
    endif()

    if(NOT OBJCOPY)
        find_program(OBJCOPY arm-none-eabi-objcopy $ENV{DEVKITARM}/bin)
        if(OBJCOPY)
            message(STATUS "objcopy: ${OBJCOPY} - found")
        else()
            message(FATAL_ERROR "objcopy - not found")
        endif()
    endif()

    generate_maxmod_soundbank(true ${MAXMOD_SOUNDBANK_NAME} MAXMOD_SOUNDBANK_LIST)

    link_directories(puzzler PRIVATE ../../cmake-build-debug-gba $ENV{DEVKITPRO}/libtonc/lib $ENV{DEVKITPRO}/libgba/lib)

    add_executable(puzzler.elf ${SOURCE_FILES})
    add_gba_executable(puzzler.elf)

    target_link_libraries(puzzler.elf gba_morpheus mm tonc)
elseif(platform_lower STREQUAL "nds")
    if(NOT NDSTOOL)
        find_program(NDSTOOL ndstool $ENV{DEVKITPRO}/tools/bin)
        if(NDSTOOL)
            message(STATUS "ndstool: ${NDSTOOL} - found")
        else()
            message(FATAL_ERROR "ndstool - not found")
        endif()
    endif()

    if(NOT BIN2S)
        # message(STATUS "Looking for bin2s...")
        find_program(BIN2S bin2s ${DEVKITARM}/bin)
        if(BIN2S)
            message(STATUS "bin2s: ${BIN2S} - found")
        else()
            message(FATAL_ERROR "bin2s - not found")
        endif()
    endif()

    link_directories(puzzler PRIVATE ../../cmake-build-debug-nds $ENV{DEVKITPRO}/libnds/lib)

    convert_tilemap_bin_image_file("assets/backgrounds/subscorescreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                                   "assets/backgrounds/subscorescreen.png" false "")
    convert_tilemap_bin_image_file("assets/backgrounds/submainmenuscreen.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                                   "assets/backgrounds/submainmenuscreen.png" true "-p!")

    list(APPEND SOURCE_FILES
            "${CMAKE_CURRENT_BINARY_DIR}/subscorescreen.c"
            "${CMAKE_CURRENT_BINARY_DIR}/subscorescreen.h"
            "${CMAKE_CURRENT_BINARY_DIR}/submainmenuscreen.c"
            "${CMAKE_CURRENT_BINARY_DIR}/submainmenuscreen.h")

    generate_maxmod_soundbank(false ${MAXMOD_SOUNDBANK_NAME} MAXMOD_SOUNDBANK_LIST)

    add_executable(puzzler.elf ${SOURCE_FILES})
    add_nds_executable(puzzler.elf "ds_icon.bmp"
            "Puzzler" "Powered by Morpheus engine" "")

    target_link_libraries(puzzler.elf nds_morpheus nds9 mm9)
endif()
