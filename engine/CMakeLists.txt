cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.0.0)

set(ENGINE_HEADER_FILES
include/engine/test_cmake.hxx
include/engine/application.hxx
include/engine/window/window.hxx
include/engine/graphics/render.hxx
include/engine/graphics/renderAPI.hxx
)

set(ENGINE_SOURCE_FILES
source/engine/test_cmake.cxx
source/engine/application.cxx
source/engine/window/window.cxx
source/engine/graphics/render.cxx
source/engine/graphics/renderAPI.cxx
)

# Metal graphics backend
list (APPEND ENGINE_HEADER_FILES
include/engine/graphics/backends/metal/renderAPIMetal.hxx
)
list(APPEND ENGINE_SOURCE_FILES
source/engine/graphics/backends/metal/renderAPIMetal.cxx
)

# GLFW window backend
list (APPEND ENGINE_HEADER_FILES
    include/engine/adapters/glfw_adapter.h
    include/engine/window/backends/glfw/glfwWindow.hxx
)
list(APPEND ENGINE_SOURCE_FILES
    source/engine/adapters/glfw_adapter.mm
    source/engine/window/backends/glfw/glfwWindow.cxx
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(Engine STATIC
${ENGINE_HEADER_FILES}
${ENGINE_SOURCE_FILES}
)

target_link_libraries(Engine PUBLIC METAL_CPP)
target_link_libraries(Engine PRIVATE glfw)
target_include_directories(Engine PUBLIC ../thirdparty/glfw/include)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)