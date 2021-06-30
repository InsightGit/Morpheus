import glob
import os
import re
import sys


HEADER_EXTENSIONS = [".hpp"]
PROJECT_NAME = "morpheus"


def generate_rst_file_for_object(breathe_command: str, object_name: str, namespace_name: str, namespace_dir: str,
                                 file_path: str):
    with open(os.path.join(namespace_dir, f"{object_name}.rst"), 'w') as header_rst_file:
        header_rst_file.write(f"{object_name}\n")

        for i in range(len(object_name)):
            header_rst_file.write("=")

        header_rst_file.write(f"\n*Located within {file_path}*\n")

        header_rst_file.write(f"\n.. {breathe_command}:: {namespace_name}::{object_name}\n"
                              f"    :project: {PROJECT_NAME}\n")

        if breathe_command == "doxygenclass":
            header_rst_file.write(f"    :members:\n"
                                  f"    :protected-members:\n"
                                  f"    :undoc-members:\n")


def generate_rst_files_for_header(breathe_dir: str, file_name: str, file_content: dict, namespace_name: str) -> None:
    namespace_dir_names = namespace_name.split("::")
    namespace_dir = os.path.join(breathe_dir, "classes")

    for namespace_dir_name in namespace_dir_names:
        namespace_dir = os.path.join(namespace_dir, namespace_dir_name)

    relative_file_path = os.path.join(namespace_dir.lstrip(breathe_dir).lstrip("lasses/"), file_name)

    print(f"file content: {file_content} namespace path: {namespace_dir} namespace name: {namespace_name}")

    # Ask for forgiveness, not for permission
    try:
        os.makedirs(namespace_dir, exist_ok=True)
    except FileExistsError:
        pass

    for class_name in file_content["classes"]:
        generate_rst_file_for_object("doxygenclass", class_name, namespace_name, namespace_dir, relative_file_path)

    for enum_name in file_content["enums"]:
        generate_rst_file_for_object("doxygenenum", enum_name, namespace_name, namespace_dir, relative_file_path)

    for struct_name in file_content["structs"]:
        generate_rst_file_for_object("doxygenstruct", struct_name, namespace_name, namespace_dir, relative_file_path)


def get_object_name(object_type_name: str, object_name_line: str) -> str:
    looking_for_class_name = False
    string_space_split = object_name_line.split(" ")

    for string in string_space_split:
        if len(string) == 0:
            continue
        elif looking_for_class_name:
            return string
        elif string == object_type_name:
            looking_for_class_name = True

    return ""


def identify_cpp_file_content(file_name: str) -> dict:
    return_value = {"classes": [], "enums": [], "structs": []}

    c_enum_re_pattern = re.compile("enum .*{")
    c_struct_re_pattern = re.compile("struct .*{")
    cpp_class_re_pattern = re.compile("class .*{")
    cpp_enum_class_re_pattern = re.compile("enum class .*{")

    with open(file_name, 'r') as cpp_file:
        for line in cpp_file.readlines():
            if cpp_class_re_pattern.search(line) is not None:
                if cpp_enum_class_re_pattern.search(line) is not None:
                    print("found enum class syntax")

                    return_value["enums"].append(get_object_name("class", line))
                else:
                    print("found class syntax")

                    return_value["classes"].append(get_object_name("class", line))
            elif c_enum_re_pattern.search(line) is not None:
                print("found enum syntax")

                return_value["enums"].append(get_object_name("enum", line))
            elif c_struct_re_pattern.search(line) is not None:
                print("found struct syntax")

                return_value["structs"].append(get_object_name("struct", line))

    return return_value


