cmake_minimum_required(VERSION 3.25)

include(FetchContent)

project(RenderEngine VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)

# SELECT PLATFORM

set(DEPLOY_PLATFORM "None" CACHE STRING "Platform to deploy CMake project")

if (EMSCRIPTEN)
    set(DEPLOY_PLATFORM "Emscripten")
endif()

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



# GLFW, GLAD, IMGUI, GLM LIBS
set(IGG_COMPONENTS "IMGUI;GLFW;GLAD;GLM;TINYOBJ" CACHE STRING "The components to import and build")


message(STATUS "-------------------------------------------------")
message(STATUS "Component Status")

# IMPORT ALL LIBS FOR DEFAULTS
foreach (comp IN ITEMS ${IGG_COMPONENTS})
    set(IMPORT_${comp} ON)
endforeach ()

# IMGUI LIB REQUIRE GLAD AND GLFW
if (IMPORT_IMGUI)
    set(IMPORT_GLAD ON)
    set(IMPORT_GLFW ON)
endif ()

if (EMSCRIPTEN)
    set(IMPORT_GLAD OFF)
    set(IMPORT_GLFW OFF)
endif()

#GLAD PARAMS
if (IMPORT_GLAD)
    option(IGG_GLAD_DOWNLOAD "Download GLAD?" ON)
    if (IGG_GLAD_DOWNLOAD)
        set(IGG_GLAD_GL_VERSION "4.5" CACHE STRING "The target gl version")
        option(IGG_GLAD_GL_CORE "The target gl profile. ON = core profile, OFF = compatibility profile" ON)
        if (IGG_GLAD_GL_CORE)
            set(GLAD_GL_PROFILE core)
        else ()
            set(GLAD_GL_PROFILE compatibility)
        endif ()
    else ()
        unset(IGG_GLAD_GL_VERSION CACHE)
        unset(IGG_GLAD_GL_CORE CACHE)
    endif ()
else ()
    unset(IGG_GLAD_DOWNLOAD CACHE)
    unset(IGG_GLAD_GL_VERSION CACHE)
    unset(IGG_GLAD_GL_CORE CACHE)
endif ()

# PRINT GLAD INFO
message(STATUS "Import Glad - ${IMPORT_GLAD}")

if (IMPORT_GLAD)
    message(STATUS "Glad OpenGL Version - ${IGG_GLAD_GL_VERSION}")
    message(STATUS "Glad OpenGL Core - ${IGG_GLAD_GL_CORE}")
    message(STATUS "Glad OpenGL Profile - ${GLAD_GL_PROFILE}")
endif()

# IMGUI PARAMS
if (IMPORT_IMGUI)
    set(IGG_IMGUI_TAG master CACHE STRING "Defines the imgui version (e.g. master, v1.89.4, v1.89.3, ...")
    if (NOT EMSCRIPTEN)
        set(IMPORT_GLFW ON)
    endif()
else ()
    unset(IGG_IMGUI_TAG CACHE)
endif ()

# PRINT IMGUI INFO
message(STATUS "Import ImGui - ${IMPORT_IMGUI} | branch - ${IGG_IMGUI_TAG}")

# GLFW INFO
if (IMPORT_GLFW)
    set(IGG_GLFW_TAG master CACHE STRING "Defines the glfw version (e.g. master, 3.3.8, 3.3.7, ...) ")
else ()
    unset(IGG_GLFW_TAG CACHE)
endif ()

# PRING GLFW INFO
message(STATUS "Import GLFW - ${IMPORT_GLFW} | branch - ${IGG_GLFW_TAG}")

# GLM PARAMS
if (IMPORT_GLM)
    set(IGG_GLM_TAG 0.9.9.8 CACHE STRING "Defines the glm version (e.g. master, 0.9.9.8, 0.9.9.7, ...")
else ()
    unset(IGG_GLM_TAG CACHE)
endif ()

# PRING GLM INFO
message(STATUS "Import GLM - ${IMPORT_GLM} | branch - ${IGG_GLM_TAG}")

# GLM PARAMS
if (IMPORT_TINYOBJ)
    set(IGG_TINYOBJ_TAG release CACHE STRING "Defines the glm version (e.g. master, 0.9.9.8, 0.9.9.7, ...")
else ()
    unset(IGG_TINYOBJ_TAG CACHE)
endif ()

# PRING GLM INFO
message(STATUS "Import TinyOBJ - ${IMPORT_TINYOBJ} | branch - ${IGG_TINYOBJ_TAG}")

