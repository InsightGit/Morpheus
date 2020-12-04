# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

string(TOLOWER "$ENV{PLATFORM}" platform_lower)

message(STATUS ${platform_lower})

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

if(platform_lower STREQUAL "gba")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libgba_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-GBA-52)
    set(CMAKE_SYSTEM_PROCESSOR armv4t)

    set(ARCH_FLAGS "-mcpu=arm7tdmi -mtune=arm7tdmi")

    add_compile_definitions(_GBA)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/gba/gba_main_loop.hpp
         morpheus/gba/gba_main_loop.cpp
         morpheus/gba/gfx/sprite.hpp
         morpheus/gba/gfx/sprite.cpp)

    message(STATUS "Compiling for the GBA...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libtonc/lib)
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_STATIC_LIBRARY_PREFIX "libnds_")
    set(CMAKE_SYSTEM_NAME Generic)
    set(CMAKE_SYSTEM_VERSION DKA-NDS-52)
    set(CMAKE_SYSTEM_PROCESSOR armv5te)

    set(ARCH_FLAGS "-marm -mthumb-interwork -march=armv5te -mtune=arm946e-s -DARM9")

    add_compile_definitions(_NDS)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/nds/nds_main_loop.hpp
         morpheus/nds/nds_main_loop.cpp
         morpheus/nds/gfx/sprite.hpp
         morpheus/nds/gfx/sprite.cpp)

    message(STATUS "Compiling for the NDS...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libnds/include)
    link_directories(${DEVKITPRO}/libnds/lib)
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")

add_library(morpheus ${MORPHEUS_SOURCE_FILES})

if("$ENV{BUILD_TESTS}")
    if(platform_lower STREQUAL "gba")
        project(morpheus-gba-test)

        if(NOT GBAFIX)
            # message(STATUS "Looking for gbafix...")
            find_program(GBAFIX gbafix $ENV{DEVKITARM}/bin)
            if(GBAFIX)
                message(STATUS "gbafix: ${GBAFIX} - found")
            else()
                message(FATAL_ERROR "gbafix - not found")
            endif()
        endif()

        if(NOT OBJCOPY)
            # message(STATUS "Looking for objcopy...")
            find_program(OBJCOPY arm-none-eabi-objcopy ${DEVKITARM}/bin)
            if(OBJCOPY)
                message(STATUS "objcopy: ${OBJCOPY} - found")
            else()
                message(WARNING "objcopy - not found")
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
        #link_directories(. ${DEVKITPRO}/libgba/lib)

        add_executable(morpheus-gba-test.elf tests/gba_input_test.cpp)
        add_gba_executable(morpheus-gba-test.elf)

        target_link_libraries(morpheus-gba-test.elf gba mm morpheus)
    elseif(platform_lower STREQUAL "nds")
        #
    endif()
endif()

if(WIN32)
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc.exe")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++.exe")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld.exe")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar.exe" CACHE STRING "")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe" CACHE STRING "")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm.exe" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib.exe" CACHE STRING "")
else()
    set(CMAKE_C_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc")
    set(CMAKE_CXX_COMPILER "$ENV{DEVKITARM}/bin/arm-none-eabi-g++")
    set(CMAKE_LINKER "$ENV{DEVKITARM}/bin/arm-none-eabi-ld")
    set(CMAKE_AR "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ar" CACHE STRING "")
    set(CMAKE_AS "$ENV{DEVKITARM}/bin/arm-none-eabi-as" CACHE STRING "")
    set(CMAKE_NM "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-nm" CACHE STRING "")
    set(CMAKE_RANLIB "$ENV{DEVKITARM}/bin/arm-none-eabi-gcc-ranlib" CACHE STRING "")
endif()
