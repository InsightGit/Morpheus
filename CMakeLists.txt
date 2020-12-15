# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

string(TOLOWER "$ENV{PLATFORM}" platform_lower)

set(MORPHEUS_PROJECT_NAME "morpheus-${platform_lower}")
set(MORPHEUS_SOURCE_FILES
    morpheus/core/main_loop.hpp
    morpheus/core/main_loop.cpp
    morpheus/core/node.hpp
    morpheus/core/uncopyable.hpp
    morpheus/core/gfx/palette_manager.hpp
    morpheus/core/gfx/palette_manager.cpp
    morpheus/core/gfx/vector_2.hpp)

project(morpheus)

#set( CMAKE_VERBOSE_MAKEFILE on )

if(platform_lower STREQUAL "gba")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libgba_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi -specs=gba.specs")

    add_compile_definitions(_GBA)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/gba/gba_main_loop.hpp
         morpheus/gba/gba_main_loop.cpp
         morpheus/gba/gfx/sprite.hpp
         morpheus/gba/gfx/sprite.cpp)

    message(STATUS "Configuring for the Game Boy Advance...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libtonc/lib)
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libnds_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9")
    set(CMAKE_EXE_LINKER_FLAGS "-specs=ds_arm9.specs -g")

    add_compile_definitions(_NDS)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/nds/nds_main_loop.hpp
         morpheus/nds/nds_main_loop.cpp
         morpheus/nds/gfx/sprite.hpp
         morpheus/nds/gfx/sprite.cpp
         morpheus/nds/node.hpp)

    message(STATUS "Configuring for the Nintendo DS...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libnds/include)
    link_directories(${DEVKITPRO}/libnds/lib)
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

add_library(morpheus ${MORPHEUS_SOURCE_FILES})

if(WIN32)
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
else()
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
endif()

if("$ENV{BUILD_TESTS}")
    if(platform_lower STREQUAL "gba")
        project(morpheus-gba-test)

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

        function(add_gba_executable target)
            get_filename_component(target_name ${target} NAME_WE)
            add_custom_target(${target_name}.gba ALL SOURCES
                    COMMAND ${OBJCOPY} -v -O binary ${target} ${target_name}.gba
                    COMMAND ${GBAFIX} ${target_name}.gba
                    DEPENDS ${target}
                    VERBATIM
                    )
            set_target_properties(${target} PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=gba.specs")
            set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.gba)
        endfunction()

        #string(CONCAT MULTI "$ENV{DEVKITARM}" "/arm-none-eabi/lib/")

        #include_directories(morpheus-gba-test PRIVATE . ${MULTI})
        link_directories(morpheus-gba-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

        add_executable(morpheus-gba-test.elf tests/gba/gba_input_test.cpp tests/gba/testconfetti.s)
        add_gba_executable(morpheus-gba-test.elf)

        target_link_libraries(morpheus-gba-test.elf morpheus tonc)
    elseif(platform_lower STREQUAL "nds")
        project(morpheus-nds-test)

        set(NDS_GAME_ICON  "tests/nds/ds_icon.bmp" CACHE STRING "NDS game icon location realitve to project location")
        set(NDS_TITLE "Morpheus NDS Test" CACHE STRING "NDS game title")
        set(NDS_SUBTITLE1 "hopefully this works" CACHE STRING "NDS game subtitle #1")
        set(NDS_SUBTITLE2 "hopefully this works ii" CACHE STRING "NDS game subtitle #2")
        set(NDS_PCX_FILES "../tests/nds/test8.pcx" CACHE STRING
                "PCX Files used by code (paths are relative to cmake build dir)")

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

        #message(STATUS ${NDS_PCX_FILES})

        function(add_nds_executable target)
            get_filename_component(target_name ${target} NAME_WE)
            add_custom_target(${target_name}.nds ALL SOURCES
                    COMMAND ${NDSTOOL} -c ${target_name}.nds -9 ${target} -b ${CMAKE_CURRENT_LIST_DIR}/${NDS_GAME_ICON}
                            "${NDS_TITLE};${NDS_SUBTITLE1};${NDS_SUBTITLE2}"
                    DEPENDS ${target}
                    VERBATIM
                    )
            #set_target_properties(${target} PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
            set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.nds)
        endfunction()

        link_directories(morpheus-gba-test PRIVATE . $ENV{DEVKITPRO}/libnds/lib)

        foreach(pcx_file IN LISTS NDS_PCX_FILES)
            get_filename_component(pcx_file_underscores_no_path ${pcx_file} NAME)
            string(REPLACE "." "_" pcx_file_underscores_no_path ${pcx_file_underscores_no_path})

            get_filename_component(pcx_dir ${pcx_file} DIRECTORY)
            set(pcx_file_underscores ${pcx_dir}/${pcx_file_underscores_no_path})

            string(REPLACE "../" "" pcx_file_underscores ${pcx_file_underscores})

            file(WRITE ${pcx_file_underscores}.h
                 "extern const uint8_t ${pcx_file_underscores_no_path}_end[];\n"
                 "extern const uint8_t ${pcx_file_underscores_no_path}[];\n"
                 "extern const uint32_t ${pcx_file_underscores_no_path}_size;")

            add_custom_command(OUTPUT ${pcx_file}.o
                               COMMAND ${BIN2S} ${pcx_file} > ${pcx_file}.s
                               COMMAND ${ASSEMBLER_TO_USE} "${pcx_file}.s" -o${pcx_file}.o
                               VERBATIM)

            list(APPEND NDS_PCX_OBJECTS "${pcx_file}.o")
        endforeach()

        add_executable(morpheus-nds-test.elf tests/nds/nds_input_test.cpp ${NDS_PCX_OBJECTS})
        add_nds_executable(morpheus-nds-test.elf)

        target_link_libraries(morpheus-nds-test.elf morpheus nds9)

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
