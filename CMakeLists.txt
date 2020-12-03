# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)
project(morpheus)

string(TOLOWER "$ENV{PLATFORM}" platform_lower)

set(MORPHEUS_SOURCE_FILES
    morpheus/core/main_loop.hpp
    morpheus/core/main_loop.cpp
    morpheus/core/node.hpp
    morpheus/core/uncopyable.hpp
    morpheus/core/gfx/palette_manager.hpp
    morpheus/core/gfx/palette_manager.cpp
    morpheus/core/gfx/vector_2.hpp)

if(platform_lower STREQUAL "gba")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/gba/gba_main_loop.hpp
         morpheus/gba/gba_main_loop.cpp
         morpheus/gba/gfx/sprite.hpp
         morpheus/gba/gfx/sprite.cpp)

    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ToolsGBA.cmake)

    message(STATUS "Compiling for the GBA...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libtonc/lib)
elseif(platform_lower STREQUAL "nds")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)

    list(APPEND MORPHEUS_SOURCE_FILES
         morpheus/nds/nds_main_loop.hpp
         morpheus/nds/nds_main_loop.cpp
         morpheus/nds/gfx/sprite.hpp
         morpheus/nds/gfx/sprite.cpp)

    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ToolsGBA.cmake)

    message(STATUS "Compiling for the NDS...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libnds/include)
    link_directories(${DEVKITPRO}/libnds/lib)
else()
    message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
endif()

set(CMAKE_CXX_STANDARD 11)

add_library(morpheus ${MORPHEUS_SOURCE_FILES})

if("$ENV{BUILD_TESTS}")
    if(platform_lower STREQUAL "gba")
        project(morpheus-gba-test)

        if(NOT GBAFIX)
            # message(STATUS "Looking for gbafix...")
            find_program(GBAFIX gbafix ${DEVKITARM}/bin)
            if(GBAFIX)
                message(STATUS "gbafix: ${GBAFIX} - found")
            else()
                message(FATAL_ERROR "gbafix - not found")
            endif()
        endif()

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
