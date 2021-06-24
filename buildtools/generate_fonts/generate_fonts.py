import os
import subprocess
import sys

from PIL import Image, ImageColor, ImageChops, ImageDraw, ImageFont


def _convert_str_to_bool(argument: str, argument_name : str):
    if argument.lower() == "true":
        return True
    elif argument.lower() == "false":
        return False
    else:
        print(f"Argument {argument_name} must be either true or false (case insensitive)")
        sys.exit(2)


def main() -> None:
    if len(sys.argv) > 6:
        if len(sys.argv) > 10:
            font_size = int(sys.argv[10])
        else:
            font_size = 16

        font = ImageFont.truetype(sys.argv[1], font_size, encoding="unic")
        make_1d = _convert_str_to_bool(sys.argv[5], "make_1d")
        #make_4bpp = _convert_str_to_bool(sys.argv[3], "make_4bpp")
        size = [128, 0]
        use_utf8 = _convert_str_to_bool(sys.argv[6], "use_utf8")

        try:
            font_bpp = int(sys.argv[3])

            if font_bpp != 8 and font_bpp != 4 and font_bpp != 1:
                raise ValueError
        except ValueError:
            print(f"font_bpp argument must be either 8, 4, or 1 instead of {font_bpp}")
            sys.exit(2)

            return

        if font_size % 8 > 0:
            rounded_font_size = font_size + (8 - (font_size % 8))
        else:
            rounded_font_size = font_size

        print(f"Original font size is {font_size} Rounded font size is {rounded_font_size}")

        try:
            with open(sys.argv[2], 'r') as file:
                characters = file.read().replace("\n", "").split(" ")

                if len(characters) * (rounded_font_size / 8) < 128 / rounded_font_size:
                    size[0] = int((len(characters) * (rounded_font_size / 8)) * rounded_font_size)

                size[1] = int((((len(characters) + 1) + (rounded_font_size / 4)) / (128 / rounded_font_size)) *
                              rounded_font_size)
        except OSError:
            character_list_path = os.path.join(os.path.join(os.path.dirname(__file__),  "character_lists"),
                                               f"{sys.argv[2]}.txt")

            print(f"Couldn't open {sys.argv[2]}! Trying {character_list_path} instead")

            try:
                with open(character_list_path, 'r') as file:
                    characters = file.read().replace("\n", "").split(" ")

                    print(f"Character len: {len(characters)} vs {128 / rounded_font_size} and font size: "
                          f"{rounded_font_size}")

                    if len(characters) * (rounded_font_size / 8) < 128 / rounded_font_size:
                        size[0] = int((len(characters) * (rounded_font_size / 8)) * rounded_font_size)

                    size[1] = int((((len(characters) + 1) + (rounded_font_size / 4)) / (128 / rounded_font_size)) *
                                  rounded_font_size)
            except OSError:
                print(f"Couldn't open {character_list_path} either!")

                sys.exit(1)

                return

        if size[1] % 8 != 0:
            size[1] += 8 - (size[1] % 8)

        if font_bpp == 1:
            image_mode = "1"

            background_color = 1
        else:
            image_mode = "RGB"

            if len(sys.argv) > 7:
                background_color = (int(sys.argv[7]), int(sys.argv[8]), int(sys.argv[9]))
            else:
                background_color = (0, 0, 0)

        image = Image.new(image_mode, (size[0], size[1]), background_color)
        image_drawing = ImageDraw.Draw(image)
        image_path = f"{sys.argv[4]}.png"

        print(f"image size: ({size[0]}, {size[1]})")

        print_position = [rounded_font_size, -6]

        for character in characters:
            if print_position[1] >= size[1] - (rounded_font_size - 1):
                break

            if print_position[0] >= size[0] - (rounded_font_size - 1):
                print_position[0] = 0
                print_position[1] += rounded_font_size

            image_drawing.text((print_position[0], print_position[1]), character, features=["-kern"], font=font)

            #print(f"{character} at ({print_position[0]}, {print_position[1]})")

            print_position[0] += rounded_font_size

        #image_drawing.multiline_text((0, 0), string_to_print, fill=background_color, font=font)

        if font_bpp == 1:
            for x in range(image.size[0]):
                for y in range(image.size[1]):
                    image.putpixel((x, y), not image.getpixel((x, y)))

        if make_1d:
            working_coords = [0, 0]
            temp_1d_image = Image.new("RGB", (size[0], size[1]), background_color)
            temp_1d_image_draw = ImageDraw.Draw(temp_1d_image)

            for image_y in range(0, size[1], font_size):
                for image_x in range(0, size[0], font_size):
                    for font_x in range(int(font_size / 8)):
                        for font_y in range(int(font_size / 8)):
                            for x in range(8):
                                for y in range(8):
                                    old_image_coords = (image_x + (font_x * 8) + x, image_y + (font_y * 8) + y)

                                    temp_1d_image_draw.line([(working_coords[0] + x, working_coords[1] + y),
                                                             (working_coords[0] + x, working_coords[1] + y)],
                                                            fill=(image.getpixel(old_image_coords)), width=1)

                            if working_coords[0] + 8 >= size[0]:
                                working_coords[1] += 8
                                working_coords[0] = 0
                            else:
                                working_coords[0] += 8

            temp_1d_image.save(image_path, "PNG")
        else:
            image.save(image_path, "PNG")

        base_image_file_name = os.path.splitext(os.path.basename(sys.argv[4]))[0].replace("-", "_")
        grit_subprocess = ["grit", image_path]

        if font_bpp == 4:
            grit_subprocess.append("-gB4")
        elif font_bpp == 8:
            grit_subprocess.append("-gB8")
            grit_subprocess.append("-MRtf")
        elif font_bpp == 1:
            grit_subprocess.append("-gB1")

        grit_subprocess.append("-ftc")

        print(subprocess.run(grit_subprocess, capture_output=True))

        if use_utf8:
            with open(f"{base_image_file_name}_utf_map.hpp", 'w') as grit_hpp_file:
                grit_hpp_file.write("#include <map>\n")

                grit_hpp_file.write(f"const std::map<unsigned int, unsigned int> {base_image_file_name}UtfMap=\n{{")

                for i in range(len(characters)):
                    if i % 4 == 0:
                        grit_hpp_file.write("\n    ")

                    if make_1d:
                        # + 1 to account for blank starting space
                        tile_id = int(4 * (i + 1))
                    else:
                        tile_id = (int(rounded_font_size / 8) * int((i + 1) % int(128 / rounded_font_size))) + \
                                  int(int(16 * int(rounded_font_size / 8)) * int((i + 1) / 8))

                    grit_hpp_file.write(f"{{ {ord(characters[i])}, {tile_id} }}, ")

                grit_hpp_file.write("\n};\n")

    else:
        print("Invalid syntax:")
        print(f"{os.path.basename(__file__)} ttf_font_file font_character_list_file font_bpp destination_file make_1d "
              f"use_utf8 [background_color_r] [background_color_g] [background_color_b] [font_size]")

        sys.exit(2)


if __name__ == "__main__":
    main()
