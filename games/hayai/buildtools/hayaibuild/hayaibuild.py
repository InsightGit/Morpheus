COIN_TILES = [0x22, 0x23, 0x32, 0x33, 0x24, 0x25, 0x34, 0x35]
ENEMY_TILES = [0x64, 0x65, 0x74, 0x75]


def _get_vector2_pos_from_tile_index(tile_index: int) -> str:
    sbb = int(tile_index / 1024)
    vector2_pos = [0, 0]  # (x, y)

    if sbb == 1 or sbb == 3:
        vector2_pos[0] += 32

    if sbb >= 2:
        vector2_pos[1] += 32

    vector2_pos[0] += int((tile_index - sbb) % 32)
    vector2_pos[1] += int((tile_index - sbb) / 32)

    return f"morpheus::core::gfx::Vector2({vector2_pos[0]}, {vector2_pos[1]})"


def get_enemy_spawn_positions_and_cleaned_map(tile_map: list) -> tuple:
    enemy_spawn_positions = []

    for tile_index in range(len(tile_map)):
        for i in range(len(ENEMY_TILES)):
            tile_id = int(tile_map[tile_index].replace(",", ""), 16)

            if (tile_id & 0x3FF) == ENEMY_TILES[i]:
                if i == 0:
                    enemy_spawn_positions.append(_get_vector2_pos_from_tile_index(tile_index))

                tile_map[tile_index] = hex(0)
                break

    return enemy_spawn_positions, tile_map


def _get_coin_indicies_str_for_one_map(tile_map: list) -> tuple:
    coin_tile_positions = []
    coin_indicies_str = ""
    past_new_line = False

    for tile_index in range(len(tile_map)):
        for coin_tile_id in COIN_TILES:
            tile_id = int(tile_map[tile_index].replace(",", ""), 16)

            if (tile_id & 0x3FF) == coin_tile_id:
                coin_tile_positions.append(hex(tile_index))
                break

    coin_indicies_str += "    \n"

    for i in range(len(coin_tile_positions)):
        coin_indicies_str += f"{coin_tile_positions[i]},"

        past_new_line = i != 0 and i % 8 == 0

        if past_new_line:
            coin_indicies_str += "\n    "

    if not past_new_line:
        coin_indicies_str += "\n"

    coin_indicies_str += "};\n"

    return len(coin_tile_positions), coin_indicies_str


def get_coin_indices(map_variable_name: str, tile_map: list, number_of_sub_tile_maps: int) -> tuple:
    header_coin_indices_string = ""
    source_coin_indices_string = ""

    if number_of_sub_tile_maps > 1:
        coin_tile_positions = []

        for i in range(number_of_sub_tile_maps):
            coin_tile_positions.append(_get_coin_indicies_str_for_one_map(tile_map[i]))

        header_coin_indices_string += f"#include <vector>\n\n"
        header_coin_indices_string += f"\n#define {map_variable_name}CoinsTileIndexLen {len(coin_tile_positions)}\n"
        header_coin_indices_string += f"extern const std::vector<unsigned short> " \
                                      f"{map_variable_name}CoinsTileIndex[{len(coin_tile_positions)}];\n"
    else:
        coin_tile_positions_len, source_coin_indices_string = _get_coin_indicies_str_for_one_map(tile_map)

        header_coin_indices_string += f"\n#define {map_variable_name}CoinsTileIndexLen {coin_tile_positions_len}\n"
        header_coin_indices_string += f"extern const unsigned short {map_variable_name}CoinsTileIndex[" \
                                      f"{coin_tile_positions_len}];\n"

    return header_coin_indices_string, f"\nconst unsigned short {map_variable_name}CoinsTileIndex[" \
                                       f"{coin_tile_positions_len}] = {{" \
                                       f"   {source_coin_indices_string}\n"
