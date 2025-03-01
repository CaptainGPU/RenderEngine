cmake_minimum_required(VERSION 3.25)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMALE_CXX_STANDART_REQUIRED ON)

# SELECT PLATFORM

set(PLATFORM_WIN 1)
set(PLATFORM_MACOS 2)
set(PLATFORM_EMSCRIPTEN 3)

set(DEPLOY_PLATFORM "None" CACHE STRING "Platform to deploy CMake project")

if (${DEPLOY_PLATFORM} STREQUAL "None")
    message( FATAL_ERROR "Not select Platform to deploy CMake project" )
endif()

if (${DEPLOY_PLATFORM} STREQUAL "Win")
    set(CURRENT_PLATFORM_ID PLATFORM_WIN)
elseif(${DEPLOY_PLATFORM} STREQUAL "MacOS")
    set(CURRENT_PLATFORM_ID PLATFORM_MACOS)
elseif(${DEPLOY_PLATFORM} STREQUAL "Emscripten")
    set(CURRENT_PLATFORM_ID PLATFORM_EMSCRIPTEN)
endif()

message(STATUS "-------------------------------------------------")

message(STATUS "Hello, let's build a Render Engine for the ${DEPLOY_PLATFORM} platform!")

# ENGINE PATH
set(ENGINE_SOURCE_PATH "${PROJECT_SOURCE_DIR}/engine/source")
set(ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/engine/include")
set(ENGINE_GENERATE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/generated")

add_subdirectory(engine)
add_subdirectory(thirdparty/glfw)

add_executable(${PROJECT_NAME} "${ENGINE_SOURCE_PATH}/main.cpp")

target_link_libraries(${PROJECT_NAME} PRIVATE Engine)

if (${CURRENT_PLATFORM_ID} EQUAL ${PLATFORM_MACOS})
    find_package(OpenGL REQUIRED)
    target_link_libraries(${PROJECT_NAME} PRIVATE OpenGL::GL)
endif()