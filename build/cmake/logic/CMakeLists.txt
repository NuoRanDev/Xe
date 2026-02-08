add_library(Logic)

target_sources(Logic
    PRIVATE
        ../../../src/logic/graph/graph.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ../../../inc/logic
        FILES
            ../../../inc/logic/graph/graph.hpp)

target_link_libraries(Logic
    PRIVATE
        DataSystem
        Core)

target_compile_features(Logic PRIVATE cxx_std_20)
