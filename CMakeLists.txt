# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

include(cmake/morpheus_build_utils.cmake)
string(TOLOWER "$ENV{PLATFORM}" platform_lower)

if(WIN32)
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
else()
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
endif()

if(NOT GRIT)
    # message(STATUS "Looking for bin2s...")
    find_program(GRIT grit $ENV{DEVKITPRO}/tools)
    if(GRIT)
        message(STATUS "grit: ${GRIT} - found")
    else()
        message(FATAL_ERROR "grit - not found")
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

execute_grit_1bpp_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/sys8.png)

set(MORPHEUS_PROJECT_NAME "morpheus-${platform_lower}")
set(MORPHEUS_SOURCE_FILES
        include/morpheus/core/controllers.hpp
        src/morpheus/core/controllers.cpp
        include/morpheus/core/main_loop.hpp
        src/morpheus/core/main_loop.cpp
        include/morpheus/core/control_reciever.hpp
        include/morpheus/core/save_manager.hpp
        include/morpheus/core/uncopyable.hpp
        include/morpheus/core/audio/max_mod_music.hpp
        src/morpheus/core/audio/max_mod_music.cpp
        include/morpheus/core/audio/max_mod_sfx.hpp
        src/morpheus/core/audio/max_mod_sfx.cpp
        src/morpheus/core/gfx/asm/tiled_background_base.s
        src/morpheus/core/gfx/sprite_base.cpp
        include/morpheus/core/gfx/sprite_base.hpp
        src/morpheus/core/gfx/text_base.cpp
        include/morpheus/core/gfx/text_base.hpp
        include/morpheus/core/gfx/streaming_background_base.hpp
        src/morpheus/core/gfx/streaming_background_base.cpp
        include/morpheus/core/gfx/tiled_background_base.hpp
        src/morpheus/core/gfx/tiled_background_base.cpp
        include/morpheus/core/gfx/vector_2.hpp
        include/morpheus/core/gfx/window.hpp
        src/morpheus/core/gfx/window.cpp
        include/morpheus/utils.hpp
        include/morpheus/core/communication_channel.hpp
        include/morpheus/core/gfx/assets/sys8.h
        src/morpheus/core/gfx/assets/sys8.c
        src/morpheus/core/gfx/animation_frame.cpp
        include/morpheus/core/gfx/animation_frame.hpp
        src/morpheus/core/gfx/animation_smoothing_attributes.cpp
        include/morpheus/core/gfx/animation_smoothing_attributes.hpp
        include/morpheus/core/gfx/font.hpp)

project(morpheus)

#set( CMAKE_VERBOSE_MAKEFILE on )

set(BASE_INCLUDE_DIRS include/ ${CMAKE_CURRENT_BINARY_DIR})

if($ENV{BUILD_DOCS})
    if(WIN32)
        find_program(DOXYGEN doxygen)
        find_program(PYTHON3 python)
    else()
        find_program(DOXYGEN doxygen)
        find_program(PYTHON3 python3)
    endif()

    if(NOT PYTHON3)
        message(FATAL_ERROR "python3 - not found")
    endif()

    add_custom_target(Documentation ALL
            COMMENT "Building Documentation"
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/docs && ${DOXYGEN}
            COMMAND ${PYTHON3}
            ${CMAKE_CURRENT_SOURCE_DIR}/buildtools/generate_breathe_rst/generate_breathe_rst.py
            ${CMAKE_CURRENT_SOURCE_DIR}/include/morpheus morpheus ${CMAKE_CURRENT_SOURCE_DIR}/docs
            COMMAND cd ${CMAKE_CURRENT_SOURCE_DIR}/docs/ && make html)
endif()

