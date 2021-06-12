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

function(add_nds_executable target game_icon title subtitle1 subtitle2)
    get_filename_component(target_name ${target} NAME_WE)
    add_custom_target(${target_name}.nds ALL SOURCES
            COMMAND ${NDSTOOL} -c ${target_name}.nds -9 ${target} -b ${CMAKE_CURRENT_SOURCE_DIR}/${game_icon}
            "${title};${subtitle1};${subtitle2}"
            DEPENDS ${target}
            VERBATIM
            )
    #set_target_properties(${target} PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.nds)
endfunction()

function(add_nds_executable_with_nitrofs_dir target game_icon title subtitle1 subtitle2 nitrofs_dir)
    get_filename_component(target_name ${target} NAME_WE)
    add_custom_target(${target_name}.nds ALL SOURCES
            COMMAND ${NDSTOOL} -c ${target_name}.nds -9 ${target} -b ${CMAKE_CURRENT_SOURCE_DIR}/${game_icon}
            "${title};${subtitle1};${subtitle2}" -d ${nitrofs_dir}
            DEPENDS ${target}
            VERBATIM
            )
    #set_target_properties(${target} PROPERTIES LINK_FLAGS "${CMAKE_EXE_LINKER_FLAGS}")
    set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${target_name}.nds)
endfunction()

function(execute_grit_sprites png_files png_image_sizes is_4bpp)
    set(png_files ${${png_files}})
    set(png_image_sizes ${${png_image_sizes}})

    #message(STATUS "execute grit called with png_files: ${png_files}\n with png_image_sizes: ${png_image_sizes}\n and is_4bpp:${is_4bpp}")

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

        #message(STATUS "${png_image_width_tiles}x${png_image_length_tiles}")

        get_filename_component(png_file_name_path ${png_file} NAME_WLE)

        add_custom_command(OUTPUT ${png_file_name_path}.o
                COMMAND ${GRIT} ${png_file} -gB${bpp_flag} -Mw ${png_image_width_tiles}
                -Mh ${png_image_length_tiles}
                COMMAND ${ASSEMBLER_TO_USE} ${png_file_name_path}.s -o${png_file_name_path}.o
                VERBATIM)
    endforeach()
endfunction()

function(execute_grit_tilemaps png_files is_4bpp palette_bank_num is_affine)
    foreach(png_file IN ${${png_files}})
        execute_grit_tilemap(png_file ${is_4bpp} ${palette_bank_num} ${is_affine})
    endforeach()
endfunction()


function(execute_grit_1bpp_font png_file)
    get_filename_component(png_file_name_path ${png_file} NAME_WLE)

    add_custom_command(OUTPUT ${png_file_name_path}.o ${png_file_name_path}.h
            COMMAND ${GRIT} ${png_file} -gB1 -tc
            COMMAND ${ASSEMBLER_TO_USE} ${CMAKE_CURRENT_BINARY_DIR}/${png_file_name_path}.s -o${png_file_name_path}.o
            VERBATIM)
endfunction()

function(execute_grit_4bpp_font png_file)
    get_filename_component(png_file_name_path ${png_file} NAME_WLE)

    add_custom_command(OUTPUT ${png_file_name_path}.o ${png_file_name_path}.h
            COMMAND ${GRIT} ${png_file} -gB4
            COMMAND ${ASSEMBLER_TO_USE} ${png_file_name_path}.s -o${png_file_name_path}.o
            VERBATIM)
endfunction()

function(execute_grit_tilemap png_file is_4bpp palette_bank_num is_affine)
    if(is_4bpp)
        set(bpp_flag "4")
        set(tile_map_flag "tpf")
    else()
        set(bpp_flag "8")
        set(tile_map_flag "tf")
    endif()

    if(is_affine)
        set(tile_map_flag "t")
    endif()

    get_filename_component(png_file_name_path ${png_file} NAME_WLE)

    add_custom_command(OUTPUT ${png_file_name_path}.o
            COMMAND ${GRIT} ${png_file} -gB${bpp_flag} -mR${tile_map_flag} -mp${palette_bank_num}
            COMMAND ${ASSEMBLER_TO_USE} ${png_file_name_path}.s -o${png_file_name_path}.o
            VERBATIM)
endfunction()

