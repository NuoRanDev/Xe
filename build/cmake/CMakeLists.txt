cmake_minimum_required(VERSION 3.23)

project(XE)

option(IS_BUILD_SAMPLE "Build the Sample executable" OFF)

if (MSVC)
    add_compile_options(/arch:AVX2)
else ()
    add_compile_options(-mavx2)
endif ()

message("Start to build Core ...")

add_subdirectory(core)