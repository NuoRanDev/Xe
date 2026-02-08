add_library(Rfd INTERFACE)

target_include_directories(Rfd INTERFACE ../../../rust/inc)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(XE_RUST_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../rust/target/debug/libxeRustLib.a)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(XE_RUST_LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../../rust/target/release/libxeRustLib.a)
endif()

target_link_libraries(Rfd INTERFACE ${XE_RUST_LIB_PATH})