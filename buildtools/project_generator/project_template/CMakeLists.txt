# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)
include("cmake/morpheus_build_utils.cmake")

## ---------------------------
## Project-dependent variables
## ---------------------------

set(PROJECT_NAME "Project Template Name")

set(MAIN_SRC_FILES src/main.cpp)

set(PNG4_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/sprite_4bpp.png")
set(PNG4_IMAGE_SIZES "32X32")

set(PNG4_BACKGROUND_TILEMAP_FILES "assets/backgrounds/cloud_tilemap.bin")
set(PNG4_BACKGROUND_TILE_FILES "assets/backgrounds/cloud_tiles.png")
set(PNG4_BACKGROUND_TILEMAP_SIZES "32X32")
set(PNG4_BACKGROUND_PALETTE_IDS 1)

set(PNG8_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/player/sprite_8bpp.png")
set(PNG8_IMAGE_SIZES "32X32")

set(MAXMOD_FILES "${CMAKE_CURRENT_SOURCE_DIR}/assets/sfx/test_sound.wav")
set(MAXMOD_SOUND_BANK_NAME "soundbank")

## -----------------------------------
## Nintendo DS game metadata variables
## -----------------------------------

set(NDS_GAME_ICON  "assets/ds_icon.bmp")
set(NDS_TITLE "${PROJECT_NAME}")
set(NDS_SUBTITLE1 "Powered by Morpheus")
set(NDS_SUBTITLE2 " ")

## ---------------------------------
## Library setup-dependent variables
## ---------------------------------

if(WIN32)
    string(REPLACE "\\" "/" DEVKITPRO $ENV{DEVKITPRO})
    string(REPLACE "\\" "/" DEVKITARM $ENV{DEVKITARM})
else()
    set(DEVKITPRO $ENV{DEVKITPRO})
    set(DEVKITARM $ENV{DEVKITARM})
endif()

set(MORPHEUS_DIR "")

# ${CMAKE_CURRENT_BINARY_DIR} is included here for including generated files
set(COMMON_INCLUDE_DIRECTORIES ${MORPHEUS_DIR}/include ${CMAKE_CURRENT_BINARY_DIR})
set(GBA_INCLUDE_DIRECTORIES ${DEVKITPRO}/libtonc/include ${DEVKITPRO}/libgba/include)
set(NDS_INCLUDE_DIRECTORIES ${DEVKITPRO}/libnds/include)

set(COMMON_LINK_DIRECTORIES ${MORPHEUS_DIR}/lib)
set(GBA_LINK_DIRECTORIES ${DEVKITPRO}/libtonc/lib ${DEVKITPRO}/libgba/lib)
set(NDS_LINK_DIRECTORIES ${DEVKITPRO}/libnds/lib)

## ----------------------------
## Cross-compiler configuration
## ----------------------------

string(TOLOWER $ENV{PLATFORM} platform_lower)

if(platform_lower STREQUAL "gba")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")

    add_compile_definitions(_GBA)

    if(NOT GBAFIX)
        find_program(GBAFIX gbafix ${DEVKITPRO}/tools/bin/)
        if(GBAFIX)
            message(STATUS "gbafix: ${GBAFIX} - found")
        else()
            message(FATAL_ERROR "gbafix - not found")
        endif()
    endif()

    if(NOT OBJCOPY)
        find_program(OBJCOPY arm-none-eabi-objcopy ${DEVKITARM}/bin)
        if(OBJCOPY)
            message(STATUS "objcopy: ${OBJCOPY} - found")
        else()
            message(FATAL_ERROR "objcopy - not found")
        endif()
    endif()
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9")
    set(CMAKE_EXE_LINKER_FLAGS "-specs=ds_arm9.specs -g")

    add_compile_definitions(_NDS)

    if(NOT NDSTOOL)
        find_program(NDSTOOL ndstool ${DEVKITPRO}/tools/bin)
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
endif()

set(CMAKE_C_COMPILER_WORKS 1)
set(CMAKE_CXX_COMPILER_WORKS 1)

if(WIN32)
    message(STATUS "Running on Windows")

    set(CMAKE_C_COMPILER "${DEVKITARM}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER "${DEVKITARM}/bin/arm-none-eabi-g++.exe")
    set(CMAKE_LINKER "${DEVKITARM}/bin/arm-none-eabi-ld.exe")
    set(CMAKE_AR "${DEVKITARM}/bin/arm-none-eabi-gcc-ar.exe")
    set(CMAKE_AS "${DEVKITARM}/bin/arm-none-eabi-as.exe")
    set(CMAKE_NM "${DEVKITARM}/bin/arm-none-eabi-gcc-nm.exe")
    set(CMAKE_RANLIB "${DEVKITARM}/bin/arm-none-eabi-gcc-ranlib.exe")
else()
    set(CMAKE_C_COMPILER "${DEVKITARM}/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "${DEVKITARM}/bin/arm-none-eabi-g++")
    set(CMAKE_LINKER "${DEVKITARM}/bin/arm-none-eabi-ld")
    set(CMAKE_AR "${DEVKITARM}/bin/arm-none-eabi-gcc-ar")
    set(CMAKE_AS "${DEVKITARM}/bin/arm-none-eabi-as")
    set(CMAKE_NM "${DEVKITARM}/bin/arm-none-eabi-gcc-nm")
    set(CMAKE_RANLIB "${DEVKITARM}/bin/arm-none-eabi-gcc-ranlib")
endif()

enable_language(ASM)

if(NOT GRIT)
    # message(STATUS "Looking for bin2s...")
    find_program(GRIT grit ${DEVKITPRO}/tools)
    if(GRIT)
        message(STATUS "grit: ${GRIT} - found")
    else()
        message(FATAL_ERROR "grit - not found")
    endif()
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

foreach(png4_file ${PNG4_FILES})
    get_filename_component(png4_file ${png4_file} NAME_WLE)

    list(APPEND PNG4_OUTPUT_FILES ${png4_file}.o)
endforeach()

foreach(png8_file ${PNG8_FILES})
    get_filename_component(png8_file ${png8_file} NAME_WLE)

    list(APPEND PNG8_OUTPUT_FILES ${png8_file}.o)
endforeach()

## ----------------------
## Project Initialization
## ----------------------

project(${PROJECT_NAME})

## ----------------------------
## Background/Sprite generation
## ----------------------------

list(LENGTH PNG4_TILEMAP_FILES png4_tilemap_files_length)

foreach(i RANGE ${png4_tilemap_files_length})
    list(GET PNG4_BACKGROUND_TILEMAP_FILES ${i} tilemap_file)
    list(GET PNG4_BACKGROUND_TILE_FILES ${i} tile_file)
    list(GET PNG4_BACKGROUND_TILEMAP_SIZES ${i} tilemap_size)
    list(GET PNG4_BACKGROUND_PALETTE_IDS ${i} palette_id)

    get_filename_component(tilemap_name ${tilemap_file} NAME_WLE)

    string(REPLACE "X" ";" tilemap_size_list ${tilemap_size})
    list(GET tilemap_size_list 0 tilemap_width)
    list(GET tilemap_size_list 1 tilemap_height)

    convert_tilemap_bin_image_file(${tilemap_file} ${CMAKE_CURRENT_BINARY_DIR} ${tilemap_width}
            ${tilemap_height} ${palette_id} ${tile_file} FALSE FALSE)

    list(APPEND PNG4_OUTPUT_FILES ${CMAKE_CURRENT_BINARY_DIR}/${tilemap_name}.c)
endforeach()

execute_grit_sprites(PNG4_FILES PNG4_IMAGE_SIZES TRUE)
execute_grit_sprites(PNG8_FILES PNG8_IMAGE_SIZES FALSE)

## -----------------------------
## Platform-specific build setup
## -----------------------------

list(LENGTH MAXMOD_FILES MAXMOD_FILES_len)

if(platform_lower STREQUAL "gba")
    libgba_fat_patch()

    message(STATUS "building ${PROJECT_NAME} for the Game Boy Advance")

    if(MAXMOD_FILES_len GREATER 0)
        generate_maxmod_soundbank(true ${MAXMOD_SOUND_BANK_NAME} MAXMOD_FILES)

        list(APPEND MAIN_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${MAXMOD_SOUND_BANK_NAME}.bin.o)
    endif()

    add_executable(${PROJECT_NAME}.elf ${MAIN_SRC_FILES} ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})

    target_include_directories(${PROJECT_NAME}.elf PRIVATE ${COMMON_INCLUDE_DIRECTORIES} ${GBA_INCLUDE_DIRECTORIES})
    target_link_directories(${PROJECT_NAME}.elf PRIVATE ${COMMON_LINK_DIRECTORIES} ${GBA_LINK_DIRECTORIES})
    target_link_libraries(${PROJECT_NAME}.elf gba_morpheus mm tonc)

    add_dependencies(${PROJECT_NAME}.elf libgba_fat_patch_target)

    add_gba_executable(${PROJECT_NAME}.elf)
elseif(platform_lower STREQUAL "nds")
    add_compile_definitions(_NDS)

    message(STATUS "building ${PROJECT_NAME} for Nintendo DS")

    if(MAXMOD_FILES_len GREATER 0)
        generate_maxmod_soundbank(false ${MAXMOD_SOUND_BANK_NAME} MAXMOD_FILES)

        list(APPEND MAIN_SRC_FILES ${CMAKE_CURRENT_BINARY_DIR}/${MAXMOD_SOUND_BANK_NAME}.bin.o)
    endif()

    add_executable(${PROJECT_NAME}.elf ${MAIN_SRC_FILES} ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})

    target_include_directories(${PROJECT_NAME}.elf PRIVATE ${COMMON_INCLUDE_DIRECTORIES} ${NDS_INCLUDE_DIRECTORIES})
    target_link_directories(${PROJECT_NAME}.elf PRIVATE ${COMMON_LINK_DIRECTORIES} ${NDS_LINK_DIRECTORIES})
    target_link_libraries(${PROJECT_NAME}.elf nds_morpheus filesystem fat nds9 mm9)

    add_nds_executable(${PROJECT_NAME}.elf ${NDS_GAME_ICON} ${NDS_TITLE} ${NDS_SUBTITLE1} ${NDS_SUBTITLE2})
endif()