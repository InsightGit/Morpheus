# Parts of this cmake file orignate from Xtansia's fork of 3ds-cmake:
# https://github.com/Xtansia/3ds-cmake/

cmake_minimum_required(VERSION 3.16)

string(TOLOWER "$ENV{PLATFORM}" platform_lower)

if(WIN32)
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as.exe")
else()
    set(ASSEMBLER_TO_USE "$ENV{DEVKITARM}/bin/arm-none-eabi-as")
endif()


set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "-g -Wall -O2 ${ARCH_FLAGS} -ffast-math -fno-strict-aliasing -fno-rtti -fno-exceptions")


if(platform_lower STREQUAL "gba")
    set(PNG4_FILES "../tests/gba/test4.png")
elseif(platform_lower STREQUAL "nds")
    set(PNG4_FILES "../tests/nds/test4.png")
endif()

if(platform_lower STREQUAL "gba")
    set(PNG8_FILES "../tests/gba/test8.png" CACHE STRING
            "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
elseif(platform_lower STREQUAL "nds")
    set(PNG8_FILES "../tests/nds/test8.png" "../tests/nds/testt.png" CACHE STRING
            "8bpp PNG Files used by test code (paths are relative to cmake build dir)")
endif()

set(PNG4_IMAGE_SIZES "32X32" CACHE STRING "Image sizes of previously listed 4bpp PNG Files (widthXlength)")
set(PNG8_IMAGE_SIZES "32X32" "32X32" CACHE STRING
        "Image sizes of the previously listed 8bpp PNG files (widthXlength)")

if(NOT GRIT)
    # message(STATUS "Looking for bin2s...")
    find_program(GRIT grit ${DEVKITPRO}/tools)
    if(GRIT)
        message(STATUS "grit: ${GRIT} - found")
    else()
        message(FATAL_ERROR "grit - not found")
    endif()
endif()

function(execute_grit png_files png_image_sizes is_4bpp)
    set(png_files ${${png_files}})
    set(png_image_sizes ${${png_image_sizes}})

    message(STATUS "execute grit called with png_files: ${png_files}\n with png_image_sizes: ${png_image_sizes}\n and is_4bpp:${is_4bpp}")

    list(LENGTH png_files png_files_length)
    list(LENGTH png_image_sizes png_image_sizes_length)

    if(NOT png_files_length EQUAL png_image_sizes_length)
        message(FATAL_ERROR "The length of the png_files list (${png_files_length}) does "
                "NOT equal the length of the PNG4_IMAGE_SIZES list (${png_image_sizes_length})."
                " Are you missing a size or image path argument for a 4bpp PNG?")
    endif()

    if(is_4bpp)
        set(bpp_flag "4")
    else()
        set(bpp_flag "8")
    endif()

    math(EXPR png_files_length "${png_files_length} - 1")

    foreach(iteration RANGE ${png_files_length})
        list(GET png_files ${iteration} png_file)
        list(GET png_image_sizes ${iteration} png_image_size)

        string(TOUPPER ${png_image_size} png_image_size)

        string(REPLACE "X" ";" png_image_size ${png_image_size})

        list(LENGTH png_image_size png_image_size_len)

        if(NOT png_image_size_len EQUAL 2)
            message(FATAL_ERROR "Invalid image size: ${png_image_size}")
        endif()

        list(GET png_image_size 0 png_image_width)
        list(GET png_image_size 1 png_image_length)

        math(EXPR png_image_width_tiles "${png_image_width} / 8")
        math(EXPR png_image_length_tiles "${png_image_length} / 8")

        message(STATUS "${png_image_width_tiles}x${png_image_length_tiles}")

        get_filename_component(png_file_name_path ${png_file} NAME_WLE)

        add_custom_command(OUTPUT ${png_file_name_path}.o
                COMMAND ${GRIT} ${png_file} -gB${bpp_flag} -Mw ${png_image_width_tiles}
                -Mh ${png_image_length_tiles}
                COMMAND ${ASSEMBLER_TO_USE} ${png_file_name_path}.s -o${png_file_name_path}.o
                VERBATIM)
    endforeach()
endfunction()

execute_grit(PNG4_FILES PNG4_IMAGE_SIZES TRUE PNG4_OUTPUT_FILES)
execute_grit(PNG8_FILES PNG8_IMAGE_SIZES FALSE)

foreach(png4_file ${PNG4_FILES})
    get_filename_component(png4_file ${png4_file} NAME_WLE)

    list(APPEND PNG4_OUTPUT_FILES ${png4_file}.o)
endforeach()

foreach(png8_file ${PNG8_FILES})
    get_filename_component(png8_file ${png8_file} NAME_WLE)

    list(APPEND PNG8_OUTPUT_FILES ${png8_file}.o)
endforeach()

message(STATUS "${PNG8_OUTPUT_FILES}")

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
        set_target_properties(${target} PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
        set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.gba)
    endfunction()

    link_directories(morpheus-gba-test PRIVATE . $ENV{DEVKITPRO}/libtonc/lib)

    add_executable(morpheus-gba-test.elf tests/gba/gba_input_test.cpp ${PNG4_OUTPUT_FILES} ${PNG8_OUTPUT_FILES})
    add_gba_executable(morpheus-gba-test.elf)

    target_link_libraries(morpheus-gba-test.elf morpheus tonc)
elseif(platform_lower STREQUAL "nds")
    project(morpheus-nds-test)

    set(NDS_GAME_ICON  "tests/nds/ds_icon.bmp" CACHE STRING "NDS game icon location realitve to project location")
    set(NDS_TITLE "Morpheus NDS Test" CACHE STRING "NDS game title")
    set(NDS_SUBTITLE1 "hopefully this works" CACHE STRING "NDS game subtitle #1")
    set(NDS_SUBTITLE2 "hopefully this works ii" CACHE STRING "NDS game subtitle #2")
    set(NDS_PCX_FILES "../tests/nds/test8.pcx" CACHE STRING
            "PCX Files used by test code (paths are relative to cmake build dir)")

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

    add_executable(morpheus-nds-test.elf tests/nds/nds_input_test.cpp ${NDS_PCX_OBJECTS} ${PNG4_OUTPUT_FILES}
            ${PNG8_OUTPUT_FILES})
    add_nds_executable(morpheus-nds-test.elf)

    target_link_libraries(morpheus-nds-test.elf morpheus nds9)

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
