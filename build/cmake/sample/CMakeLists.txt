add_executable(Sample)

target_sources(Sample PRIVATE ../../../test/game_main.cpp)

target_link_libraries(Sample
    PRIVATE
        Core
        Logic
        DataSystem
        Rfd
        Tools
        Graphics)

target_compile_features(Sample PRIVATE cxx_std_20)

if(
  (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC") OR
  (CMAKE_CXX_COMPILER_FRONTEND_VARIANT STREQUAL "MSVC")
)

target_compile_options(Sample PRIVATE /W3)
elseif(
  (CMAKE_CXX_COMPILER_ID STREQUAL "GNU") OR
  (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
)

target_compile_options(Sample PRIVATE -Wall)

endif()