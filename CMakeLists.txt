cmake_minimum_required(VERSION 3.25)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMALE_CXX_STANDART_REQUIRED ON)

# SELECT PLATFORM

set(CMAKE_TOOLS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake_tools")
include("${CMAKE_TOOLS_DIR}/consts.cmake")

set(DEPLOY_PLATFORM "None" CACHE STRING "Platform to deploy CMake project")

if (${DEPLOY_PLATFORM} STREQUAL "None")
    message( FATAL_ERROR "Not select Platform to deploy CMake project" )
endif()

if (${DEPLOY_PLATFORM} STREQUAL "Win")
    set(CURRENT_PLATFORM_ID ${PLATFORM_WIN})
elseif(${DEPLOY_PLATFORM} STREQUAL "MacOS")
    set(CURRENT_PLATFORM_ID ${PLATFORM_MACOS})
elseif(${DEPLOY_PLATFORM} STREQUAL "Emscripten")
    set(CURRENT_PLATFORM_ID ${PLATFORM_EMSCRIPTEN})
endif()

# SELECT GRAPHICS API

set(DEPLOY_GRAPHICS_API "None" CACHE STRING "Graphics API in project")

if (${DEPLOY_GRAPHICS_API} STREQUAL "Metal")
    set(CURRENT_GRAPHICS_API ${GRAPHICS_API_METAL})
elseif(${DEPLOY_GRAPHICS_API} STREQUAL "Vulkan")
    set(CURRENT_GRAPHICS_API ${GRAPHICS_API_VULKAN})
endif()

message(STATUS "-------------------------------------------------")

message(STATUS "Hello, let's build a Render Engine for the ${DEPLOY_PLATFORM} platform!")

# ENGINE PATH
set(ENGINE_SOURCE_PATH "${PROJECT_SOURCE_DIR}/engine/source")
set(ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/engine/include")
set(ENGINE_GENERATE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/generated")

set_property(DIRECTORY PROPERTY PLATFORM_WIN ${PLATFORM_WIN})
set_property(DIRECTORY PROPERTY PLATFORM_MACOS ${PLATFORM_MACOS})
set_property(DIRECTORY PROPERTY PLATFORM_EMSCRIPTEN ${PLATFORM_EMSCRIPTEN})
set_property(DIRECTORY PROPERTY CURRENT_PLATFORM_ID ${CURRENT_PLATFORM_ID})
set_property(DIRECTORY PROPERTY CURRENT_GRAPHICS_API ${CURRENT_GRAPHICS_API})
set_property(DIRECTORY PROPERTY GRAPHICS_API_METAL ${GRAPHICS_API_METAL})
set_property(DIRECTORY PROPERTY GRAPHICS_API_VULKAN ${GRAPHICS_API_VULKAN})

if (${CURRENT_GRAPHICS_API} EQUAL ${GRAPHICS_API_METAL})
add_subdirectory(thirdparty/metal-cpp)
endif()

add_subdirectory(engine)
add_subdirectory(thirdparty/glfw)

add_executable(${PROJECT_NAME} "${ENGINE_SOURCE_PATH}/main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

# if (${CURRENT_PLATFORM_ID} EQUAL ${PLATFORM_MACOS})
#     find_package(OpenGL REQUIRED)
#     target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
# endif()

set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT RenderEngine)