# TODO(Bobby): Update this function to match the working function below this function (convert_tilemap_bin_png_file)
function(convert_tilemap_bin_files build_dir palette_bank_num bin_files target_name_var)
    if(WIN32)
        find_program(PYTHON3 python)
    else()
        find_program(PYTHON3 python3)
    endif()

    if(NOT PYTHON3)
        message(FATAL_ERROR "python3 - not found")
    endif()

    foreach(bin_file IN ${${bin_files}})
        add_custom_command(OUTPUT ${bin_file}.c
                COMMAND ${PYTHON3} buildtools/bintileconvert/bintileconvert.py ${bin_file} ${build_dir}
                        ${palette_bank_num}
                VERBATIM)
    endforeach()

    execute_grit_tilemaps(palette_bank_num)
endfunction()

function(convert_tilemap_bin_image_file bin_file build_dir width height palette_bank_num image_file is_8bpp is_affine)
    if(WIN32)
        find_program(PYTHON3 python)
    else()
        find_program(PYTHON3 python3)
    endif()

    if(NOT PYTHON3)
        message(FATAL_ERROR "python3 - not found")
    endif()

    get_filename_component(bin_file_path_name ${bin_file} NAME)
    string(REPLACE ".bin" "" bin_file_path_name ${bin_file_path_name})

    message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/buildtools/bintileconvert/bintileconvert.py)
    message(STATUS ${bin_file_path_name})
    #message(STATUS "Palette bank = " ${palette_bank_num})

    if(is_8bpp)
        set(image_bpp 8)
    else()
        set(image_bpp 4)
    endif()

    if(is_affine)
        set(image_affine "true")
    else()
        set(image_affine "false")
    endif()

    add_custom_command(OUTPUT ${build_dir}/${bin_file_path_name}.c ${build_dir}/${bin_file_path_name}.h
            COMMAND ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/buildtools/bintileconvert/bintileconvert.py
            ${CMAKE_CURRENT_SOURCE_DIR}/${bin_file} ${build_dir} ${width} ${height}
            ${palette_bank_num} ${CMAKE_CURRENT_SOURCE_DIR}/${image_file} ${image_bpp} ${image_affine}
            VERBATIM)

endfunction()

function(generate_font ttf_font_file font_character_list_file make_4bpp background_color_r background_color_g
         background_color_b font_size)
    if(WIN32)
        find_program(PYTHON3 python)
    else()
        find_program(PYTHON3 python3)
    endif()

    if(NOT PYTHON3)
        message(FATAL_ERROR "python3 - not found")
    endif()

    get_filename_component(base_font_name ttf_font_file NAME_WE)

    add_custom_target(${CMAKE_CURRENT_BINARY_DIR}/${base_font_name} ALL SOURCES
            COMMAND ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/buildtools/generate_fonts/generate_fonts.py ${ttf_font_file}
                    ${font_character_list_file} ${make_4bpp} ${CMAKE_CURRENT_BINARY_DIR}/${base_font_name}
                    ${background_color_r} ${background_color_g} ${background_color_b} ${font_size}
            VERBATIM)
endfunction()

function(generate_maxmod_soundbank is_gba soundbank_name sound_files)
    if(NOT MMUTIL)
        find_program(MMUTIL mmutil ${DEVKITPRO}/tools)
        if(MMUTIL)
            message(STATUS "mmutil: ${MMUTIL} - found")
        else()
            message(FATAL_ERROR "mmutil - not found")
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

    # TODO(Bobby): Fix this... mess
    if(is_gba)
        message(STATUS "using maxmod for gba")
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.o
                COMMAND ${MMUTIL} -o${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin
                    -h${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.h ${${sound_files}}
                COMMAND ${BIN2S} ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin >
                                 ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.s
                COMMAND ${ASSEMBLER_TO_USE} ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.s -o
                                            ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.o
                VERBATIM)
    else()
        message(STATUS "using maxmod for nds")
        add_custom_command(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.o
                COMMAND ${MMUTIL} -d -o${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin
                -h${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.h ${${sound_files}}
                COMMAND ${BIN2S} ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin >
                ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.s
                COMMAND ${ASSEMBLER_TO_USE} ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.s -o
                ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}.bin.o
                VERBATIM)
    endif()

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${soundbank_name}_bin.h
            "extern const uint8_t ${soundbank_name}_bin_end[];\n"
            "extern const uint8_t ${soundbank_name}_bin[];\n"
            "extern const uint32_t ${soundbank_name}_bin_size;")
endfunction()
