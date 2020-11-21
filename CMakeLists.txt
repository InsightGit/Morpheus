cmake_minimum_required(VERSION 3.16)
project(morpheus)

string(TOLOWER "$ENV{PLATFORM}" platform_lower)

if(platform_lower STREQUAL "gba")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)

    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ToolsGBA.cmake)

    message(STATUS "Compiling for the GBA...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libgba/lib)
else()
    #message(FATAL_ERROR "Unrecognized platform (" $ENV{PLATFORM} ") specified in PLATFORM environment variable!")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)

    include(${CMAKE_CURRENT_LIST_DIR}/cmake/ToolsGBA.cmake)

    message(STATUS "Compiling for the GBA...")

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libgba/lib)
endif()

set(CMAKE_CXX_STANDARD 11)

add_library(morpheus core/main_loop.hpp core/uncopyable.hpp core/node.hpp gba/gfx/sprite.cpp gba/gfx/sprite.hpp gba/gba_main_loop.cpp gba/gba_main_loop.hpp core/gfx/vector_2.hpp core/input_event.hpp tests/gba_input_test.cpp gba/gba.hpp core/core.hpp)

#if("$ENV{BUILD_TESTS}")
    #if(platform_lower STREQUAL "gba")
        project(morpheus-gba-test)

        string(CONCAT MULTI "$ENV{DEVKITARM}" "/arm-none-eabi/lib/")

        include_directories(morpheus-gba-test PRIVATE . ${MULTI})
        link_directories(. ${DEVKITPRO}/libgba/lib)

        add_executable(morpheus-gba-test.elf tests/gba_input_test.cpp tests/test_image.cpp)
        add_gba_executable(morpheus-gba-test.elf)

        target_link_libraries(morpheus-gba-test.elf gba mm morpheus)
    #endif()
#endif()