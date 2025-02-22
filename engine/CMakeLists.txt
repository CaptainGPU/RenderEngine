cmake_minimum_required(VERSION 3.25)
project(Engine VERSION 1.0.0)

file(GLOB_RECURSE HEADER_FILES "include/*.hxx")

message(STATUS "Найденные .hxx файлы:")
foreach(FILE ${HEADER_FILES})
    message(STATUS "  ${FILE}")
endforeach()

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_library(Engine STATIC
    source/engine/test_cmake.cxx
)

target_include_directories(Engine
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)