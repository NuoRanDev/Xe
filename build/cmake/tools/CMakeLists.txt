add_library(Tools)

target_sources(Tools
    PRIVATE
        ../../../src/tools/xeFileDialog.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ../../../inc/tools
        FILES
            ../../../inc/tools/xeFileDialog.hpp)

target_link_libraries(Tools
    PRIVATE
        DataSystem
        Core
        Rfd)

target_compile_features(Tools PRIVATE cxx_std_20)