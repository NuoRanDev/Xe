import os
import sys

RESET   = "\033[0m"
RED     = "\033[31m"
GREEN   = "\033[32m"
YELLOW  = "\033[33m"
BLUE    = "\033[34m"
WHITE   = "\033[37m"

IXX_CONFIG                          = " -std=c++23 {0} --precompile -o {1}"

IS_DEBUG:bool                       = False
DEBUG_MARCO:list[str]               = ["_DEBUG"]

BUILD_CXX_COMPILER:str              = "clang++"
BUILD_C_COMPILER:str                = "clang"

BUILD_CSHRAP_COMPILER:str           = "ilc"

IS_EXPORT_CSHRP_API:bool            = False
EXPORT_CSHARP_API_MARCO:list[str]   = ["EXPORT_C_SHARP_API"]

ENGINE_PROJECT_NAME                 = "Engine"
ENGINE_PROJECT_PATH                 = "../../csrc/Engine/"
ENGINE_MARCO:list[str]              = ["SPNG_STATIC"]
ENGINE_INCLUDE_PATH                 = ["../../csrc/Engine/Testure/","../../csrc/Engine/XeCore/","../../csrc/Engine/IO/","../../csrc/Engine/Graphical/Widget/","../../csrc/Engine/AssetsIO/","../../csrc/Engine/Graphical/Vulkan/","../../csrc/Engine/Graphical/3D/","../../csrc/Engine/Graphical/2D/","../../csrc/Engine/Graphical/","../../csrc/Engine/Audio/"]


PROJECT_BUILD_OUTPUT_PATH           = "../../SDK/{0}/"

MAKE_FILE_PATH                      = os.path.dirname(os.path.abspath(__file__))

def InitInput(argv_strs:list[str]) -> None:
    global IS_EXPORT_CSHRP_API
    global BUILD_CXX_COMPILER
    global BUILD_C_COMPILER
    for argv_str in argv_strs[1:]:
        if(argv_str == "-csharpapi"):
            IS_EXPORT_CSHRP_API = True
        elif(argv_str.find("-cxx_compiler=") != -1):
            BUILD_CXX_COMPILER = argv_str.replace("-cxx_compiler=", "")
            if(BUILD_CXX_COMPILER == "clang++"):
                continue
            elif(BUILD_CXX_COMPILER == "g++"):
                continue
            else:
                raise Exception("ERROR: CXX COMPILER <{0}> NOT SUPPORT".format(BUILD_CXX_COMPILER))
        elif(argv_str.find("-c_compiler=") != -1):
            BUILD_C_COMPILER = argv_str.replace("-c_compiler=", "")
            if(BUILD_C_COMPILER == "clang"):
                BUILD_C_COMPILER = "clang"
                continue
            elif(BUILD_C_COMPILER == "gcc"):
                BUILD_C_COMPILER = "gcc"
                continue
            else:
                raise Exception("ERROR: CXX COMPILER <{0}> NOT SUPPORT".format(BUILD_C_COMPILER))

def GetEngineIncludeAndMarco()->str:
    output:str = ""
    for opath in ENGINE_INCLUDE_PATH:
        output = output + " -i\"{0}\"".format(os.path.join(MAKE_FILE_PATH, opath))
    for marco in ENGINE_MARCO:
        output = output + " -D\"{0}\"".format(marco)
    if(IS_DEBUG):
        for marco in DEBUG_MARCO:
            output = output + "-D\"{0}\"".format(marco)
    return output

def GetAllFilePaths(directory):
    file_list:list[str] = []
    for root, dirs, files in os.walk(directory):
        for file in files:
            file_path = os.path.join(root, file)
            file_list.append(file_path)
    return file_list

def BuildEngine(config:str)->bool:
    global BUILD_C_COMPILER
    global BUILD_CXX_COMPILER
    global PROJECT_BUILD_OUTPUT_PATH
    global ENGINE_PROJECT_NAME

    THIS_OUTPUT_PATH = os.path.join(PROJECT_BUILD_OUTPUT_PATH , ENGINE_PROJECT_NAME) 
    THIS_OUTPUT_OBJ_PATH = os.path.join(THIS_OUTPUT_PATH, "obj")
    THIS_OUTPUT_PCM_PATH = os.path.join(THIS_OUTPUT_PATH, "pcm")

    if(not os.path.exists(THIS_OUTPUT_PATH)):
        os.mkdir(THIS_OUTPUT_PATH)
        os.mkdir(THIS_OUTPUT_OBJ_PATH)
        os.mkdir(THIS_OUTPUT_PCM_PATH)
    else:
        if(not os.path.exists(THIS_OUTPUT_OBJ_PATH)):
            os.mkdir(THIS_OUTPUT_OBJ_PATH)
        if(not os.path.exists(THIS_OUTPUT_PCM_PATH)):
            os.mkdir(THIS_OUTPUT_PCM_PATH)
    
    print("{0}------engine starting building------{1}".format(YELLOW, RESET))
    engine_src_files_list = GetAllFilePaths(os.path.join(MAKE_FILE_PATH, ENGINE_PROJECT_PATH))
    for engine_src_file in engine_src_files_list:
        src_file = os.path.basename(engine_src_file)
        if(src_file.find(".ixx") != -1):
            print("{0}build{2} {1}".format(GREEN, src_file, RESET))
            os.system(BUILD_CXX_COMPILER + " " + IXX_CONFIG.format(engine_src_file, os.path.join(THIS_OUTPUT_PCM_PATH, src_file.replace("ixx", "pcm"))))
        elif(src_file.find(".cpp") != -1):
            print("{0}build{2} {1}".format(GREEN, src_file, RESET))
        else:
            continue


if __name__ == "__main__":
    print("{1}current path:{2}\t\t{3}\"{0}\"{2}".format(MAKE_FILE_PATH, GREEN, RESET, BLUE))
    InitInput(sys.argv)
    print("{2}use cxx compiler:\t{3}{0}\n{2}use c compiler:\t\t{3}{1}".format(BUILD_CXX_COMPILER, BUILD_C_COMPILER, GREEN, RESET))
    if(IS_DEBUG):
        PROJECT_BUILD_OUTPUT_PATH = os.path.join(MAKE_FILE_PATH, PROJECT_BUILD_OUTPUT_PATH.format("debug"))
    else:
        PROJECT_BUILD_OUTPUT_PATH = os.path.join(MAKE_FILE_PATH, PROJECT_BUILD_OUTPUT_PATH.format("release"))
    if(not os.path.exists(PROJECT_BUILD_OUTPUT_PATH)):
        os.mkdir(PROJECT_BUILD_OUTPUT_PATH)
    print("output path: {0}{1}{2}".format(YELLOW, PROJECT_BUILD_OUTPUT_PATH, RESET))
    ENGINE_CONFIG_LIST = GetEngineIncludeAndMarco()
    BuildEngine(ENGINE_CONFIG_LIST)