cmake_minimum_required(VERSION 3.0.0)
project(GameApp VERSION 0.1.0)

add_subdirectory(${PROJECT_SOURCE_DIR}/Base/ogre-master)

add_executable(GameApp GameApp/src/main.cpp) 