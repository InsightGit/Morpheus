import os
import sys

from PIL import Image, ImageColor, ImageDraw, ImageFont


def main():
    if len(sys.argv) > 4:
        try:
            if len(sys.argv) > 8:
                font_size = sys.argv[7]
            else:
                font_size = 16

            font = ImageFont.truetype(sys.argv[1], font_size)
            size = [font_size * 8, 0]

            with open(sys.argv[2], 'r') as file:
                characters = file.read().replace("\n", "").split(" ")

                if len(characters) >= 128 / font_size:
                    size[0] = len(characters) * font_size

                size[1] = (len(characters) % (128 / font_size))

            if len(sys.argv) > 7:
                background_color = (sys.argv[5], sys.argv[6], sys.argv[7])
            else:
                background_color = (0, 0, 0)

            image = Image.new("RGB", (size[0], size[1]), background_color)
            image_drawing = ImageDraw.Draw(image)

            string_to_print = ""

            for i in range(len(characters)):
                string_to_print += characters[i]

                if i != 0 and i % 16 == 0:
                    string_to_print += "\n"

            image_drawing.multiline_text((0, 0), string_to_print, fill=background_color)

            image.de

            image.show()
        except OSError:
            print(f"Couldn't read font {sys.argv[2], }")
    else:
        print("Invalid syntax:")
        print(f"{os.path.basename(__file__)} ttf_font_file font_character_list_file make_4bpp destination_file "
              f"[background_color_r] [background_color_g] [background_color_b] [font_size]")

        sys.exit(2)


if __name__ == "__main__":
    main()
