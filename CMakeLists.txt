if("$ENV{PLATFORM}" STREQUAL "gba")
    set(CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/DevkitArmGBA.cmake)

    include(ToolsGBA)

    include_directories(morpheus PRIVATE . /opt/devkitpro/libtonc/include)
    link_directories(${DEVKITPRO}/libgba/lib)
endif()


cmake_minimum_required(VERSION 3.16)
project(morpheus)

set(CMAKE_CXX_STANDARD 11)

add_library(morpheus core/main_loop.hpp core/uncopyable.hpp core/node.hpp gba/gfx/sprite.cpp gba/gfx/sprite.hpp gba/gba_main_loop.cpp gba/gba_main_loop.hpp core/gfx/vector_2.hpp core/input_event.hpp tests/gba_input_test.cpp gba/gba.hpp core/core.hpp)

if("$ENV{BUILD_TESTS}")
    if("$ENV{PLATFORM}" STREQUAL "gba")
        project(morpheus-gba-test)

        LIST(APPEND EXTRA_DATA_FILES
                
        )

        add_executable(morpheus-gba-test.elf tests/gba_input_test.cpp)
        add_gba_executable(morpheus-gba-test.elf)

        target_link_libraries(morpheus-gba-test.elf gba mm morpheus)
    endif()
endif()