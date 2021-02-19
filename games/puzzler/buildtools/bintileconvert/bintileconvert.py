import os.path
import subprocess
import sys

from typing import Union


def _camel_case_conversion(header_guard: str) -> str:
    return_value = list(header_guard.lower())

    for i in range(len(return_value) - 1):
        if return_value[i] == "_":
            return_value[i + 1] = str(return_value[i + 1]).upper()

    return_value = "".join(return_value).replace("_", "")

    return return_value


def _generate_header_file(build_dir: str, file_name: str, with_image_file: bool,
                          total_tile_size: int) -> Union[str, bool]:
    header_guard_name = file_name.upper().split()[0]

    try:
        variable_name = header_guard_name.lower() + "Map"

        if with_image_file:
            with open(os.path.join(build_dir, file_name) + ".h", 'r+') as file_obj:
                lines = file_obj.readlines()

                tile_map_string = "#define " + header_guard_name.lower() + "MapLen " + str(total_tile_size * 2) + "\n"
                tile_map_string += "extern const unsigned short " + variable_name + "[" + str(total_tile_size) + \
                                   "];\n\n"

                for i in range(len(lines)):
                    if "#endif" in lines[i]:
                        lines.insert(i, tile_map_string)
                        break

                file_obj.seek(0)

                file_obj.writelines(lines)
        else:
            with open(os.path.join(build_dir, file_name) + ".h", 'w') as file_obj:
                file_obj.write("#ifndef " + header_guard_name + "\n#define " + header_guard_name + "\n")
                file_obj.write("extern const unsigned short " + variable_name + "[" + str(total_tile_size) + "];\n")
                file_obj.write("#define " + header_guard_name + "_TILE_MAP_LENGTH " + str(total_tile_size * 2) + "\n")
                file_obj.write("#endif // " + header_guard_name + "\n")

        return variable_name
    except OSError as e:
        raise e
        print("Couldn't create generated tilemap header file " + file_name + "!")

        return False


def _generate_source_file(build_dir: str, file_name: str, variable_name: str, hex_data: list,
                          palette_bank: int, with_image_file: bool, total_tile_size: int) -> bool:
    file_path = os.path.join(build_dir, file_name)

    try:
        if with_image_file:
            file_mode = 'a'
        else:
            file_mode = 'w'

        with open(file_path + ".c", file_mode) as file_obj:
            file_obj.write("\nconst unsigned short " + variable_name + "[" + str(total_tile_size) + "] " +
                           "__attribute__((aligned(4))) __attribute__((visibility(\"hidden\"))) =\n{\n     ")

            for i in range(0, len(hex_data), 2):
                file_obj.write(hex(palette_bank) + str(int(hex_data[i + 1], 16)) + hex_data[i] + ",")

                if i % 16 == 0 and i > 0:
                    file_obj.write("\n      ")

            file_obj.write("\n};\n")

        return True
    except OSError:
        print("Couldn't create generated tilemap source file " + file_name + "!")

        return False


def _open_and_convert(file_path: str, build_dir: str, width: int, height: int,
                      palette_bank: int = 0, image_file: str = "", is_4bpp: bool = False) -> bool:
    try:
        file_obj = open(file_path, 'rb')
    except FileNotFoundError:
        print("Couldn't find Tilemap Studio BIN file " + file_path + "!")

        return False
    except OSError:
        print("Couldn't open Tilemap Studio BIN file " + file_path + "!")

        return False

    file_name_array = os.path.basename(file_path).split(".")
    file_name = ""

    for i in range(len(file_name_array)):
        if i != len(file_name_array) - 1:
            file_name += file_name_array[i]

    file_name = file_name.replace(".", "_")

    if len(image_file) > 0:
        grit_subprocess = ["grit", image_file]

        if is_4bpp:
            grit_subprocess.append("-gB4")
            #grit_subprocess.append("-MRtpf")
        else:
            grit_subprocess.append("-gB8")
            #grit_subprocess.append("-MRtf")

        grit_subprocess.append("-ftc")

        print(subprocess.run(["which", "grit"], capture_output=True))
        print(subprocess.run(grit_subprocess, capture_output=True))

    variable_name = _generate_header_file(build_dir, file_name, len(image_file) > 0, width * height)

    if isinstance(variable_name, bool) and not variable_name:
        file_obj.close()

        return False
    else:
        assert isinstance(variable_name, str)

        # Based of OneCricketeer's sample code: https://stackoverflow.com/a/35516257
        return_value = _generate_source_file(build_dir, file_name, variable_name,
                                             ["{:02x}".format(char).upper() for char in file_obj.read()], palette_bank,
                                             len(image_file) > 0, width * height)

        file_obj.close()

        if return_value:
            print("Successfully converted Tilemap Studio BIN file to C header and source files in " + build_dir + "!")

        return return_value


def main() -> None:
    if len(sys.argv) > 4:
        try:
            height = int(sys.argv[4])
            width = int(sys.argv[3])

            if (height != 32 and height != 64) or (width != 32 and width != 64):
                raise ValueError
        except ValueError:
            print("Tile width and height must be either 32 or 64!")

            sys.exit(2)

        if len(sys.argv) > 5:
            try:
                palette_bank = int(sys.argv[5])

                print("palette_bank:" + str(palette_bank))

                if palette_bank > 15 or palette_bank < 0:
                    raise ValueError

                if len(sys.argv) > 6:
                    if len(sys.argv) < 7:
                        print("Missing image_bpp argument to accompany image_file argument")
                        sys.exit(2)

                    # TODO(Bobby): Value checking like palette_bank on the image-related integer argument(s)
                    return_value = _open_and_convert(sys.argv[1], sys.argv[2], int(sys.argv[3]),
                                                     int(sys.argv[4]), palette_bank, sys.argv[6],
                                                     int(sys.argv[7]) == 4)
                else:
                    return_value = _open_and_convert(sys.argv[1], sys.argv[2], int(sys.argv[3]), int(sys.argv[4]),
                                                     palette_bank)
            except ValueError:
                print("palette_bank argument must be an integer between 0 and 15!")

                sys.exit(2)
        else:
            return_value = _open_and_convert(sys.argv[1], sys.argv[2],int(sys.argv[3]), int(sys.argv[4]))

        if not return_value:
            sys.exit(1)
    else:
        print("Invalid syntax:")
        print(os.path.basename(__file__) + " tilemap_studio_bin_file_path build_dir width height [palette_bank] "
                                           "[image_file] [image_bpp]")
        print("Note: if an image_file argument is given, image_bpp becomes a " +\
              "required argument.")
        sys.exit(2)


if __name__ == "__main__":
    main()
