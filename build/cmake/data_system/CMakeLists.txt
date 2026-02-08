add_library(DataSystem)

find_package(PNG REQUIRED)

target_sources(DataSystem
    PRIVATE
        ../../../src/data_system/file/xeMemoryFile.cpp
        ../../../src/data_system/file/image/libjpegAddons.cpp
        ../../../src/data_system/file/image/libsnpngAddons.cpp
        ../../../src/data_system/image/xeImage.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ../../../inc/data_system
        FILES
            ../../../inc/data_system/file/xeMemoryFile.hpp
            ../../../inc/data_system/file/image/xeImgaeFile.hpp
            ../../../inc/data_system/file/image/xeReadImage.hpp
            ../../../inc/data_system/image/xeImage.hpp)

target_link_libraries(DataSystem
    PRIVATE
        Core)

target_compile_features(DataSystem PRIVATE cxx_std_20)