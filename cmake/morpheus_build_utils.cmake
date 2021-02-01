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

function(execute_grit_tilemaps png_files is_4bpp palette_bank_num)
    foreach(png_file IN ${${png_files}})
        execute_grit_tilemap(png_file ${is_4bpp} ${palette_bank_num})
    endforeach()
endfunction()

function(execute_grit_tilemap png_file is_4bpp palette_bank_num)
    if(is_4bpp)
        set(bpp_flag "4")
        set(tile_map_flag "tpf")
    else()
        set(bpp_flag "8")
        set(tile_map_flag "tf")
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

function(convert_tilemap_bin_image_file bin_file build_dir width height palette_bank_num image_file is_8bpp)
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

    add_custom_command(OUTPUT ${build_dir}/${bin_file_path_name}.c ${build_dir}/${bin_file_path_name}.h
            COMMAND ${PYTHON3} ${CMAKE_CURRENT_SOURCE_DIR}/buildtools/bintileconvert/bintileconvert.py
            ${CMAKE_CURRENT_SOURCE_DIR}/${bin_file} ${build_dir} ${width} ${height}
            ${palette_bank_num} ${CMAKE_CURRENT_SOURCE_DIR}/${image_file} 8
            VERBATIM)

endfunction()
