import os
import subprocess
import sys

from PIL import Image, ImageColor, ImageDraw, ImageFont


def main() -> None:
    if len(sys.argv) > 5:
        print(sys.argv)

        if len(sys.argv) > 9:
            font_size = int(sys.argv[9])
        else:
            font_size = 16

        if sys.argv[3].lower() == "true":
            make_4bpp = True
        elif sys.argv[3].lower() == "false":
            make_4bpp = False
        else:
            print("make_4bpp argument must either be True or False (case insensitive)")

            sys.exit(2)

            return

        if sys.argv[5].lower() == "true":
            make_1d = True
        elif sys.argv[5].lower() == "false":
            make_1d = False
        else:
            print("make_1d argument must either be True or False (case insensitive)")

            sys.exit(2)

            return

        font = ImageFont.truetype(sys.argv[1], font_size, encoding="unic")
        size = [font_size * 8, 0]

        try:
            with open(sys.argv[2], 'r') as file:
                characters = file.read().replace("\n", "").split(" ")

                if len(characters) >= 128 / font_size:
                    size[0] = len(characters) * font_size

                size[1] = (len(characters) % (128 / font_size))
        except OSError:
            character_list_path = os.path.join(os.path.join(os.path.dirname(__file__),  "character_lists"),
                                               f"{sys.argv[2]}.txt")

            print(f"Couldn't open {sys.argv[2]}! Trying {character_list_path} instead")

            try:
                with open(character_list_path, 'r') as file:
                    characters = file.read().replace("\n", "").split(" ")

                    print(f"Character len: {len(characters)} vs {128 / font_size} and font size: {font_size}")

                    if len(characters) + (font_size / 8) <= 128 / font_size:
                        size[0] = int((len(characters) + (font_size / 8)) * font_size)

                    size[1] = int(((len(characters) + (font_size / 4)) / (128 / font_size)) * font_size)
            except OSError:
                print(f"Couldn't open {character_list_path} either!")

                sys.exit(1)

                return

        if len(sys.argv) > 7:
            background_color = (int(sys.argv[6]), int(sys.argv[7]), int(sys.argv[8]))
        else:
            background_color = (0, 0, 0)

        image = Image.new("RGB", (size[0], size[1]), background_color)
        image_drawing = ImageDraw.Draw(image)
        image_path = f"{sys.argv[4]}.png"

        print(f"image size: ({size[0]}, {size[1]})")

        print_position = [font_size, -6]

        for character in characters:
            if print_position[1] >= size[1] - (font_size - 1):
                break

            if print_position[0] >= size[0] - (font_size - 1):
                print_position[0] = 0
                print_position[1] += font_size

            image_drawing.text((print_position[0], print_position[1]), character, font=font)

            print(f"{character} at ({print_position[0]}, {print_position[1]})")

            print_position[0] += font_size

        #image_drawing.multiline_text((0, 0), string_to_print, fill=background_color, font=font)

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

        grit_subprocess = ["grit", image_path]

        if sys.argv:
            grit_subprocess.append("-gB4")
            #grit_subprocess.append("-MRtpf")
        else:
            grit_subprocess.append("-gB8")
            grit_subprocess.append("-MRtf")

        grit_subprocess.append("-ftc")

        print(subprocess.run(grit_subprocess, capture_output=True))
    else:
        print("Invalid syntax:")
        print(f"{os.path.basename(__file__)} ttf_font_file font_character_list_file make_4bpp destination_file make_1d "
              f"[background_color_r] [background_color_g] [background_color_b] [font_size]")

        sys.exit(2)


if __name__ == "__main__":
    main()
