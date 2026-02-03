add_library(Core)

find_package(mimalloc REQUIRED)

target_sources(Core
    PRIVATE
        ../../../src/core/devboost/xeMemCmp.cpp
        ../../../src/core/devboost/xeMemFind.cpp
        ../../../src/core/filesystem/xeFileMmapStream.cpp
        ../../../src/core/filesystem/xePath.cpp
        ../../../src/core/log/xeLogOutpout.cpp
        ../../../src/core/memory/xeAlloc.cpp
        ../../../src/core/string/xeString.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ../../../inc/core
        FILES
            ../../../inc/core/xeCompiler.hpp
            ../../../inc/core/devboost/xeMemCmp.hpp
            ../../../inc/core/devboost/xeMemFind.hpp
            ../../../inc/core/filesystem/xeFileMmapStream.hpp
            ../../../inc/core/filesystem/xeFilesystem.hpp
            ../../../inc/core/filesystem/xePath.hpp
            ../../../inc/core/log/xeLogOutput.hpp
            ../../../inc/core/math/xeBigNumber.hpp
            ../../../inc/core/memory/xeAlloc.hpp
            ../../../inc/core/memory/xeDiskMemoryPool.hpp
            ../../../inc/core/string/xeRegexp.hpp
            ../../../inc/core/string/xeString.hpp
            ../../../inc/core/type/xeDataStruction.hpp
            ../../../inc/core/type/xeOrdinals.hpp)

target_compile_features(Core PRIVATE cxx_std_20)