def generate_header_rst_files(header_path: str, breathe_path: str, root_namespace: str = "") -> list:
    print(f"header path: {header_path}")

    for root, dirs, files in os.walk(header_path):
        for file in files:
            file_name, file_extension = os.path.splitext(file)

            #print(f"file name: {file_name} file extension: {file_extension}")

            if file_extension in HEADER_EXTENSIONS:
                complete_file_path = os.path.join(root, file)

                cpp_file_content = identify_cpp_file_content(complete_file_path)

                namespace_name = root_namespace + os.path.dirname(complete_file_path).replace(header_path, "").\
                    replace("/", "::")
                namespace_name = namespace_name.lstrip("::")

                generate_rst_files_for_header(breathe_path, file, cpp_file_content, namespace_name)

        print(f"{dirs} with namespace name {namespace_name}")

        for dir in dirs:
            complete_dir_path = os.path.join(os.path.join(os.path.join(breathe_path, "classes"),
                                                          os.path.join(root_namespace, dir)))


            if namespace_name != root_namespace:
                namespace_path = namespace_name.replace("::", "/")

                complete_dir_path = complete_dir_path.replace(os.path.join("classes", root_namespace),
                                                              os.path.join("classes", namespace_path))

            print(complete_dir_path)

            working_namespace_name = complete_dir_path.replace(breathe_path, "").replace("/", "::").lstrip("classes::")

            print(working_namespace_name)

            os.makedirs(complete_dir_path, exist_ok=True)

            generate_namespace_toctree(complete_dir_path, working_namespace_name)


def generate_namespace_toctree(namespace_dir: str, namespace_name: str):
    added_file_to_toc = False

    with open(os.path.join(namespace_dir, "class_index.rst"), 'w') as file:
        title_string = f"{namespace_name}\n"

        for i in range(len(namespace_name)):
            title_string += "="

        file.write(f"{title_string}\n"
                   f".. toctree::\n"
                   f"   :maxdepth: 1\n\n")

        sub_toc_files_to_add = []

        for root_file in os.listdir(namespace_dir):
            class_toctree = os.path.join(root_file, "class_index.rst")

            if os.path.isdir(os.path.join(namespace_dir, root_file)):
                sub_toc_files_to_add.append(os.path.join(root_file, "class_index.rst"))

        for sub_toc_file in sorted(sub_toc_files_to_add):
            file.write(f"   {sub_toc_file}\n")

            added_file_to_toc = True

        rst_files_to_add = []

        for rst_file in glob.glob(os.path.join(namespace_dir, "*.rst")):
            if rst_file == os.path.join(namespace_dir, "class_index.rst"):
                continue
            else:
                rst_files_to_add.append(rst_file.replace(namespace_dir, "").lstrip("/"))

        for rst_file in sorted(rst_files_to_add):
            file.write(f"   {rst_file}\n")

            added_file_to_toc = True

    if not added_file_to_toc:
        os.remove(os.path.join(namespace_dir, "class_index.rst"))
        os.rmdir(namespace_dir)


def generate_root_toctree(root_namespace_name: str = "morpheus"):
    root_path = "../docs/classes"

    with open(os.path.join(root_path, "class_index.rst"), 'w') as root_toctree:
        class_reference_title = "Class Reference"

        root_toctree.write(f"{class_reference_title}\n")

        for i in range(len(class_reference_title)):
            root_toctree.write("=")

        root_toctree.write(f"\n\n.. toctree::\n"
                           f"   :maxdepth: 2\n\n")

        relative_dir = os.path.join(root_path, root_namespace_name)

        for root_file in os.listdir(relative_dir):
            class_toctree = os.path.join(root_file, "class_index.rst")

            if os.path.isdir(os.path.join(relative_dir, root_file)):
                relative_path = os.path.join(root_namespace_name, root_file, "class_index.rst")

                root_toctree.write(f"   {relative_path}\n")


def main() -> None:
    path = sys.argv[3]

    print(f"file path: {path}")

    if len(sys.argv) <= 1:
        print("Invalid syntax:")
        print(os.path.basename(__file__) + " header_directory [root_namespace]")

        sys.exit(2)

    if os.path.isfile(os.path.join(path, "conf.py")) and \
       os.path.isfile(os.path.join(path, "Makefile")) and \
       os.path.isfile(os.path.join(path, "make.bat")):
        i = 0

        # TODO(Bobby): Get rid of this hack
        while i < 2:
            if len(sys.argv) > 2:
                generate_header_rst_files(sys.argv[1], path, sys.argv[2])
                generate_root_toctree(sys.argv[2])
            else:
                generate_header_rst_files(sys.argv[1], path)
                generate_root_toctree()

            i += 1
    else:
        print("This script is not in the breathe build directory!")

        sys.exit(1)


if __name__ == "__main__":
    main()
