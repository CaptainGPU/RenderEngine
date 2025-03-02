#!/bin/bash

PLATFORM_ID=$1
GENERATOR=$2

build_folder_path="../build"

if [ -d "$build_folder_path" ]; then
    echo "Folder \"$build_folder_path\" is present. Wait to clear."
    rm -rf "$build_folder_path"
else
    echo "Folder \"$build_folder_path\" not found. Wait to create project."
fi

mkdir "$build_folder_path"

cmake ../ -DDEPLOY_PLATFORM="$1" -DDEPLOY_GRAPHICS_API=Vulkan -G "$2" -B"$build_folder_path"

echo "Final, try Xcode in ${project_dir}/build folder"