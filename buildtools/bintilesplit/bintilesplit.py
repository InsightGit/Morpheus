import os
import subprocess
import sys


def _convert_tile_map_to_sbbs(width: int, height: int, palette_bank: int, hex_data: list) -> list:
    converted_map = []

    for i in range(width*height):
        converted_map.append(0)

    for tile_y in range(height):
        for tile_x in range(width):
            sbb = int(((tile_x >> 5) + (tile_y >> 5) * (width >> 5)))#int(((tile_y / 32) * (width / 32)) + (tile_x / 32))
            unconverted_map_index = int(tile_x + (tile_y * width))
            tile_id = int(sbb*1024 + ((tile_x & 31) + (tile_y & 31) * 32))#int((sbb*1024) + ((tile_y % 32) * 32) + (tile_x % 32))

            converted_map[tile_id] = (palette_bank << 12) | int.from_bytes(hex_data[unconverted_map_index], "little")

    return converted_map


def split_bin_file(single_bin_file_path: str, height: int, width: int, asset_dir: str, palette_bank: int) -> list:
    with open(single_bin_file_path, "rb") as single_tilemap_file:
        # Break it up into 64x64 tilemaps which themselves will be
        # broken up into 32x32 tilemaps
        # TODO(Bobby): Make this work for non-square large maps

        base_file_name = os.path.splitext(os.path.basename(single_bin_file_path))[0]
        hex_data = []
        tilemaps_64x64 = []

        for i in range(height * width):
            hex_data.append(0)

        for y in range(height):
            for x in range(width):
                hex_data[(height * y) + x] = single_tilemap_file.read(2)

        #["{:02x}".format(char).upper() for char in file_obj.read()]
        for background_y in range(int(height / 64)):
            for background_x in range(int(width / 64)):
                unconverted_tilemap_64x64 = []

                for i in range(64 * 64):
                    unconverted_tilemap_64x64.append(0)

                for y in range(background_y * 64, (background_y * 64) + 64):
                    for x in range(background_x * 64, (background_x * 64) + 64):
                        new_x = x - (background_x * 64)
                        new_y = y - (background_y * 64)

                        unconverted_tilemap_64x64[(new_y * 64) + new_x] = hex_data[(y * height) + x]

                tilemaps_64x64.append(_convert_tile_map_to_sbbs(64, 64, palette_bank, unconverted_tilemap_64x64))

        for i in range(len(tilemaps_64x64)):
            os.makedirs(asset_dir, exist_ok=True)

            with open(os.path.join(asset_dir, f"{base_file_name}-{i}.bin"), "wb") as split_tilemap_file:
                for tile in tilemaps_64x64[i]:
                    split_tilemap_file.write(tile.to_bytes(2, byteorder="little"))

        return tilemaps_64x64