# DOWNLOAD GLAD
if (IMPORT_GLAD)
    message(STATUS "Download Glad")
    if (NOT IGG_GLAD_DOWNLOAD)
        message(STATUS "Using included version of the glad loader sources (gl 4.5/core) ")
        set(glad_SOURCE_DIR glad)
    else ()
        if ("${glad_INSTALLED_VERSION}" STREQUAL "${IGG_GLAD_GL_VERSION}-${GLAD_GL_PROFILE}")
            message(STATUS "Avoiding repeated download of glad gl ${IGG_GLAD_GL_VERSION}/${GLAD_GL_PROFILE}")
            set(glad_SOURCE_DIR ${glad_LAST_SOURCE_DIR})
        else ()
            find_program(IGG_CURL NAMES curl curl.exe)
            if (NOT IGG_CURL)
                message(STATUS "Could not find curl, using included version of the glad loader sources (gl 4.5/core)")
                set(glad_SOURCE_DIR glad)
            else ()
                execute_process(
                        COMMAND ${IGG_CURL} -s -D - -X POST -d generator=c&api=egl%3Dnone&api=gl%3D${IGG_GLAD_GL_VERSION}&profile=gl%3D${GLAD_GL_PROFILE}&api=gles1%3Dnone&profile=gles1%3Dcommon&api=gles2%3Dnone&api=glsc2%3Dnone&api=glx%3Dnone&api=vulkan%3Dnone&api=wgl%3Dnone&options=LOADER https://gen.glad.sh/generate
                        OUTPUT_VARIABLE out
                        RESULT_VARIABLE res
                        ERROR_VARIABLE err
                )
                if (NOT res EQUAL "0")
                    message(WARNING "${IGG_CURL} returned: " ${res})
                    if (err)
                        message(WARNING "Error message: " ${err})
                    endif ()
                    message(STATUS "Using included version of the glad loader sources (gl 4.5/core)")
                    set(glad_SOURCE_DIR glad)
                else ()
                    string(REGEX MATCH "[Ll][Oo][Cc][Aa][Tt][Ii][Oo][Nn]: ([A-Za-z0-9_\\:/\\.]+)" location ${out})
                    set(location "${CMAKE_MATCH_1}")
                    if (NOT location OR location STREQUAL "/")
                        message(WARNING "Could not extract location from http response, using included version of the glad loader sources (gl 4.5/core)")
                        message(STATUS "Response: " ${out})
                        set(glad_SOURCE_DIR glad)
                    else ()
                        string(REGEX REPLACE "/$" "" location ${location})
                        string(APPEND location "/glad.zip")
                        if (NOT ${location} MATCHES "^http")
                            string(PREPEND location "https://gen.glad.sh")
                        endif ()
                        message(STATUS "Downloading glad loader sources for gl${IGG_GLAD_GL_VERSION}/${GLAD_GL_PROFILE} from ${location}")
                        FetchContent_Declare(
                                glad
                                URL ${location}
                        )
                        FetchContent_MakeAvailable(glad)
                        set(glad_INSTALLED_VERSION ${IGG_GLAD_GL_VERSION}-${GLAD_GL_PROFILE} CACHE INTERNAL "")
                        set(glad_LAST_SOURCE_DIR ${glad_SOURCE_DIR} CACHE INTERNAL "")
                    endif ()
                endif ()
            endif ()
        endif ()
    endif ()

    add_library(
            glad
            ${glad_SOURCE_DIR}/src/gl.c
            ${glad_SOURCE_DIR}/include/glad/gl.h
            ${glad_SOURCE_DIR}/include/KHR/khrplatform.h
    )
    target_include_directories(glad PUBLIC ${glad_SOURCE_DIR}/include)
endif ()

# DOWNLOAD GLFW
if (IMPORT_GLFW)
    message(STATUS "Download GLFW")
    FetchContent_Declare(
            glfw
            GIT_REPOSITORY https://github.com/glfw/glfw.git
            GIT_TAG ${IGG_GLFW_TAG}
    )
    list(APPEND components glfw)
endif ()

# DOWNLOAD GLM
if (IMPORT_GLM)
    message(STATUS "Download GLM")
    FetchContent_Declare(
            glm
            GIT_REPOSITORY https://github.com/g-truc/glm.git
            GIT_TAG ${IGG_GLM_TAG}
    )
    list(APPEND components glm)
endif ()

# DOWNLOAD TINYOBJ
if (IMPORT_TINYOBJ)
    message(STATUS "Download TinyOBJ")
    FetchContent_Declare(
            tinyobjloader
            GIT_REPOSITORY https://github.com/tinyobjloader/tinyobjloader.git
            GIT_TAG ${IGG_TINYOBJ_TAG}
    )
    list(APPEND components tinyobjloader)
endif ()

# DOWNLOAD ImGui
if (IMPORT_IMGUI)
    message(STATUS "Download ImGui")
    FetchContent_Declare(
            imgui
            GIT_REPOSITORY https://github.com/ocornut/imgui.git
            GIT_TAG ${IGG_IMGUI_TAG}
    )
    list(APPEND components imgui)
endif ()

message(STATUS "-------------------------------------------------")
message(STATUS "Make Avaible for components: ${components}")
if (components)
    FetchContent_MakeAvailable(${components})
endif ()

