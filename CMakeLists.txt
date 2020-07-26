cmake_minimum_required(VERSION 3.16)
project(morpheus)

set(CMAKE_CXX_STANDARD 11)

add_executable(morpheus main.cpp core/main_loop.hpp core/uncopyable.hpp core/node.hpp gba/gfx/sprite.cpp gba/gfx/sprite.hpp gba/gba_main_loop.cpp gba/gba_main_loop.hpp)

#if($ENV{PLATFORM} STREQUAL "gba")

target_include_directories(morpheus PRIVATE . /opt/devkitpro/libgba/include)#$ENV{PLATFORM_INCLUDE_PATH})