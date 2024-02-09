//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "fileSystem.hxx"

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <vector>
#include <string>
#endif

void mountFileSystem()
{
#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
    mountFileSystemEmscripten();
#endif
}

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
void mountFileSystemEmscripten()
{
    printf("Start mounting file system for Emscripten\n");

    std::vector<std::string> files = {
        "/shaders/WebGL/mesh.frag",
        "/shaders/WebGL/mesh.vert",
        "/shaders/WebGL/meshBound.frag",
        "/shaders/WebGL/meshBound.vert",
        "/shaders/WebGL/fullScreen.frag",
        "/shaders/WebGL/fullScreen.vert",
        "/shaders/WebGL/light.frag",
        "/shaders/WebGL/light.vert",
        "/shaders/WebGL/sunLightShadow.vert",
        "/shaders/WebGL/sunLightShadow.frag",
        "/shaders/WebGL/pointLightShadow.vert",
        "/shaders/WebGL/pointLightShadow.frag",
        "/Models/monkey.mesh",
        "/Models/monkey_hd.mesh",
        "/Models/quad.mesh",
        "/Models/light.mesh",
        "/Models/crash_normal.mesh",
        "/Models/monkey_normal.mesh",
        "/Models/plane.mesh",
        "/Models/spot.mesh",
        "/Models/sun.mesh",
        "/Models/scene74.mesh",
        "/Models/scene74.mtl",
        "/Models/gltf/CubeGLTF.gltf"
    };

    for (size_t i = 0; i < files.size(); i++)
    {
        int result = emscripten_wget(files[i].c_str(), files[i].c_str());
        if (result == 0)
        {
            printf("upload: %s\n", files[i].c_str());
        }
        else
        {
            printf("error: %s\n", files[i].c_str());
        }
    }

    printf("End mounting file system for Emscripten\n");
}
#endif