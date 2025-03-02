cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.0.0)

set(CMAKE_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../cmake_tools")
include("${CMAKE_TOOLS_DIR}/consts.cmake")

message(STATUS "Parent Directory: ${CMAKE_SOURCE_DIR}/")

get_property(PLATFORM_WIN DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY PLATFORM_WIN)
get_property(PLATFORM_MACOS DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY PLATFORM_MACOS)
get_property(PLATFORM_EMSCRIPTEN DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY PLATFORM_EMSCRIPTEN)

# Configure defines.hxx
get_property(ENGINE_PLATFORM_ID DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY CURRENT_PLATFORM_ID)
get_property(ENGINE_GRAPHICS_API DIRECTORY ${CMAKE_SOURCE_DIR} PROPERTY CURRENT_GRAPHICS_API)
message(STATUS "ENGINE_PLATFORM_ID: ${ENGINE_PLATFORM_ID}")
message(STATUS "ENGINE_GRAPHICS_API: ${ENGINE_GRAPHICS_API}")
configure_file(
    "include/engine/defines.hxx.in"
    "${CMAKE_CURRENT_SOURCE_DIR}/include/engine/defines.hxx"
)
list(APPEND ENGINE_HEADER_FILES "${CMAKE_CURRENT_SOURCE_DIR}/include/engine/defines.hxx")

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

if (${ENGINE_PLATFORM_ID} EQUAL ${PLATFORM_MACOS})
# Metal graphics backend
list (APPEND ENGINE_HEADER_FILES
    include/engine/graphics/backends/metal/renderAPIMetal.hxx
)
list(APPEND ENGINE_SOURCE_FILES
    source/engine/graphics/backends/metal/renderAPIMetal.cxx
)
endif()

# GLFW window backend
list (APPEND ENGINE_HEADER_FILES
    include/engine/window/backends/glfw/glfwWindow.hxx
)
list(APPEND ENGINE_SOURCE_FILES
    source/engine/window/backends/glfw/glfwWindow.cxx
)

# GLFW Metal/Cocoa Adapter
if (${ENGINE_PLATFORM_ID} EQUAL ${PLATFORM_MACOS})
list (APPEND ENGINE_HEADER_FILES
    include/engine/adapters/glfw_adapter.h
)
list(APPEND ENGINE_SOURCE_FILES
    source/engine/adapters/glfw_adapter.mm
)
endif()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(Engine STATIC
    ${ENGINE_HEADER_FILES}
    ${ENGINE_SOURCE_FILES}
)

if (${ENGINE_PLATFORM_ID} EQUAL ${PLATFORM_MACOS})
    target_link_libraries(Engine PUBLIC METAL_CPP)
endif()
target_link_libraries(Engine PRIVATE glfw)
target_include_directories(Engine PUBLIC ../thirdparty/glfw/include)
target_include_directories(Engine PUBLIC ${CMAKE_CURRENT_BINARY_DIR}) # Изменено

target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)