import os
import subprocess
import sys


def apply_patch(patch_path: str):
    with open(patch_path, 'r+') as fat_header_file:
        lines = fat_header_file.readlines()

        for i in range(len(lines)):
            if "#include \"gba_types.h\"" in lines[i]:
                if "//#include \"gba_types.h\"" in lines[i]:
                    print("The libgba libfat patch was already applied!", file=sys.stderr)
                    sys.exit(1)
                else:
                    lines[i] = lines[i].replace("#include \"gba_types.h\"", "//#include \"gba_types.h\"")

        fat_header_file.seek(0)

        fat_header_file.writelines(lines)

    print("Successfully applied libgba libfat patch!")


def main():
    devkitpro_path = os.getenv("DEVKITPRO")

    if devkitpro_path is None:
        print("DEVKITPRO environment variable was not defined!", file=sys.stderr)
        sys.exit(2)

    patch_path = os.path.join(devkitpro_path, "libgba/include/disc_io.h")

    if len(sys.argv) >= 2 and sys.argv[1].lower() == "true":
        apply_patch(patch_path)
    else:
        try:
            with open(patch_path, 'r') as fat_header_file:
                lines = fat_header_file.readlines()

                for i in range(len(lines)):
                    if "#include \"gba_types.h\"" in lines[i] and "//#include \"gba_types.h\"" not in lines[i]:
                        if sys.platform == "win32":
                            print("libgba libfat patch required. Patching...")

                            apply_patch(patch_path)
                        else:
                            print("libgba libfat patch required. Requesting sudo...")

                            subprocess.run(["sudo", f"DEVKITPRO={devkitpro_path}", "python3", sys.argv[0], "true"])

                            return
        except OSError as e:
            print(f"Couldn't open {patch_path}! Can we read this path? Was libgba not installed?", file=sys.stderr)

            raise e

            sys.exit(2)


if __name__ == "__main__":
    main()
