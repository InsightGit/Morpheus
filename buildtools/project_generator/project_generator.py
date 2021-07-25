#!/usr/bin/python3

import os
import shutil
import sys

from distutils import dir_util

BUILDTOOLS_TO_COPY = ["bintileconvert", "bintilesplit", "gba_fat_patch", "generate_fonts"]


def _print_usage():
    print(f"Usage: {__file__} morpheus_dir_path new_project_name [new_project_dir_path]")


def main():
    if len(sys.argv) > 1 and sys.argv[1] == "--help":
        _print_usage()

        sys.exit(0)

    if len(sys.argv) > 2:
        morpheus_dir = sys.argv[1]
        new_project_name = sys.argv[2]

        if len(sys.argv) > 3:
            new_project_dir_path = sys.argv[3]
        else:
            new_project_dir_path = os.getcwd()

        project_dir = os.path.join(new_project_dir_path, new_project_name)

        try:
            os.makedirs(project_dir, exist_ok=False)

            os.makedirs(os.path.join(project_dir, "buildtools"), exist_ok=False)
            os.makedirs(os.path.join(project_dir, "cmake"), exist_ok=False)
            os.makedirs(os.path.join(project_dir, "src"), exist_ok=False)
        except OSError as e:
            raise e

            print(f"Found existing Morpheus directory structure under {project_dir}!\n"
                  f"Not overwriting files and aborting...", file=sys.stderr)

            sys.exit(1)

        try:
            for buildtool in BUILDTOOLS_TO_COPY:
                dir_util.copy_tree(os.path.join(morpheus_dir, "buildtools", buildtool),
                                   os.path.join(project_dir, "buildtools", buildtool))

            build_utils_path = os.path.join("cmake", "morpheus_build_utils.cmake")

            dir_util.copy_tree(os.path.join(morpheus_dir, "buildtools", "project_generator", "project_template"),
                               project_dir)

            shutil.copy(os.path.join(morpheus_dir, build_utils_path), os.path.join(project_dir, build_utils_path))
        except OSError:
            print(f"Couldn't copy project template files over to {project_dir}! \n"
                  f"Reverting to original state and aborting...", file=sys.stderr)

            try:
                shutil.rmtree(project_dir)
            except OSError:
                print("Couldn't revert to original state! Incomplete project "
                      "directory will have to be manually deleted.", file=sys.stderr)

            sys.exit(1)

        with open(os.path.join(project_dir, "CMakeLists.txt"), 'r') as cmake_file:
            cmake_file_contents = cmake_file.read()

        with open(os.path.join(project_dir, "CMakeLists.txt"), 'w') as cmake_file:
            cmake_file.write(cmake_file_contents.\
                replace("set(PROJECT_NAME \"Project Template Name\")",
                        f"set(PROJECT_NAME \"{new_project_name}\")").\
                replace("set(MORPHEUS_DIR \"\")",
                        f"set(MORPHEUS_DIR \"{os.path.abspath(morpheus_dir).replace("\", "\/")})"))

        print(f"Morpheus project {new_project_name} successfully created at {project_dir}!")
    else:
        print("Insufficent arguments!", file=sys.stderr)

        _print_usage()

        sys.exit(1)


if __name__ == "__main__":
    main()
