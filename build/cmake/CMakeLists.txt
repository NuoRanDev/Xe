cmake_minimum_required(VERSION 3.23)

project(XE)

option(IS_BUILD_SAMPLE "Build the Sample executable" ON)

if (MSVC)
    add_compile_options(/arch:AVX2)
else ()
    add_compile_options(-mavx2)
endif ()

message("Start to build Core ...")

add_subdirectory(core)

add_subdirectory(data_system)

add_subdirectory(Rfd)

add_subdirectory(logic)

add_subdirectory(tools)

add_subdirectory(graphics)

if(IS_BUILD_SAMPLE)
    add_subdirectory(sample)
endif()