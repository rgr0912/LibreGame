#include "LibreGame.h"

LibreGame::LibreGame()
{
}
void LibreGame::initApp()
{
    crearRoot();
}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
void LibreGame::initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window)
{
    mAConfig = AConfiguration_new();
    AConfiguration_fromAssetManager(mAConfig, assetMgr);
    mAAssetMgr = assetMgr;

    mWindows.resize(1);
    mWindows[0].native = window;
    crearRoot();
}
#endif