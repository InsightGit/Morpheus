import os.path
import sys

from typing import Union


def _camel_case_conversion(header_guard: str) -> str:
    return_value = list(header_guard.lower())

    for i in range(len(return_value) - 1):
        if return_value[i] == "_":
            return_value[i + 1] = str(return_value[i + 1]).upper()

    return_value = "".join(return_value).replace("_", "")

    return return_value


def _generate_header_file(build_dir: str, file_name: str) -> Union[str, bool]:
    header_guard_name = file_name.upper().split()[0]

    try:
        variable_name = _camel_case_conversion(header_guard_name) + "TileMap"

        with open(os.path.join(build_dir, file_name) + ".h", 'w') as file_obj:
            file_obj.write("#ifndef " + header_guard_name + "\n#define " + header_guard_name + "\n")
            file_obj.write("extern const unsigned short " + variable_name + "[1024];\n")
            file_obj.write("#endif // " + header_guard_name + "\n")

        return variable_name
    except OSError:
        print("Couldn't create generated tilemap header file " + file_name + "!")

        return False


def _generate_source_file(build_dir: str, file_name: str, variable_name: str, hex_data: list,
                          palette_bank: int) -> bool:
    try:
        with open(os.path.join(build_dir, file_name) + ".c", 'w') as file_obj:
            file_obj.write("const unsigned short " + variable_name + "[1024] =\n{\n     ")

            for i in range(0, len(hex_data), 2):
                file_obj.write(hex(palette_bank) + str(int(hex_data[i + 1], 16)) + hex_data[i] + ",")

                if i % 16 == 0 and i > 0:
                    file_obj.write("\n      ")

            file_obj.write("\n};\n")

        return True
    except OSError:
        print("Couldn't create generated tilemap source file " + file_name + "!")

        return False


def _open_and_convert(file_path: str, build_dir: str, palette_bank: int = 0) -> bool:
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

    variable_name = _generate_header_file(build_dir, file_name)

    if isinstance(variable_name, bool) and not variable_name:
        file_obj.close()

        return False
    else:
        assert isinstance(variable_name, str)

        # Based of OneCricketeer's sample code: https://stackoverflow.com/a/35516257
        return_value = _generate_source_file(build_dir, file_name, variable_name,
                                             ["{:02x}".format(char).upper() for char in file_obj.read()], palette_bank)

        file_obj.close()

        if return_value:
            print("Successfully converted Tilemap Studio BIN file to C header and source files in " + build_dir + "!")

        return return_value


def main() -> None:
    if len(sys.argv) > 2:
        if len(sys.argv) > 3:
            try:
                palette_bank = int(sys.argv[3])

                if palette_bank > 15 or palette_bank < 0:
                    raise ValueError

                return_value = _open_and_convert(sys.argv[1], sys.argv[2], palette_bank)
            except ValueError:
                print("palette_bank argument must be an integer between 0 and 15!")
                sys.exit(2)
        else:
            return_value = _open_and_convert(sys.argv[1], sys.argv[2])

        if not return_value:
            sys.exit(1)
    else:
        print("Invalid syntax:")
        print(os.path.basename(__file__) + " tilemap_studio_bin_file_path build_dir [palette_bank]")
        sys.exit(2)


if __name__ == "__main__":
    main()
