add_library(Graphics)

find_package(Vulkan REQUIRED)

find_package(SDL3 REQUIRED)

target_sources(Graphics
    PRIVATE
        ../../../src/graphics/graphicsAPI/xeVkConfig.cpp
        ../../../src/graphics/graphicsAPI/xeVkContext.cpp
        ../../../src/graphics/graphicsAPI/xeVkSwapChain.cpp
        ../../../src/graphics/testure/xeTesture.cpp
        ../../../src/graphics/window/xeWindow.cpp
        ../../../src/graphics/window/xeWindowManager.cpp
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS
            ../../../inc/graphics
        FILES
            ../../../inc/graphics/xeSurface.hpp
            ../../../inc/graphics/graphicsAPI/xeVkConfig.hpp
            ../../../inc/graphics/graphicsAPI/xeVkContext.hpp
            ../../../inc/graphics/graphicsAPI/xeVkSwapChain.hpp
            ../../../inc/graphics/shader/xeFragmentShader.hpp
            ../../../inc/graphics/testure/xeTesture.hpp
            ../../../inc/graphics/window/xeWindow.hpp
            ../../../inc/graphics/window/xeWindowManager.hpp)

target_link_libraries(Graphics
    PRIVATE
        Vulkan::Vulkan
        SDL3::SDL3
        DataSystem
        Core
        Logic
        Tools)

target_compile_features(Graphics PRIVATE cxx_std_20)