import platform
import os
import sys
import shutil
import subprocess
from conan.errors import ConanInvalidConfiguration

def do_build(build_type: str = "Debug"):

    if build_type != "Debug" and build_type != "Release":
        raise ConanInvalidConfiguration("Only Debug and Release build types exists")

    # if os.path.exists("build"):
    #     shutil.rmtree("build")
    
    subprocess.run(f"conan install . --build missing --settings=build_type={build_type}")

    if platform.system() == "Windows":
        gen_folder = "generators\\"
        toolchain = f"{gen_folder}conan_toolchain.cmake"
        command = []
        with os.chdir("build"):
            command.append(
                f"conanbuild.bat"
            )
            command.append(
                f"cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE={toolchain} -DCMAKE_BUILD_TYPE={build_type}"
            )
            command.append(
                "deactivate_conanbuild.bat"
            )
            command.append(
                f"cmake --build . -j10 --config {build_type}"
            )
            subprocess.run(" && ".join(command))
    else:
        gen_folder = f"{build_type}/generators/"
        toolchain = f"{gen_folder}conan_toolchain.cmake"
        command = []
        with os.chdir("build"):
            command.append(
                f". conanbuild.sh"
            )
            command.append(
                f"cmake .. -G Ninja -DCMAKE_TOOLCHAIN_FILE={toolchain} -DCMAKE_BUILD_TYPE={build_type}"
            )
            command.append(
                ". deactivate_conanbuild.sh"
            )
            command.append(
                f"cmake --build . -j10 --config {build_type}"
            )
            subprocess.run(" && ".join(command))

if __name__ == "__main__":
    build_type = "Debug"
    if len(sys.argv) == 2:
        build_type = sys.argv[1]
    do_build(build_type=build_type)