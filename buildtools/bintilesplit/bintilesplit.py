import os
import tkinter
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

            converted_map[tile_id] = (palette_bank << 12) | int.from_bytes(hex_data[unconverted_map_index], "big")

    return converted_map


def split_bin_file(single_bin_file_path: str, single_bin_file_path_len: int, asset_dir: str, palette_bank: int):
    with open(single_bin_file_path, "rb") as single_tilemap_file:
        # Break it up into 64x64 tilemaps which themselves will be
        # broken up into 32x32 tilemaps
        # TODO(Bobby): Make this work for non-square large maps

        base_file_name = os.path.splitext(os.path.basename(single_bin_file_path))[0]
        cursor = 0
        tilemaps_64x64 = []

        while single_bin_file_path_len > cursor:
            hex_data = []

            for i in range(64 * 64):
                hex_data.append(0)

            for y in range(64):
                for x in range(64):
                    hex_data[(64 * y) + x] = single_tilemap_file.read(2)

            tilemaps_64x64.append(_convert_tile_map_to_sbbs(64, 64, palette_bank, hex_data))

            cursor += 64 * 64 * 2

        for i in range(len(tilemaps_64x64)):
            with open(os.path.join(asset_dir, f"{base_file_name}-{i}.bin"), "wb") as split_tilemap_file:
                for tile in tilemaps_64x64[i]:
                    split_tilemap_file.write(tile.to_bytes(2, byteorder="big"))


def execute_grit(image_file: str, image_bpp: int):
    grit_subprocess = ["grit", image_file]

    if image_bpp == 4:
        grit_subprocess.append("-gB4")
    elif image_bpp == 8:
        grit_subprocess.append("-gB8")
    else:
        print(f"Unknown image_bpp of {image_bpp} specified! Expected image_bpp values are 4 and 8.", file=sys.stderr)
        sys.exit(2)

    print(subprocess.run(grit_subprocess, capture_output=True))




def main():
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

            if (height == 64 and width == 64) or (height != 64 and height != 128) or (width != 64 and width != 128):
                print("Tile width and height must be one of these combinations for a streaming background:\n " \
                      "(64, 128), (128, 64), (128, 128)", file=sys.stderr)
                sys.exit(2)

            if height == 128 and width == 128:
                if file_size != (128 * 128 * 2):
                    print(f"{single_bin_file_path} of size (128, 128) does not match the expected file "
                          f"size of {128 * 128 * 2}")
                    sys.exit(1)
            elif file_size != (128 * 64 * 2):
                print(f"{single_bin_file_path} of size (128, 128) does not match the expected file "
                      f"size of {64 * 128 * 2}")
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

        split_bin_file(single_bin_file_path, file_size, sys.argv[2], palette_bank)

        print("Split bin file successfully!")

        if len(sys.argv) > 6:
            if len(sys.argv) > 7:
                execute_grit()
            else:
                print("Missing image_bpp argument!")
                sys.exit(2)
    else:
        print("Invalid syntax:", file=sys.stderr)
        print(f"{os.path.basename(__file__)} single_bin_file_path asset_dir width height [palette_bank]"
              f"[image_file] [image_bpp]",
              file=sys.stderr)
        print("Note: if an image_file argument is given, image_bpp becomes a " + \
              "required argument.", file=sys.stderr)
        sys.exit(2)


if __name__ == "__main__":
    main()
