import os
import subprocess
import time

# ------------------------------------------------------------------------------

def main():
    root_folder = os.path.dirname(os.path.abspath(__file__))

    node_test = os.path.join(root_folder, 'node_process_bridge', 'test')
    node_test_01 = os.path.join(node_test, 'test_01.js')
    node_test_02 = os.path.join(node_test, 'test_02.js')
    node_child = os.path.join(node_test, 'child.js')
    
    run([
        'node', stringify_path(node_test_01),
        'node', stringify_path(node_child)
    ])

    run([
        'node', stringify_path(node_test_02),
        'node', stringify_path(node_child)
    ])
    
    c_test = os.path.join(root_folder, 'c_process_bridge', 'test')
    c_build_folder = os.path.join(c_test, 'build')
    c_bin_folder = os.path.join(c_test, 'bin')
    c_executable = os.path.join(c_bin_folder, 'test_process_bridge')
    if os.name == 'nt':
        c_executable += '.exe'

    # build child.c
    clean_directory(c_build_folder)
    clean_directory(c_bin_folder)
    build_system = '"Unix Makefiles"'
    if os.name == 'nt':
        build_system = '"MinGW Makefiles"'
    build_type = '-DCMAKE_BUILD_TYPE=Debug'
    run([
        'cmake', 
        '-S', stringify_path(c_test), 
        '-B', stringify_path(c_build_folder), 
        '-G', build_system, 
        build_type
    ])
    run([
        'cmake', 
        '--build', stringify_path(c_build_folder)
    ])
    
    run([
        'node', stringify_path(node_test_01),
        stringify_path(c_executable)
    ])

    run([
        'node', stringify_path(node_test_02),
        stringify_path(c_executable)
    ])
    

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