def execute_grit(image_file: str, image_bpp: int, tilemaps_64x64: list, build_dir: str) -> None:
    base_image_file_name = os.path.splitext(os.path.basename(image_file))[0]
    grit_subprocess = ["grit", image_file]
    grit_target_file = os.path.join(build_dir, base_image_file_name)

    if image_bpp == 4:
        grit_subprocess.append("-gB4")
    elif image_bpp == 8:
        grit_subprocess.append("-gB8")
    else:
        print(f"Unknown image_bpp of {image_bpp} specified! Expected image_bpp values are 4 and 8.", file=sys.stderr)
        sys.exit(2)

    grit_subprocess.append("-ftc")

    grit_subprocess.append(f"-o{grit_target_file}.c")

    subprocess.run(grit_subprocess, capture_output=True)

    try:
        header_guard_name = base_image_file_name.upper()
        variable_name = header_guard_name.lower() + "Maps"

        with open(f"{grit_target_file}.h", 'r+') as file_obj:
            lines = file_obj.readlines()

            tile_map_string = f"#define {header_guard_name.lower()}IndividualMapLen {64 * 64 * 2}\n"
            tile_map_string += f"#define {variable_name}Len {len(tilemaps_64x64)}\n"
            tile_map_string += f"extern const unsigned short {variable_name}[{len(tilemaps_64x64)}][{64 * 64}];\n"
            tile_map_string += f"extern const unsigned short *{variable_name}Rows[{len(tilemaps_64x64)}];\n\n"

            for i in range(len(lines)):
                if "#endif" in lines[i]:
                    lines.insert(i, tile_map_string)
                    break

            file_obj.seek(0)

            file_obj.writelines(lines)

        with open(f"{grit_target_file}.c", 'a') as file_obj:
            file_obj.write(f"\nconst unsigned short {variable_name}[{len(tilemaps_64x64)}][{64 * 64}] = \n{{")

            for tilemap in tilemaps_64x64:
                file_obj.write("\n    {")

                for i in range(len(tilemap)):
                    if i % 8 == 0:
                        file_obj.write("\n      ")

                    file_obj.write(f"{hex(tilemap[i])},")

                file_obj.write("\n    },")

            file_obj.write("\n};")

            file_obj.write(f"\nconst unsigned short *{variable_name}Rows[{len(tilemaps_64x64)}] = \n{{")

            for i in range(len(tilemaps_64x64)):
                if i % 8 == 0:
                    file_obj.write("\n      ")

                file_obj.write(f"{variable_name}[{i}],")

            file_obj.write("\n};\n")
    except OSError:
        print(f"Grit couldn't generate the image header/source files!")
        sys.exit(1)


def main():
    #print(sys.argv)

    if len(sys.argv) > 4:
        single_bin_file_path = sys.argv[1]

        try:
            height = int(sys.argv[4])
            width = int(sys.argv[3])
        except ValueError:
            print("height and width must be integers!")
            sys.exit(2)

        try:
            file_size = os.path.getsize(single_bin_file_path)

            if (height != 128 and height != 256 and height != 512) or (width != 128 and width != 256 and width != 512):
                print("Tile width and height must be one of these combinations for a streaming background:\n "
                      "(128, 128), (256, 256), (512, 512)", file=sys.stderr)
                sys.exit(2)

            expected_file_size = height * width * 2

            if file_size != expected_file_size:
                print(f"{single_bin_file_path} of size ({width}, {height}) does not match the expected file "
                      f"size of {expected_file_size}")
                sys.exit(1)
        except OSError:
            print("Could not get size of the bin file provided!", file=sys.stderr)
            sys.exit(1)

        if len(sys.argv) > 5:
            try:
                palette_bank = int(sys.argv[5])

                if palette_bank > 15:
                    raise ValueError
            except ValueError:
                print("Palette bank must be number between 0 and 15!")
                sys.exit(2)
        else:
            palette_bank = 0

        tilemaps_64x64 = split_bin_file(single_bin_file_path, height, width, sys.argv[2], palette_bank)

        print(f"Split bin file successfully! File size was {file_size} bytes")

        if len(sys.argv) > 6:
            if len(sys.argv) > 7:
                if len(sys.argv) > 8:
                    try:
                        execute_grit(sys.argv[6], int(sys.argv[7]), tilemaps_64x64, sys.argv[8])
                    except ValueError:
                        print("image_bpp argument must be an integer!")
                        sys.exit(2)
                else:
                    print("Missing build_dir argument!")
                    sys.exit(2)
            else:
                print("Missing image_bpp and build_dir arguments!")
                sys.exit(2)
    else:
        print("Invalid syntax:", file=sys.stderr)
        print(f"{os.path.basename(__file__)} single_bin_file_path asset_dir width height [palette_bank]"
              f"[image_file] [image_bpp] [build_dir]",
              file=sys.stderr)
        print("Note: if an image_file argument is given, then image_bpp and build_dir become " +
              "required arguments.", file=sys.stderr)
        sys.exit(2)


if __name__ == "__main__":
    main()
