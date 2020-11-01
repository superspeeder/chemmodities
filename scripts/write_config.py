from distutils import spawn
import os
import zipfile
import urllib.request


project_dir = os.path.dirname(os.path.realpath(__file__))
project_dir = os.path.join(*os.path.split(project_dir)[:-1])

programfiles86 = "%PROGRAMFILES(x86)%"

isad = False

if (os.path.isfile(os.path.join(project_dir,'sid'))):
    with open(os.path.join(project_dir,'sid'), 'r') as f:
        if f.read() == "andy":
            programfiles86 = "D:\\Program Files (x86)"
            isad = True


d = "D" if isad else "C"

def downloadcmake():
    url = "https://github.com/Kitware/CMake/releases/download/v3.19.0-rc2/cmake-3.19.0-rc2-win64-x64.zip"
    response = urllib.request.urlopen(url)
    data = response.read()
    os.makedirs(os.path.join(project_dir,'tools','cmake'))
    with open(os.path.join(project_dir,'tools','cmake','cmake-3.19.0-rc2-win64-x64.zip'), 'wb+') as f:
        f.write(data)

    with zipfile.ZipFile(os.path.join(project_dir,'tools','cmake','cmake-3.19.0-rc2-win64-x64.zip'),'r') as zf:
        zf.extractall(os.path.join(project_dir,'tools','cmake'))
    



cmake_location = spawn.find_executable("cmake")
cmake_location = None
if (cmake_location == None):
    if (os.path.isfile(os.path.join(project_dir,'tools','cmake','cmake-3.19.0-rc2-win64-x64','bin','cmake.exe'))):
        cmake_location = os.path.join(project_dir,'tools','cmake','cmake-3.19.0-rc2-win64-x64','bin','cmake.exe')
        print("Located portable cmake")
    else:
        download = input("CMake not found. Download? ")
        if (download.lower() == "y" or download.lower() == "yes"):
            print("Downloading CMake")
            downloadcmake()
            print(f"Downloaded CMake to {os.path.join(project_dir, 'tools', 'cmake')}")
            cmake_location = os.path.join(project_dir,'tools','cmake','cmake-3.19.0-rc2-win64-x64','bin','cmake.exe')
        else:
            cmake_location = input("Please enter the location of cmake.exe: ")
            while not cmake_location.endswith("cmake.exe"):
                cmake_location = input("Please enter the location of a valid cmake.exe (must be named cmake.exe): ")

def download7z():
    url = "https://www.7-zip.org/a/7z1900-x64.exe"
    response = urllib.request.urlopen(url)
    data = response.read()
    os.makedirs(os.path.join(project_dir,'tools','7zip'))

    with open(f"{project_dir}/tools/7zip/7z1900-x64.exe", "wb+") as inst:
        inst.write(data)
    os.system(f"{os.path.join(project_dir,'tools','7zip','7z1900-x64.exe')} /S /D=\"{os.path.join(project_dir,'tools','7zip')}\"")


_7z_location = spawn.find_executable("7z")
if (_7z_location == None):
    if (os.path.isfile(os.path.join(project_dir,'tools','7zip','7z.exe'))):
        _7z_location = os.path.join(project_dir,'tools','7zip','7z.exe')
        print("Located portable 7zip")
    else:
        download = input("7-Zip not found. Download? ")
        if (download.lower() == "y" or download.lower() == "yes"):
            print("Downloading 7-Zip")
            download7z()
            print(f"7-zip installed to {os.path.join(project_dir,'tools','7zip')}")
            _7z_location = os.path.join(project_dir,'tools','7zip','7z.exe')
        else:
            _7z_location = input("Please enter the location of 7z.exe: ")
            while not _7z_location.endswith("7z.exe"):
                _7z_location = input("Please enter the location of a valid 7z.exe (must be named 7z.exe): ")




def check(bm):
    build_config = None
    if (bm == 1):
        build_config = "Debug"
    elif (bm == 2):
        build_config = "Release"
    else:
        print(f"{bm} is not a valid build mode. Please pick 1 or 2")    
        nbm = int(input("> "))
        build_config = check(nbm)

    return build_config


print("Pick a build config:\n\t[1] Debug")
print("Pick a build config:\n\t[2] Release")
bc = int(input("> "))
build_config = check(bc)


config_file = f"""
cd "D:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\"
call "{d}:\\Program Files (x86)\\Microsoft Visual Studio\\2019\\Community\\Common7\\Tools\\VsDevCmd.bat"
cd "{project_dir}\\scripts"

SET "CMAKE_LOCATION={cmake_location}"
SET "PROJECTDIR={project_dir}"
SET "BUILDCONFIG={build_config}"
SET "_7ZIP_EXE={_7z_location}"
SET "PROGRAMFILES86={programfiles86}"
"""

with open(os.path.join(project_dir, "scripts", "config.bat"), "w+") as f:
    f.write(config_file)