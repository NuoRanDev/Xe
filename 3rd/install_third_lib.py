_3rd_library ={
    "mimalloc":["https://github.com/microsoft/mimalloc.git","/include/",""],
    "glfw":["https://github.com/glfw/glfw.git" , "/include/","/build/src/"],
}



windos_build_cli = {"Debug":"-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDebugDLL",
              "Release":"-DCMAKE_MSVC_RUNTIME_LIBRARY=MultiThreadedDLL"}

cmake_build_cli = "cmake {0} -B build -G Ninja {}"
make_cli = "cmake --build build --config {}"