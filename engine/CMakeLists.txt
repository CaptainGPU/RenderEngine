cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.0.0)

set(ENGINE_HEADER_FILES
include/engine/test_cmake.hxx
)

set(ENGINE_SOURCE_FILES
source/engine/test_cmake.cxx
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(Engine STATIC
${ENGINE_HEADER_FILES}
${ENGINE_SOURCE_FILES}
)

target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)