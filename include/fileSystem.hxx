//
// Created by Eugene Karpenko @ CaptainGPU
// https://twitter.com/CaptainGPU
//

#include "defines.hxx"

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
#include <iostream>
#include <string>
#include <fstream>
#include <emscripten.h>
#endif

void mountFileSystem();

#if CURRENT_PLATFORM == PLATFORM_EMSCRIPTEN
void mountFileSystemEmscripten();
#endif