import os
import subprocess
import time

# ------------------------------------------------------------------------------

def main():
    root_folder = os.path.dirname(os.path.abspath(__file__))
    node_examples = os.path.join(root_folder, 'node_process_bridge', 'example')
    node_parent = os.path.join(node_examples, 'parent.js')
    node_child = os.path.join(node_examples, 'child.js')
    c_examples = os.path.join(root_folder, 'c_process_bridge', 'example')
    
    c_build_folder = os.path.join(c_examples, 'build')
    c_bin_folder = os.path.join(c_examples, 'bin')
    c_executable = os.path.join(c_bin_folder, 'example_process_bridge')
    if os.name == 'nt':
        c_executable += '.exe'
    
    
    # run node parent node child
    run([
        'node', stringify_path(node_parent),
        'node', stringify_path(node_child)
    ])
    
    # build child.c
    clean_directory(c_build_folder)
    clean_directory(c_bin_folder)
    build_system = '"Unix Makefiles"'
    if os.name == 'nt':
        build_system = '"MinGW Makefiles"'
    build_type = '-DCMAKE_BUILD_TYPE=Debug'
    run([
        'cmake', 
        '-S', stringify_path(c_examples), 
        '-B', stringify_path(c_build_folder), 
        '-G', build_system, 
        build_type
    ])
    run([
        'cmake', 
        '--build', stringify_path(c_build_folder)
    ])
    
    # run node parent child.c
    run([
        'node', stringify_path(node_parent),
        stringify_path(c_executable)
    ])
    
#
#    # configure
#
#    # build
#    run(['cmake', '--build', build_folder_str])
#
#    # run
#    run(stringify_path(executable))


# ------------------------------------------------------------------------------

def run(cmd: list[str]) -> None:
    command = None
    if isinstance(cmd, str):
        command = cmd
    elif isinstance(cmd, list) and all(isinstance(item, str) for item in cmd):
        command = ' '.join(cmd)
    else:
        raise ValueError("Argument type invalid, must be 'str' or 'list[str]'")
    
    START = '-----START-----'
    END = '-----END-----'
    
    print()
    print(f"Run: {command}")
    
    print(START)
    try:
        completed_process = subprocess.run(command, shell=True, check=True)
        print(END)
        print(f"Return code: {completed_process.returncode}")
    except subprocess.CalledProcessError as cpe:
        print(END)
        print(cpe)
    except Exception as e:
        print(END)
        print(f"Generic error: {e}")

    print()

# ------------------------------------------------------------------------------

def clean_directory(path: str) -> None:
    if os.path.exists(path):
        for root, dirs, files in os.walk(path, topdown=False):
            for name in files:
                file_path = os.path.join(root, name)
                os.remove(file_path)
            for name in dirs:
                dir_path = os.path.join(root, name)
                os.rmdir(dir_path)
        os.rmdir(path)
    os.makedirs(path, exist_ok=True)


def stringify_path(path: str) -> str:
    return f'"{path}"'

# ------------------------------------------------------------------------------

main()