#include "LibreGame.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
void LibreGame::initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window)
{
    lgConfigAndroid = AConfiguration_new();
    AConfiguration_fromAssetManager(lgConfigAndroid, assetMgr);
    lgManagerAndroid = assetMgr;

    ventana_lista.resize(1);
    ventana_lista[0].ventana_nativa = window;
    initApp();
}
#endif