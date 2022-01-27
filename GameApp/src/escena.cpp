#include "GameApp.h"
GameApp::GameApp()
{
}
void GameApp::escena()
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    initApp();
#endif
}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/permission_manager.h>
void permisos()
{
    int32_t APermissionManager_checkPermission(
        const char *permission,
        pid_t pid,
        uid_t uid,
        int32_t *outResult);
}
void GameApp::initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window)
{
    lgConfigAndroid = AConfiguration_new();
    AConfiguration_fromAssetManager(lgConfigAndroid, assetMgr);
    lgManagerAndroid = assetMgr;
    permisos();

    ventana_lista.resize(1);
    ventana_lista[0].ventana_nativa = window;
    initApp();
}
#endif