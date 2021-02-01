cmake_minimum_required(VERSION 3.0.0)
project(LibreGame VERSION 0.1.0)

file(GLOB SDK ${CMAKE_SOURCE_DIR}/SDK/src/*.cpp)

add_library(LibreGame src/LibreGame.cpp ${SDK})
target_link_libraries(LibreGame OgreMain OgreBites OgreRTShaderSystem OgreTerrain)
target_include_directories(LibreGame PUBLIC ${CMAKE_SOURCE_DIR}/SDK/include)