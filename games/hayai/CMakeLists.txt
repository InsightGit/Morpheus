cmake_minimum_required(VERSION 3.19)
include("cmake/morpheus_build_utils.cmake")

if(WIN32)
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
else()
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
endif()

string(TOLOWER $ENV{PLATFORM} platform_lower)

if(platform_lower STREQUAL "gba")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9")
    set(CMAKE_EXE_LINKER_FLAGS "-specs=ds_arm9.specs -g")
endif()

if(NOT GRIT)
    # message(STATUS "Looking for bin2s...")
    find_program(GRIT grit ${DEVKITPRO}/tools)
    if(GRIT)
        message(STATUS "grit: ${GRIT} - found")
    else()
        message(FATAL_ERROR "grit - not found")
    endif()
endif()

#set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

set(MAIN_SRC_FILES src/main.cpp
        src/hayai/level.cpp src/hayai/level.hpp src/hayai/scene.hpp src/hayai/player.cpp src/hayai/player.hpp
        src/hayai/player_hud.cpp src/hayai/player_hud.hpp src/hayai/enemy.cpp src/hayai/enemy.hpp)

#if(platform_lower STREQUAL "gba")
#    set(PNG4_FILES "../tests/gba/test4.png")
#elseif(platform_lower STREQUAL "nds")
#    set(PNG4_FILES "../tests/nds/test4.png")
#endif()

set(PNG4_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerleftarms0.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerleftarms1.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerleftarms2.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerrightarms0.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerrightarms1.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/playerrightarms2.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/enemydown.png"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/enemyup.png")
set(MAXMOD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/coin_pickup.wav"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/enemy_kill.wav"
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/player_hurt.wav")

#if(platform_lower STREQUAL "gba")
#    set(PNG8_FILES "../tests/gba/test8.png" CACHE STRING
#            "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
#elseif(platform_lower STREQUAL "nds")
#    set(PNG8_FILES "../tests/nds/test8.png" "../tests/nds/testt.png" CACHE STRING
#            "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
#endif()

set(PNG4_IMAGE_SIZES "32X32" "32X32" "32X32" "32X32" "32X32" "32X32" "32X32" "32X32")
#set(PNG8_IMAGE_SIZES "32X32" "32X32" "32X32" "32X32" "32X32" "32X32" CACHE STRING
#        "Image sizes of the previously listed 8bpp PNG files (widthXlength)")

execute_grit_sprites(PNG4_FILES PNG4_IMAGE_SIZES TRUE)
#execute_grit(PNG8_FILES PNG8_IMAGE_SIZES FALSE)

foreach(png4_file ${PNG4_FILES})
    get_filename_component(png4_file ${png4_file} NAME_WLE)

    list(APPEND PNG4_OUTPUT_FILES ${png4_file}.o)
endforeach()

#foreach(png8_file ${PNG8_FILES})
#    get_filename_component(png8_file ${png8_file} NAME_WLE)

#    list(APPEND PNG8_OUTPUT_FILES ${png8_file}.o)
#endforeach()

#message(STATUS "${PNG8_OUTPUT_FILES}")

project(hayai)

include_directories(hayai PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ../../morpheus .. $ENV{DEVKITPRO}/libnds/include)

convert_tilemap_bin_image_file("assets/test_map_1.bin" ${CMAKE_CURRENT_BINARY_DIR} 64 64 0 "assets/tiles1.png" TRUE
                               FALSE)
#convert_tilemap_bin_image_file("assets/test_map_1_128.bin" ${CMAKE_CURRENT_BINARY_DIR} 64 128 0 "assets/tiles1.png" TRUE
#        FALSE)

list(APPEND PNG4_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/test_map_1.c ${CMAKE_CURRENT_BINARY_DIR}/test_map_1.h)
#list(APPEND PNG4_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/test_map_1_128.c ${CMAKE_CURRENT_BINARY_DIR}/test_map_1_128.h)
list(APPEND PNG4_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/player_hud.c ${CMAKE_CURRENT_BINARY_DIR}/player_hud.h)

if(platform_lower STREQUAL "gba")
    add_compile_definitions(_GBA)

    message(STATUS "building hayai for Gameboy Advance")

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

    generate_maxmod_soundbank(true "soundbank" MAXMOD_FILES)

    include_directories(hayai PRIVATE $ENV{DEVKITPRO}/libtonc/include $ENV{DEVKITPRO}/libgba/include)
    link_directories(hayai PRIVATE ../../cmake-build-debug-gba $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

    add_executable(hayai.elf ${MAIN_SRC_FILES} ${PNG4_OUTPUT_FILES} ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o) #${PNG8_OUTPUT_FILES})
    add_gba_executable(hayai.elf)

    target_link_libraries(hayai.elf gba_morpheus mm tonc)
elseif(platform_lower STREQUAL "nds")
    add_compile_definitions(_NDS)

    message(STATUS "building hayai for Nintendo DS")

    set(NDS_GAME_ICON  "tests/nds/ds_icon.bmp" CACHE STRING "NDS game icon location realitve to project location")
    set(NDS_TITLE "Hayai" CACHE STRING "NDS game title")
    set(NDS_SUBTITLE1 "Powered by Morpheus" CACHE STRING "NDS game subtitle #1")
    set(NDS_SUBTITLE2 "" CACHE STRING "NDS game subtitle #2")

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

    generate_maxmod_soundbank(false "soundbank" MAXMOD_FILES)

    link_directories(hayai PRIVATE ../../cmake-build-debug-nds $ENV{DEVKITPRO}/libnds/lib)

    add_executable(hayai.elf ${MAIN_SRC_FILES} ${PNG4_OUTPUT_FILES} ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o) #${PNG8_OUTPUT_FILES})
    add_nds_executable(hayai.elf assets/ds_icon.bmp "Hayai" "an Acceleration-based platformer" "powered by Morpheus")

    target_link_libraries(hayai.elf nds_morpheus fat filesystem nds9 mm9)
endif()

if(WIN32)
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++.exe")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld.exe")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar.exe" CACHE STRING "")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm.exe" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib.exe" CACHE STRING "")
else()
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar" CACHE STRING "")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib" CACHE STRING "")
endif()

set(CMAKE_AS ASSEMBLER_TO_USE CACHE STRING "")

enable_language(ASM)
