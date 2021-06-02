import os
import re
import sys


HEADER_EXTENSIONS = [".hpp"]
PROJECT_NAME = "morpheus"


def generate_header_rst_file(class_name: str, namespace_name: str, breathe_path: str) -> None:
    namespace_dir = ""
    namespace_dir_names = namespace_name.split("::")

    for namespace_dir_name in namespace_dir_names:
        namespace_dir = os.path.join(namespace_dir, namespace_dir_name)

    namespace_dir = os.path.join(breathe_path, namespace_dir)

    # Ask for forgiveness, not for permission
    try:
        os.mkdir(namespace_dir)
    except FileExistsError:
        pass

    with open(os.path.join(namespace_dir, f"{class_name}.rst"), 'w') as header_rst_file:
        equal_string = ""

        header_rst_file.write(class_name)

        for i in range(len(class_name)):
            equal_string += "="

        header_rst_file.write(f"{equal_string}\n"
                              f"    :project: {PROJECT_NAME}"
                              f"    :members:")


def identify_cpp_classes(file_name: str) -> list:
    class_list = []
    cpp_class_re_pattern = re.compile("class .*{")

    with open(file_name, 'r') as cpp_file:
        for line in cpp_file.readlines():
            if cpp_class_re_pattern.search() is not None and "enum class" not in line:
                looking_for_class_name = False
                string_space_split = line.split(" ")

                for string in string_space_split:
                    if string.empty():
                        continue
                    elif looking_for_class_name:
                        class_list.append(string)
                        break
                    elif string == "class":
                        looking_for_class_name = True


def generate_header_rst_files(header_path: str, breathe_path: str) -> None :
    for root, dirs, files in os.walk(header_path):
        for file in files:
            file_name, file_extension = os.path.splitext(file)

            if file_extension in HEADER_EXTENSIONS:
                complete_file_path = os.path.join(root, file)

                for class_name in identify_cpp_classes(complete_file_path):
                    namespace_name = os.path.dirname(complete_file_path).replace("/", "::")

                    generate_header_rst_file(class_name, namespace_name, breathe_path)


def main() -> None:
    path = os.path.realpath(__file__)

    header_path = os.path.isfile(os.path.join(path, "../morpheus"))

    if os.path.isfile(os.path.join(path, "conf.py")) and \
       os.path.isfile(os.path.join(path, "Makefile")) and \
       os.path.isfile(os.path.join(path, "make.bat")) and \
       os.path.isdir(header_path):
        generate_header_rst_files(header_path, path)
    else:
        print("This script is not in the breathe build directory!")

        sys.exit(1)


if __name__ == "__main__":
    main()
