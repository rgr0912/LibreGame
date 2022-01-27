cmake_minimum_required(VERSION 3.0.0)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)  

project(GameApp VERSION 0.1.0)
option(EXPORT_GAME "Compilar juego" OFF)
if(EXPORT_GAME)
set(EXPORTAR 1)
else()
set(EXPORTAR 0)
endif()

configure_file(SDK/include/config.h.in ${CMAKE_CURRENT_SOURCE_DIR}/SDK/include/config.h @ONLY)
configure_file(SDK/configsys.xml ${CMAKE_BINARY_DIR}/SDK/configsys.xml COPYONLY)
configure_file(SDK/escena.dtd ${CMAKE_BINARY_DIR}/SDK/escena.dtd COPYONLY)


if (ANDROID)
add_subdirectory(${PROJECT_SOURCE_DIR}/Base/ogre-master)
else()
add_subdirectory(${PROJECT_SOURCE_DIR}/Base/ogre-master)
endif()
add_subdirectory(${PROJECT_SOURCE_DIR}/Base/openal-soft-1.21.1)
add_subdirectory(${PROJECT_SOURCE_DIR}/Base/vorbis-1.3.7)
add_subdirectory(${PROJECT_SOURCE_DIR}/SDK)


set(CMAKE_MODULE_PATH
  "${PROJECT_SOURCE_DIR}/SDK/CMake"
  "${PROJECT_SOURCE_DIR}/SDK/CMake/Utils"
  "${PROJECT_SOURCE_DIR}/SDK/CMake/Packages"
)
set(OGRE_TEMPLATES_DIR "${PROJECT_SOURCE_DIR}/SDK/CMake/Templates")
set(OGRE_WORK_DIR ${PROJECT_BINARY_DIR})
file(GLOB ESCENA ${PROJECT_SOURCE_DIR}/GameApp/src/escena.cpp)
if (ANDROID)
    include(AndroidMacros)

    add_ndk_cpufeatures_library()
    add_ndk_native_app_glue_library()

    include_directories(${PROJECT_SOURCE_DIR}/RenderSystems/GLSupport/include)
    set(SOURCE_FILES ${PROJECT_SOURCE_DIR}/GameApp/src/gestureDetector.cpp
    ${PROJECT_SOURCE_DIR}/GameApp/src/android_main.cpp)
    add_library(GameApp SHARED ${HEADER_FILES} ${SOURCE_FILES} ${RESOURCE_FILES} ${ESCENA})
    
else()
add_executable(GameApp GameApp/src/main.cpp ${ESCENA}) 
target_link_libraries(GameApp LibreGame)
install(TARGETS GameApp DESTINATION "${CMAKE_INSTALL_PREFIX}/bin")
endif()
if(ANDROID)
    target_link_libraries(GameApp native_app_glue cpufeatures android c m dl z log EGL GLESv2 "-u ANativeActivity_onCreate")
    target_link_libraries(GameApp OgreMain OgreBites OgreRTShaderSystem OgreTerrain)
    target_link_libraries(GameApp LibreGame)
    target_link_libraries(GameApp ${log-lib})
    set_target_properties(GameApp PROPERTIES LINK_FLAGS -s) # strip symbols

    SET(ANDROID_MOD_NAME "GameApp")
    SET(NDKOUT "${CMAKE_BINARY_DIR}/GameAppNDK")
    SET(PKG_NAME "org.ogre.gameapp")
    SET(HAS_CODE "false")
    SET(MAIN_ACTIVITY "android.app.NativeActivity")
    SET(HEADERS "")
    SET(SAMPLE_LDLIBS "")

    copy_assets_to_android_proj()

    file(MAKE_DIRECTORY "${NDKOUT}/app/src/main/jniLibs/${ANDROID_NDK_ABI_NAME}")
    set_target_properties(GameApp PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${NDKOUT}/app/src/main/jniLibs/${ANDROID_NDK_ABI_NAME}")
    set_target_properties(GameApp PROPERTIES OUTPUT_NAME "GameApp")

    create_android_proj(GameApp)
    # this whole part should be simplified more
endif(ANDROID)

target_include_directories(GameApp PUBLIC ${CMAKE_SOURCE_DIR}/GameApp/include)
target_include_directories(GameApp PUBLIC ${CMAKE_SOURCE_DIR}/SDK/include)
