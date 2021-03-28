# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

include(cmake/morpheus_build_utils.cmake)
string(TOLOWER "$ENV{PLATFORM}" platform_lower)

set(MORPHEUS_PROJECT_NAME "morpheus-${platform_lower}")
set(MORPHEUS_SOURCE_FILES
        morpheus/core/controllers.hpp
        morpheus/core/controllers.cpp
        morpheus/core/main_loop.hpp
        morpheus/core/main_loop.cpp
        morpheus/core/node.hpp
        morpheus/core/node.cpp
        morpheus/core/save_manager.hpp
        morpheus/core/uncopyable.hpp
        morpheus/core/audio/max_mod_music.hpp
        morpheus/core/audio/max_mod_music.cpp
        morpheus/core/audio/max_mod_sfx.hpp
        morpheus/core/audio/max_mod_sfx.cpp
        morpheus/core/gfx/tiled_background_base.hpp
        morpheus/core/gfx/tiled_background_base.cpp
        morpheus/core/gfx/vector_2.hpp
        morpheus/core/gfx/window.hpp
        morpheus/core/gfx/window.cpp
        morpheus/utils.hpp morpheus/core/gfx/sprite_base.cpp morpheus/core/gfx/sprite_base.hpp)


project(morpheus)

#set( CMAKE_VERBOSE_MAKEFILE on )

set(BASE_INCLUDE_DIRS . ${CMAKE_CURRENT_BINARY_DIR})

if(platform_lower STREQUAL "gba")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libgba_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")

    add_compile_definitions(_GBA)

    list(APPEND MORPHEUS_SOURCE_FILES
            morpheus/gba/gba_controllers.hpp
            morpheus/gba/gba_controllers.cpp
            morpheus/gba/gba_main_loop.hpp
            morpheus/gba/gba_main_loop.cpp
            morpheus/gba/audio/gba_max_mod_music.hpp
            morpheus/gba/audio/gba_max_mod_sfx.hpp
            morpheus/gba/gfx/sprite.hpp
            morpheus/gba/gfx/sprite.cpp
            morpheus/gba/gfx/sprite_4_bpp.hpp
            morpheus/gba/gfx/sprite_4_bpp.cpp
            morpheus/gba/gfx/sprite_8_bpp.hpp
            morpheus/gba/gfx/sprite_8_bpp.cpp
            morpheus/gba/gfx/tiled_background.hpp
            morpheus/gba/gfx/tiled_background.cpp
            morpheus/gba/gfx/gba_window.hpp
            morpheus/gba/gfx/gba_window.cpp)

    message(STATUS "Configuring for the Game Boy Advance...")

    include_directories(morpheus PRIVATE ${BASE_INCLUDE_DIRS} /opt/devkitpro/libtonc/include
                        /opt/devkitpro/libgba/include/)
    link_directories(${DEVKITPRO}/libtonc/lib ${DEVKITPRO}/libgba/lib)
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libnds_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9")
    set(CMAKE_EXE_LINKER_FLAGS "-specs=ds_arm9.specs -g")

    add_compile_definitions(_NDS)

    list(APPEND MORPHEUS_SOURCE_FILES
            morpheus/nds/nds_controllers.cpp
            morpheus/nds/nds_controllers.hpp
            morpheus/nds/nds_main_loop.hpp
            morpheus/nds/nds_main_loop.cpp
            morpheus/nds/audio/nds_max_mod_music.hpp
            morpheus/nds/audio/nds_max_mod_sfx.hpp
            morpheus/nds/gfx/sprite.hpp
            morpheus/nds/gfx/sprite.cpp
            morpheus/nds/gfx/sprite_4_bpp.hpp
            morpheus/nds/gfx/sprite_4_bpp.cpp
            morpheus/nds/gfx/sprite_8_bpp.hpp
            morpheus/nds/gfx/sprite_8_bpp.cpp
            morpheus/nds/gfx/text.hpp
            morpheus/nds/gfx/text.cpp
            morpheus/nds/gfx/tiled_background.hpp
            morpheus/nds/gfx/tiled_background.cpp
            morpheus/nds/gfx/tiled_background_4_bpp.hpp
            morpheus/nds/gfx/tiled_background_4_bpp.cpp
            morpheus/nds/gfx/tiled_background_8_bpp.hpp
            morpheus/nds/gfx/tiled_background_8_bpp.cpp
            morpheus/nds/gfx/nds_window.cpp
            morpheus/nds/gfx/nds_window.hpp)

    message(STATUS "Configuring for the Nintendo DS...")

    include_directories(morpheus PRIVATE ${BASE_INCLUDE_DIRS} /opt/devkitpro/libnds/include)
    link_directories(${DEVKITPRO}/libnds/lib)
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