##############################
#            imgui           #
##############################
if (IMPORT_IMGUI)
    file(GLOB imgui_SRC ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)
    add_library(
            imgui
            ${imgui_SRC}
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
            ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
    )
    if (NOT EMSCRIPTEN)
        target_link_libraries(imgui PUBLIC glfw glad)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DIMGUI_IMPL_OPENGL_LOADER_GLAD2")
    endif()
    target_include_directories(
            imgui
            PUBLIC
            ${imgui_SOURCE_DIR}
            ${imgui_SOURCE_DIR}/backends
    )
endif ()

##############################
# GTest                      #
##############################
set(GTEST_VERSION 1.14.0 CACHE STRING "Google test version")
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG "v${GTEST_VERSION}"
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

## APP

# ENGINE PATH
set(ENGINE_SOURCE_PATH "${PROJECT_SOURCE_DIR}/source")
set(ENGINE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")
set(ENGINE_GENERATE_INCLUDE_PATH "${PROJECT_SOURCE_DIR}/generated")

# PLATFORM PATH
set(SOURCE_PATH "${PROJECT_SOURCE_DIR}/source")
set(INCLUDE_PATH "${PROJECT_SOURCE_DIR}/include")

# ENGINE RECURSE FILES
file( GLOB_RECURSE EngineSources "${ENGINE_SOURCE_PATH}/*.cxx" )
file( GLOB_RECURSE EngineHeaders "${ENGINE_INCLUDE_PATH}/*.hXX" )

# GENERATE engine defines.hxx
configure_file("${ENGINE_GENERATE_INCLUDE_PATH}/defines.hxx.in" "${ENGINE_GENERATE_INCLUDE_PATH}/defines.hxx")

list(APPEND EngineHeaders "${ENGINE_GENERATE_INCLUDE_PATH}/defines.hxx")

# EMSCRIPTEN IMGUI SOURCES - ALTERNATIVE WAY (NO SUPPORT IN FUTURE)
# set(Emscripten_imgui_sources "")

# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui.cpp")
# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui_demo.cpp")
# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui_draw.cpp")
# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui_impl_glfw.cpp")
# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui_impl_opengl3.cpp")
# list(APPEND Emscripten_imgui_sources "${SOURCE_PATH}/imgui_widgets.cpp")

# set(Emscripten_imgui_headers "")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imconfig.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imgui.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imgui_impl_glfw.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imgui_impl_opengl3.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imgui_internal.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imstb_rectpack.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imstb_textedit.h")
# list(APPEND Emscripten_imgui_headers "${INCLUDE_PATH}/imstb_truetype.h")

# foreach(X IN LISTS Emscripten_imgui_sources)
#     list(APPEND EngineSources ${X})
# endforeach()

# foreach(X IN LISTS Emscripten_imgui_headers)
#     list(APPEND EngineHeaders ${X})
# endforeach()


############

# add_library(
#     myImGui
#     ${Emscripten_imgui_sources}
#     ${Emscripten_imgui_headers}
# )

# target_include_directories(
#     myImGui
#     PUBLIC
#     ${INCLUDE_PATH}
# )

# EMSCRIPTEN/OPENGL3/GLFW ONLY ImGui Lib

# file(GLOB imgui_SRC ${imgui_SOURCE_DIR}/*.cpp ${imgui_SOURCE_DIR}/*.h)

# add_library(
#     myImGui
#     ${imgui_SRC}
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.h
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.cpp
#     ${imgui_SOURCE_DIR}/backends/imgui_impl_glfw.h
# )

# target_include_directories(
#     myImGui
#     PUBLIC
#     ${imgui_SOURCE_DIR}
#     ${imgui_SOURCE_DIR}/backends
# )


############


add_executable(${PROJECT_NAME} ${EngineSources} ${EngineHeaders})

target_include_directories( ${PROJECT_NAME} PUBLIC ${ENGINE_INCLUDE_PATH} ${ENGINE_GENERATE_INCLUDE_PATH} )

message(STATUS "-------------------------------------------------")

message(STATUS "Engine Sources:")
foreach(X IN LISTS EngineSources)
    message(STATUS "${X}")
endforeach()

message(STATUS "")
message(STATUS "Engine Headers:")

foreach(X IN LISTS EngineHeaders)
    message(STATUS "${X}")
endforeach()


if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "-s USE_GLFW=3 -s FULL_ES3=1 -sASYNCIFY")
endif()

find_package(OpenGL REQUIRED)

target_link_libraries(
    ${PROJECT_NAME}
    imgui
    glm
    tinyobjloader
)

##############################
# Tests                      #
##############################
add_executable(${PROJECT_NAME}_tests tests/application_test.cxx)
target_include_directories(${PROJECT_NAME}_tests PRIVATE ${INCLUDE_PATH} )
target_link_libraries(${PROJECT_NAME}_tests gtest gtest_main)

enable_testing()
add_test(NAME ${PROJECT_NAME}_tests COMMAND ${PROJECT_NAME}_tests)

message(STATUS "-------------------------------------------------")
