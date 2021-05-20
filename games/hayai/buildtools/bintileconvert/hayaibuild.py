COIN_TILES = [0x22, 0x23, 0x24, 0x25, 0x32, 0x33, 0x34, 0x35]


def write_coins(base_file_path: str, map_variable_name: str, tile_map: list):
    coin_tile_positions = []

    for tile_index in range(len(tile_map)):
        for coin_tile_id in COIN_TILES:
            tile_id = int(tile_map[tile_index].replace(",", ""), 16)

            if (tile_id & 0x3FF) == coin_tile_id:
                coin_tile_positions.append(hex(tile_index))
                break

    if len(coin_tile_positions) > 0:
        with open(base_file_path + ".h", "r") as header_file_obj:
            header_source = header_file_obj.read()

        new_header_source = ""

        for header_source_line in iter(header_source.splitlines(True)):
            new_header_source += header_source_line

            if map_variable_name + "[" in header_source_line:
                new_header_source += f"\n#define {map_variable_name}CoinsTileIndexLen {len(coin_tile_positions)}\n"
                new_header_source += f"extern const unsigned short " \
                                     f"{map_variable_name}CoinsTileIndex[{len(coin_tile_positions)}];\n"

        with open(base_file_path + ".h", "w") as header_file_obj:
            header_file_obj.write(new_header_source)

        with open(base_file_path + ".c", "a") as source_file_obj:
            source_file_obj.write(f"\nconst unsigned short "
                                  f"{map_variable_name}CoinsTileIndex[{len(coin_tile_positions)}] = \n{{\n")

            past_new_line = False

            source_file_obj.write("    ")

            for i in range(len(coin_tile_positions)):
                source_file_obj.write(f"{coin_tile_positions[i]},")

                past_new_line = i != 0 and i % 8 == 0

                if past_new_line:
                    source_file_obj.write("\n    ")

            if not past_new_line:
                source_file_obj.write("\n")

            source_file_obj.write("};\n")