if(platform_lower STREQUAL "gba")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libgba_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")

    add_compile_definitions(_GBA)

    list(APPEND MORPHEUS_SOURCE_FILES
            include/morpheus/gba/gba_controllers.hpp
            src/morpheus/gba/gba_controllers.cpp
            include/morpheus/gba/gba_main_loop.hpp
            src/morpheus/gba/gba_main_loop.cpp
            include/morpheus/gba/audio/gba_max_mod_music.hpp
            include/morpheus/gba/audio/gba_max_mod_sfx.hpp
            include/morpheus/gba/gfx/sprite.hpp
            src/morpheus/gba/gfx/sprite.cpp
            include/morpheus/gba/gfx/sprite_4_bpp.hpp
            src/morpheus/gba/gfx/sprite_4_bpp.cpp
            include/morpheus/gba/gfx/sprite_8_bpp.hpp
            src/morpheus/gba/gfx/sprite_8_bpp.cpp
            src/morpheus/gba/gfx/text.cpp
            include/morpheus/gba/gfx/text.hpp
            include/morpheus/gba/gfx/tiled_background.hpp
            src/morpheus/gba/gfx/tiled_background.cpp
            include/morpheus/gba/gba_eeprom_save_manager.hpp
            src/morpheus/gba/gba_eeprom_save_manager.cpp
            include/morpheus/gba/gba_flash_save_manager.hpp
            src/morpheus/gba/gba_flash_save_manager.cpp
            include/morpheus/gba/gfx/gba_window.hpp
            src/morpheus/gba/gfx/gba_window.cpp
            src/morpheus/gba/serial_communication.cpp
            include/morpheus/gba/serial_communication.hpp
            include/morpheus/gba/gba_sram_save_manager.hpp
            src/morpheus/gba/gba_sram_save_manager.cpp
            include/morpheus/gba/gfx/gba_animation_frame.hpp
            src/morpheus/gba/gfx/gba_animation_frame.cpp)

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
            src/morpheus/nds/nds_controllers.cpp
            include/morpheus/nds/nds_controllers.hpp
            include/morpheus/nds/nds_main_loop.hpp
            src/morpheus/nds/nds_main_loop.cpp
            include/morpheus/nds/audio/nds_max_mod_music.hpp
            include/morpheus/nds/audio/nds_max_mod_sfx.hpp
            include/morpheus/nds/gfx/nds_animation_frame.hpp
            src/morpheus/nds/gfx/nds_animation_frame.cpp
            include/morpheus/nds/gfx/sprite.hpp
            src/morpheus/nds/gfx/sprite.cpp
            include/morpheus/nds/gfx/sprite_4_bpp.hpp
            src/morpheus/nds/gfx/sprite_4_bpp.cpp
            include/morpheus/nds/gfx/sprite_8_bpp.hpp
            src/morpheus/nds/gfx/sprite_8_bpp.cpp
            include/morpheus/nds/gfx/text.hpp
            src/morpheus/nds/gfx/text.cpp
            include/morpheus/nds/gfx/tiled_background.hpp
            src/morpheus/nds/gfx/tiled_background.cpp
            include/morpheus/nds/gfx/tiled_background_4_bpp.hpp
            src/morpheus/nds/gfx/tiled_background_4_bpp.cpp
            include/morpheus/nds/gfx/tiled_background_8_bpp.hpp
            src/morpheus/nds/gfx/tiled_background_8_bpp.cpp
            src/morpheus/nds/gfx/nds_window.cpp
            include/morpheus/nds/gfx/nds_window.hpp
            include/morpheus/nds/ds_flashcard_save_manager.hpp
            src/morpheus/nds/ds_flashcard_save_manager.cpp)

    message(STATUS "Configuring for the Nintendo DS...")

    include_directories(morpheus ${BASE_INCLUDE_DIRS} /opt/devkitpro/libnds/include)
    link_directories(${DEVKITPRO}/libnds/lib)
elseif($ENV{BUILD_DOCS})
    return()
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "../lib/")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-g -Wall -Werror -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

add_library(morpheus ${MORPHEUS_SOURCE_FILES})

if(platform_lower STREQUAL "gba")
    libgba_fat_patch()
    add_dependencies(morpheus libgba_fat_patch_target)
endif()

