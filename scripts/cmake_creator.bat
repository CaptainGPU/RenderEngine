@echo off

set build_folder_path=..\build

if exist "%build_folder_path%" (
    echo Folder "%build_folder_path%" is present. Wait to clear.
    rmdir /s /q "%build_folder_path%"
) else (
    echo Folder "%build_folder_path%" not found. Wait to create project.
)

mkDir "%build_folder_path%"

cmake ../ -DDEPLOY_PLATFORM=%1 -G %2 -B"%build_folder_path%"

@REM echo "Final, try Visual Visual Studio 17 2022 in ${project_dir}/build folder"