add_library(morpheus ${MORPHEUS_SOURCE_FILES})

if(WIN32)
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
else()
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
endif()

if("$ENV{BUILD_TESTS}")
    if(platform_lower STREQUAL "gba")
        set(PNG4_FILES "../tests/gba/input_test/test4.png")
    elseif(platform_lower STREQUAL "nds")
        set(PNG4_FILES "../tests/nds/input_test/test4.png")
    endif()

    set(PNG4_IMAGE_SIZES "32X32")

    if(platform_lower STREQUAL "gba")
        set(PNG8_FILES "../tests/gba/input_test/test8.png" CACHE STRING
                "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
        set(PNG8_IMAGE_SIZES "32X32")
    elseif(platform_lower STREQUAL "nds")
        set(PNG8_FILES "../tests/nds/input_test/test8.png" "../tests/nds/input_test/testt.png" CACHE STRING
                "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
        set(PNG8_IMAGE_SIZES "32X32" "32X32")
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

    execute_grit_sprites(PNG4_FILES PNG4_IMAGE_SIZES TRUE PNG4_OUTPUT_FILES)
    execute_grit_sprites(PNG8_FILES PNG8_IMAGE_SIZES FALSE)
    execute_grit_tilemap("../tests/tileset_test/region_map2.png" FALSE 3)

    foreach(png4_file ${PNG4_FILES})
        get_filename_component(png4_file ${png4_file} NAME_WLE)

        list(APPEND PNG4_OUTPUT_FILES ${png4_file}.o)
    endforeach()

    foreach(png8_file ${PNG8_FILES})
        get_filename_component(png8_file ${png8_file} NAME_WLE)

        list(APPEND PNG8_OUTPUT_FILES ${png8_file}.o)
    endforeach()

    #message(STATUS "${PNG8_OUTPUT_FILES}")

    list(APPEND maxmod_soundbank_list "${CMAKE_CURRENT_SOURCE_DIR}/tests/maxmod_test/examplesfx.wav")

    if(platform_lower STREQUAL "gba")
        list(APPEND maxmod_soundbank_list "${CMAKE_CURRENT_SOURCE_DIR}/tests/maxmod_test/example.it")

        message(STATUS ${maxmod_soundbank_list})

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

        project(morpheus-gba-input-test)

        link_directories(morpheus-gba-input-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-input-test.elf tests/gba/input_test/gba_input_test.cpp ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})
        add_gba_executable(morpheus-gba-input-test.elf)

        target_link_libraries(morpheus-gba-input-test.elf morpheus mm tonc)


        project(morpheus-gba-sram-test)

        link_directories(morpheus-gba-sram-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-sram-test.elf tests/gba/sram_test/gba_sram_test.cpp
                       ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o)
        add_gba_executable(morpheus-gba-sram-test.elf)

        target_link_libraries(morpheus-gba-sram-test.elf morpheus mm tonc)


        project(morpheus-gba-tileset-test)

        link_directories(morpheus-gba-tileset-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

        convert_tilemap_bin_image_file("tests/tileset_test/region_map.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                                       "tests/tileset_test/region_map.png" 8)

        add_executable(morpheus-gba-tileset-test.elf tests/tileset_test/tileset_test.cpp
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o"
                "${CMAKE_CURRENT_BINARY_DIR}/brin.c" "${CMAKE_CURRENT_BINARY_DIR}/brin.h")
        add_gba_executable(morpheus-gba-tileset-test.elf)

        target_link_libraries(morpheus-gba-tileset-test.elf morpheus tonc)


        project(morpheus-gba-maxmod-test)

        message(STATUS ${maxmod_soundbank_list})

        generate_maxmod_soundbank(true "soundbank" maxmod_soundbank_list)

        link_directories(morpheus-gba-maxmod-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib $ENV{DEVKITPRO}/libgba/lib)

        add_executable(morpheus-gba-maxmod-test.elf tests/maxmod_test/maxmod_test.cpp
                       ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o)
        add_gba_executable(morpheus-gba-maxmod-test.elf)

        target_link_libraries(morpheus-gba-maxmod-test.elf morpheus mm tonc)


        project(morpheus-gba-gfx-effects-test)

        set(GFX_EFFECT_PNG8_FILES "../tests/gba/input_test/test8.png")

        # skipping the grit step cause the .o rule for test8 already exists
        # skipping the tilemap conversion step cause the .o rule for region_map already exists

        foreach(png8_file ${GFX_EFFECT_PNG8_FILES})
            get_filename_component(png8_file ${png8_file} NAME_WLE)

            list(APPEND GFX_EFFECT_PNG8_OUTPUT_FILES ${png8_file}.o)
        endforeach()

        link_directories(morpheus-gba-gfx-effects-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib $ENV{DEVKITPRO}/libgba/lib)

        convert_tilemap_bin_image_file("tests/gfx_effects_test/region_map_window.bin" ${CMAKE_CURRENT_BINARY_DIR}
                32 32 0 "tests/gfx_effects_test/region_map.png" 8)

        add_executable(morpheus-gba-gfx-effects-test.elf tests/gfx_effects_test/gfx_effects_test.cpp
                       ${GFX_EFFECT_PNG8_OUTPUT_FILES} "${CMAKE_CURRENT_BINARY_DIR}/region_map.c"
                       "${CMAKE_CURRENT_BINARY_DIR}/region_map_window.c")
        add_gba_executable(morpheus-gba-gfx-effects-test.elf)

        target_link_libraries(morpheus-gba-gfx-effects-test.elf morpheus mm tonc)
    elseif(platform_lower STREQUAL "nds")
        list(APPEND maxmod_soundbank_list "${CMAKE_CURRENT_SOURCE_DIR}/tests/maxmod_test/example.it")

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

        project(morpheus-nds-input-test)

        set(NDS_PCX_FILES "../tests/nds/input_test/test8.pcx" CACHE STRING
                "PCX Files used by test code (paths are relative to cmake build dir)")

        link_directories(morpheus-nds-input-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        foreach(pcx_file IN LISTS NDS_PCX_FILES)
            get_filename_component(pcx_file_underscores_no_path ${pcx_file} NAME)
            string(REPLACE "." "_" pcx_file_underscores_no_path ${pcx_file_underscores_no_path})

            file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${pcx_file_underscores_no_path}.h
                    "extern const uint8_t ${pcx_file_underscores_no_path}_end[];\n"
                    "extern const uint8_t ${pcx_file_underscores_no_path}[];\n"
                    "extern const uint32_t ${pcx_file_underscores_no_path}_size;")

            add_custom_command(OUTPUT ${pcx_file_underscores_no_path}.o
                    COMMAND ${BIN2S} ${pcx_file} > ${pcx_file_underscores_no_path}.s
                    COMMAND ${ASSEMBLER_TO_USE} ${pcx_file_underscores_no_path}.s
                    -o${pcx_file_underscores_no_path}.o
                    VERBATIM)

            list(APPEND NDS_PCX_OBJECTS "${pcx_file_underscores_no_path}.o")
        endforeach()

        add_executable(morpheus-nds-input-test.elf tests/nds/input_test/nds_input_test.cpp
                ${NDS_PCX_OBJECTS} ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})
        add_nds_executable(morpheus-nds-input-test.elf "tests/nds/input_test/ds_icon.bmp" "Morpheus Input Test"
                           "Morpheus engine" "")

        target_link_libraries(morpheus-nds-input-test.elf morpheus nds9)


        project(morpheus-nds-tileset-test)

        link_directories(morpheus-nds-tileset-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

        convert_tilemap_bin_image_file("tests/tileset_test/region_map.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                "tests/tileset_test/region_map.png" 8)

        add_executable(morpheus-nds-tileset-test.elf tests/tileset_test/tileset_test.cpp
                "${CMAKE_CURRENT_BINARY_DIR}/custom1map.c" "${CMAKE_CURRENT_BINARY_DIR}/custom1map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o"
                "${CMAKE_CURRENT_BINARY_DIR}/brin.c" "${CMAKE_CURRENT_BINARY_DIR}/brin.h")
        add_nds_executable(morpheus-nds-tileset-test.elf "tests/nds/input_test/ds_icon.bmp" "Morpheus Tileset Test"
                           "Morpheus engine" "")

        target_link_libraries(morpheus-nds-tileset-test.elf morpheus nds9)


        project(morpheus-nds-extended-palette-test)

        convert_tilemap_bin_image_file("tests/nds/extended_palette_test/custom1map.bin" ${CMAKE_CURRENT_BINARY_DIR}
                64 64 10 "tests/nds/extended_palette_test/custom1map.png" 8)

        link_directories(morpheus-nds-extended-palette-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-extended-palette-test.elf
                tests/nds/extended_palette_test/nds_extended_palette_test.cpp
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/custom1map.c" "${CMAKE_CURRENT_BINARY_DIR}/custom1map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o"
                "${CMAKE_CURRENT_BINARY_DIR}/brin.c" "${CMAKE_CURRENT_BINARY_DIR}/brin.h")
        add_nds_executable(morpheus-nds-extended-palette-test.elf "tests/nds/input_test/ds_icon.bmp"
                "Morpheus ExtPalette Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-extended-palette-test.elf morpheus nds9)


        project(morpheus-nds-flash-test)

        link_directories(morpheus-nds-flash-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-flash-test.elf tests/nds/flash_test/nds_flash_test.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o)
        add_nds_executable(morpheus-nds-flash-test.elf "tests/nds/input_test/ds_icon.bmp"
                "Morpheus Flash Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-flash-test.elf nds9 mm9 morpheus)


        project(morpheus-nds-maxmod-test)

        link_directories(morpheus-nds-maxmod-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        generate_maxmod_soundbank(false "soundbank" maxmod_soundbank_list)

        add_executable(morpheus-nds-maxmod-test.elf
                tests/maxmod_test/maxmod_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o)
        add_nds_executable(morpheus-nds-maxmod-test.elf "tests/nds/input_test/ds_icon.bmp"
                "Morpheus MaxMod Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-maxmod-test.elf morpheus nds9 mm9)

        project(morpheus-nds-gfx-effects-test)

        set(GFX_EFFECT_PNG8_FILES "../tests/gba/input_test/test8.png")

        # skipping the grit step cause the .o rule for test8 already exists
        # skipping the tilemap conversion step cause the .o rule for region_map already exists

        foreach(png8_file ${GFX_EFFECT_PNG8_FILES})
            get_filename_component(png8_file ${png8_file} NAME_WLE)

            list(APPEND GFX_EFFECT_PNG8_OUTPUT_FILES ${png8_file}.o)
        endforeach()

        link_directories(morpheus-nds-gfx-effects-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-gfx-effects-test.elf
                tests/gfx_effects_test/gfx_effects_test.cpp ${GFX_EFFECT_PNG8_OUTPUT_FILES}
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c")
        add_nds_executable(morpheus-nds-gfx-effects-test.elf "tests/nds/input_test/ds_icon.bmp"
                "Morpheus GFX Effect Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-gfx-effects-test.elf morpheus nds9 mm9)
    endif()
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
