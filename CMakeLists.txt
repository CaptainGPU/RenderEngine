cmake_minimum_required(VERSION 3.25)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMALE_CXX_STANDART_REQUIRED ON)

# SELECT PLATFORM

set(DEPLOY_PLATFORM "None" CACHE STRING "Platform to deploy CMake project")

if (${DEPLOY_PLATFORM} STREQUAL "None")
    message( FATAL_ERROR "Not select Platform to deploy CMake project" )
endif()

if (${DEPLOY_PLATFORM} STREQUAL "Win")
    set(CURRENT_PLATFORM_ID 1)
elseif(${DEPLOY_PLATFORM} STREQUAL "MacOS")
    set(CURRENT_PLATFORM_ID 2)
elseif(${DEPLOY_PLATFORM} STREQUAL "Emscripten")
    set(CURRENT_PLATFORM_ID 3)
endif()

message(STATUS "-------------------------------------------------")

message(STATUS "Hello, let's build a Render Engine for the ${DEPLOY_PLATFORM} platform!")

# ENGINE PATH
set(ENGINE_SOURCE_PATH "${PROJECT_SOURCE_DIR}/engine/source")
set(ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/engine/include")
set(ENGINE_GENERATE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/generated")


add_executable(${PROJECT_NAME} "${ENGINE_SOURCE_PATH}/main.cpp")

add_subdirectory(engine)

target_link_libraries(${PROJECT_NAME} Engine)
target_include_directories(${PROJECT_NAME} PRIVATE ${Engine_SOURCE_DIR}/include())