if("$ENV{BUILD_TESTS}")
    include_directories(${CMAKE_CURRENT_SOURCE_DIR}/assets)

    set(PNG4_FILES "../tests/input_test/test4.png")
    set(PNG4_IMAGE_SIZES "32X32")

    set(PNG8_FILES "../tests/input_test/test8.png")
    set(PNG8_IMAGE_SIZES "32X32")

    execute_grit_sprites(PNG4_FILES PNG4_IMAGE_SIZES TRUE PNG4_OUTPUT_FILES)
    execute_grit_sprites(PNG8_FILES PNG8_IMAGE_SIZES FALSE)
    execute_grit_tilemap("../tests/tileset_test/region_map2.png" FALSE 3 FALSE)

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

        add_executable(morpheus-gba-input-test.elf tests/input_test/input_test.cpp ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})
        add_gba_executable(morpheus-gba-input-test.elf)

        target_link_libraries(morpheus-gba-input-test.elf morpheus mm tonc)


        project(morpheus-gba-sram-test)

        link_directories(morpheus-gba-sram-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-sram-test.elf tests/save_test/save_test.cpp)
        add_gba_executable(morpheus-gba-sram-test.elf)

        target_link_libraries(morpheus-gba-sram-test.elf morpheus mm tonc)

        project(morpheus-gba-eeprom-test)

        link_directories(morpheus-gba-eeprom-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-eeprom-test.elf tests/save_test/save_test.cpp)
        add_gba_executable(morpheus-gba-eeprom-test.elf)

        target_link_libraries(morpheus-gba-eeprom-test.elf morpheus mm tonc)

        target_compile_definitions(morpheus-gba-eeprom-test.elf PUBLIC GBA_EEPROM_SAVE)

        project(morpheus-gba-eeprom-test)

        link_directories(morpheus-gba-flash-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-flash-test.elf tests/save_test/save_test.cpp)
        add_gba_executable(morpheus-gba-flash-test.elf)

        target_link_libraries(morpheus-gba-flash-test.elf morpheus mm tonc)

        target_compile_definitions(morpheus-gba-flash-test.elf PUBLIC GBA_FLASH_SAVE)

        project(morpheus-gba-tileset-test)

        link_directories(morpheus-gba-tileset-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

        convert_tilemap_bin_image_file("tests/tileset_test/region_map.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                                       "tests/tileset_test/region_map.png" 8 false)

        add_executable(morpheus-gba-tileset-test.elf tests/tileset_test/tileset_test.cpp tests/tileset_test/brin.c
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o")
        add_gba_executable(morpheus-gba-tileset-test.elf)

        target_link_libraries(morpheus-gba-tileset-test.elf morpheus mm tonc)

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
                32 32 0 "tests/gfx_effects_test/region_map.png" 8 false)

        add_executable(morpheus-gba-gfx-effects-test.elf tests/gfx_effects_test/gfx_effects_test.cpp
                       ${GFX_EFFECT_PNG8_OUTPUT_FILES} "${CMAKE_CURRENT_BINARY_DIR}/region_map.c"
                       "${CMAKE_CURRENT_BINARY_DIR}/region_map_window.c")
        add_gba_executable(morpheus-gba-gfx-effects-test.elf)

        target_link_libraries(morpheus-gba-gfx-effects-test.elf morpheus mm tonc)

        project(morpheus-gba-animation-test)

        set(GFX_EFFECT_PNG8_FILES "../tests/gba/input_test/test8.png")

        # skipping the grit step cause the .o rule for test8 already exists
        # skipping the tilemap conversion step cause the .o rule for region_map already exists

        foreach(png8_file ${GFX_EFFECT_PNG8_FILES})
            get_filename_component(png8_file ${png8_file} NAME_WLE)

            list(APPEND GFX_EFFECT_PNG8_OUTPUT_FILES ${png8_file}.o)
        endforeach()

        link_directories(morpheus-gba-animation-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib $ENV{DEVKITPRO}/libgba/lib)

        convert_tilemap_bin_image_file("tests/gfx_effects_test/region_map_window.bin" ${CMAKE_CURRENT_BINARY_DIR}
                32 32 0 "tests/gfx_effects_test/region_map.png" 8 false)

        add_executable(morpheus-gba-animation-test.elf tests/animation_test/animation_test.cpp
                ${GFX_EFFECT_PNG8_OUTPUT_FILES} "${CMAKE_CURRENT_BINARY_DIR}/region_map.c"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map_window.c" tests/animation_test/test_animation.hpp)
        add_gba_executable(morpheus-gba-animation-test.elf)

        target_link_libraries(morpheus-gba-animation-test.elf morpheus mm tonc)

        project(morpheus-gba-serial-test)

        link_directories(morpheus-gba-serial-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-serial-test.elf tests/gba/serial_test/gba_serial_test.cpp)
        add_gba_executable(morpheus-gba-serial-test.elf)

        target_link_libraries(morpheus-gba-serial-test.elf morpheus mm tonc)

        project(morpheus-gba-affine-test)

        link_directories(morpheus-gba-affine-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        convert_tilemap_bin_image_file("tests/affine_test/region_map_affine_128.bin" ${CMAKE_CURRENT_BINARY_DIR}
                128 128 0 "tests/affine_test/region_map_affine.png" true true)

        add_executable(morpheus-gba-affine-test.elf tests/affine_test/affine_test.cpp
                       "${CMAKE_CURRENT_BINARY_DIR}/region_map_affine_128.c"
                       "${CMAKE_CURRENT_BINARY_DIR}/region_map_affine_128.h" ${PNG4_OUTPUT_FILES})
        add_gba_executable(morpheus-gba-affine-test.elf)

        target_link_libraries(morpheus-gba-affine-test.elf morpheus mm tonc)

        project(morpheus-gba-custom-font-test)

        link_directories(morpheus-gba-custom-font-test PRIVATE . $ENV{DEVKITPRO}/libgba/lib $ENV{DEVKITPRO}/libtonc/lib)

        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Stick-Regular.ttf jp 1 147 235 255 15 false
                      true)
        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Stick-Regular.ttf en 1 147 235 255 15 false
                      false)
        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Montserrat-Light.ttf en 4 147 235 255 15 false
                      false)
        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Roboto-Black.ttf en 4 147 235 255 15 false
                      false)

        add_executable(morpheus-gba-custom-font-test.elf tests/custom_font_test/custom_font_test.cpp
                       ${CMAKE_CURRENT_BINARY_DIR}/Stick-Regular-en.c
                       #${CMAKE_CURRENT_BINARY_DIR}/Montserrat-Light-en.c
                       #${CMAKE_CURRENT_BINARY_DIR}/Roboto-Black-en.c
                       ${CMAKE_CURRENT_BINARY_DIR}/Stick-Regular-jp.c)
        add_gba_executable(morpheus-gba-custom-font-test.elf)

        target_link_libraries(morpheus-gba-custom-font-test.elf morpheus mm tonc fat gba)

        project(morpheus-gba-streaming-background-test)

        link_directories(morpheus-gba-streaming-background-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib
                $ENV{DEVKITPRO}/libgba/lib)

        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled.bin
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 128 128 0
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled.png
                                      false false false)
        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_256.bin
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 256 256 0
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_256.png
                                      false false false)
        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_512.bin
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 512 512 0
                                      ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_512.png
                                      false false false)

        add_executable(morpheus-gba-streaming-background-test.elf
                tests/streaming_background_test/streaming_background_test.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled.c ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled_256.c
                ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled_512.c)
        add_gba_executable(morpheus-gba-streaming-background-test.elf)

        target_link_libraries(morpheus-gba-streaming-background-test.elf morpheus mm tonc fat gba)
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

        project(morpheus-nds-animation-test)

        set(GFX_EFFECT_PNG8_FILES "../tests/gba/input_test/test8.png")

        # skipping the grit step cause the .o rule for test8 already exists
        # skipping the tilemap conversion step cause the .o rule for region_map already exists

        foreach(png8_file ${GFX_EFFECT_PNG8_FILES})
            get_filename_component(png8_file ${png8_file} NAME_WLE)

            list(APPEND GFX_EFFECT_PNG8_OUTPUT_FILES ${png8_file}.o)
        endforeach()

        link_directories(morpheus-nds-animation-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        convert_tilemap_bin_image_file("tests/gfx_effects_test/region_map_window.bin" ${CMAKE_CURRENT_BINARY_DIR}
                32 32 0 "tests/gfx_effects_test/region_map.png" 8 false)

        add_executable(morpheus-nds-animation-test.elf tests/animation_test/animation_test.cpp
                ${GFX_EFFECT_PNG8_OUTPUT_FILES} tests/animation_test/test_animation.hpp)
        add_nds_executable(morpheus-nds-animation-test.elf "tests/nds/ds_icon.bmp" "Morpheus Animation Test"
                           "Morpheus' Ani animation engine" "")

        target_link_libraries(morpheus-nds-animation-test.elf morpheus filesystem fat nds9 mm9)

        project(morpheus-nds-input-test)

        link_directories(morpheus-nds-input-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-input-test.elf tests/input_test/input_test.cpp
                       ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})
        add_nds_executable(morpheus-nds-input-test.elf "tests/nds/ds_icon.bmp" "Morpheus Input Test"
                           "Morpheus engine" "")

        target_link_libraries(morpheus-nds-input-test.elf morpheus filesystem fat nds9)

        project(morpheus-nds-tileset-test)

        link_directories(morpheus-nds-tileset-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

        convert_tilemap_bin_image_file("tests/tileset_test/region_map.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                "tests/tileset_test/region_map.png" 8 false)

        add_executable(morpheus-nds-tileset-test.elf tests/tileset_test/tileset_test.cpp tests/tileset_test/brin.c
                "${CMAKE_CURRENT_BINARY_DIR}/custom1map.c" "${CMAKE_CURRENT_BINARY_DIR}/custom1map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o")
                #"${CMAKE_CURRENT_BINARY_DIR}/brin.c" "${CMAKE_CURRENT_BINARY_DIR}/brin.h")
        add_nds_executable(morpheus-nds-tileset-test.elf "tests/nds/ds_icon.bmp" "Morpheus Tileset Test"
                           "Morpheus engine" "")

        target_link_libraries(morpheus-nds-tileset-test.elf morpheus filesystem fat nds9)


        project(morpheus-nds-extended-palette-test)

        convert_tilemap_bin_image_file("tests/nds/extended_palette_test/custom1map.bin" ${CMAKE_CURRENT_BINARY_DIR}
                64 64 10 "tests/nds/extended_palette_test/custom1map.png" 8 false)

        link_directories(morpheus-nds-extended-palette-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-extended-palette-test.elf
                tests/nds/extended_palette_test/nds_extended_palette_test.cpp
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/custom1map.c" "${CMAKE_CURRENT_BINARY_DIR}/custom1map.h"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map2.o")
                #"${CMAKE_CURRENT_BINARY_DIR}/brin.c" "${CMAKE_CURRENT_BINARY_DIR}/brin.h")
        add_nds_executable(morpheus-nds-extended-palette-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus ExtPalette Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-extended-palette-test.elf morpheus filesystem fat nds9)

        project(morpheus-nds-save-test)

        link_directories(morpheus-nds-save-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-save-test.elf tests/save_test/save_test.cpp)
        add_nds_executable_with_nitrofs_dir(morpheus-nds-save-test.elf "tests/nds/ds_icon.bmp"
                                            "Morpheus DS Save Test" "Morpheus engine" ""
                                            "${CMAKE_CURRENT_SOURCE_DIR}/tests/save_test/nitrofs/")

        target_link_libraries(morpheus-nds-save-test.elf morpheus mm9 filesystem fat nds9)


        project(morpheus-nds-maxmod-test)

        link_directories(morpheus-nds-maxmod-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        generate_maxmod_soundbank(false "soundbank" maxmod_soundbank_list)

        add_executable(morpheus-nds-maxmod-test.elf
                tests/maxmod_test/maxmod_test.cpp ${CMAKE_CURRENT_BINARY_DIR}/soundbank.bin.o)
        add_nds_executable(morpheus-nds-maxmod-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus MaxMod Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-maxmod-test.elf morpheus filesystem fat nds9 mm9)

        project(morpheus-nds-gfx-effects-test)

        set(GFX_EFFECT_PNG8_FILES "../tests/gba/input_test/test8.png")

        # skipping the grit step cause the .o rule for test8 already exists
        # skipping the tilemap conversion step cause the .o rule for region_map already exists

        foreach(png8_file ${GFX_EFFECT_PNG8_FILES})
            get_filename_component(png8_file ${png8_file} NAME_WLE)

            list(APPEND GFX_EFFECT_PNG8_OUTPUT_FILES ${png8_file}.o)
        endforeach()

        link_directories(morpheus-nds-gfx-effects-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        convert_tilemap_bin_image_file("tests/gfx_effects_test/region_map_window.bin" ${CMAKE_CURRENT_BINARY_DIR}
                32 32 0 "tests/gfx_effects_test/region_map.png" 8 false)

        add_executable(morpheus-nds-gfx-effects-test.elf
                tests/gfx_effects_test/gfx_effects_test.cpp ${GFX_EFFECT_PNG8_OUTPUT_FILES}
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map_window.c"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map.h" "${CMAKE_CURRENT_BINARY_DIR}/region_map_window.h")
        add_nds_executable(morpheus-nds-gfx-effects-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus GFX Effect Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-gfx-effects-test.elf morpheus filesystem fat nds9 mm9)

        project(morpheus-nds-affine-test)

        execute_grit_tilemap("../tests/affine_test/region_map_affine2.png" FALSE 3 TRUE)
        convert_tilemap_bin_image_file("tests/affine_test/region_map_affine.bin" ${CMAKE_CURRENT_BINARY_DIR} 32 32 0
                "tests/affine_test/region_map_affine.png" 8 true)
        convert_tilemap_bin_image_file("tests/affine_test/region_map_affine_128.bin" ${CMAKE_CURRENT_BINARY_DIR}
                128 128 0 "tests/affine_test/region_map_affine.png" true true)

        link_directories(morpheus-nds-affine-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        add_executable(morpheus-nds-affine-test.elf tests/affine_test/affine_test.cpp ${PNG4_OUTPUT_FILES}
                "${CMAKE_CURRENT_BINARY_DIR}/region_map_affine.c" "${CMAKE_CURRENT_BINARY_DIR}/region_map_affine_128.c"
                "${CMAKE_CURRENT_BINARY_DIR}/region_map_affine2.o")
        add_nds_executable(morpheus-nds-affine-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus Affine Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-affine-test.elf morpheus filesystem fat nds9 mm9 )

        project(morpheus-nds-custom-font-test)

        link_directories(morpheus-nds-custom-font-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Stick-Regular.ttf jp 1 147 235 255 15 false
                      true)
        generate_font(${CMAKE_CURRENT_SOURCE_DIR}/assets/test_fonts/Stick-Regular.ttf en 1 147 235 255 15 false
                      false)

        add_executable(morpheus-nds-custom-font-test.elf tests/custom_font_test/custom_font_test.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/Stick-Regular-jp.c
                ${CMAKE_CURRENT_BINARY_DIR}/Stick-Regular-en.c)
        add_nds_executable(morpheus-nds-custom-font-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus Custom Font Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-custom-font-test.elf morpheus filesystem fat nds9 mm9)

        project(morpheus-nds-streaming-background-test)

        link_directories(morpheus-nds-streaming-background-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled.bin
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 128 128 0
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled.png
                false false false)
        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_256.bin
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 256 256 0
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_256.png
                false false false)
        generate_streaming_background(${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_512.bin
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/asset_dir 512 512 0
                ${CMAKE_CURRENT_SOURCE_DIR}/tests/streaming_background_test/kakariko_tiled_512.png
                false false false)

        add_executable(morpheus-nds-streaming-background-test.elf
                tests/streaming_background_test/streaming_background_test.cpp
                ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled.c ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled_256.c
                ${CMAKE_CURRENT_BINARY_DIR}/kakariko_tiled_512.c)
        add_nds_executable(morpheus-nds-streaming-background-test.elf "tests/nds/ds_icon.bmp"
                "Morpheus Streaming Background Test" "Morpheus engine" "")

        target_link_libraries(morpheus-nds-streaming-background-test.elf morpheus filesystem fat mm9 nds9)
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

enable_language(ASM)

message(STATUS "set CMAKE_CXX_COMPILER to ${CMAKE_CXX_COMPILER}")
