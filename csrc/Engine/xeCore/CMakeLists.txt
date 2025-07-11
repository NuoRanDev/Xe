CMAKE_MINIMUM_REQUIRED(VERSION 3.25)

project(xeCore CXX)
set(CMAKE_CXX_STANDARD 23)

# Enable the module experiment
set(CMAKE_EXPERIMENTAL_CXX_MODULE_DYNDNS ON)

add_library(xeCore STATIC)
target_sources(
        xeCore
        PRIVATE
        xeApplication.cpp
        xeClock.cpp
        xeCoreClrOutput.cpp
        xeMemCmp.cpp
        xeString.cpp
        PUBLIC
        FILE_SET all_my_modules TYPE CXX_MODULES FILES
        xeAlloc.cppm
        xeApplication.cppm
        xeBaseDataType.cppm
        xeClock.cppm
        xeCoreClrOutput.cppm
        XeCoreCsharp.cppm
        xeMemCmp.cppm
        xeObject.cppm
        xeOrdinals.cppm
        xeString.cppm
        xeUnit.cppm
)