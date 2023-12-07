mkDir "../build"
cd "../build"

cmake ../ -DDEPLOY_PLATFORM=Win -G "Visual Studio 16 2019"

echo "Final, try Visual Studio 16 2019 Solution in ${project_dir}/build folder"