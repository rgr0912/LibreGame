#include "lgOgre.h"
#include "lgSDL.h"
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/configuration.h>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/input.h>
#endif

class LibreGame : public lgOgre, public lgSDL
{
public:
    LibreGame();
    void initApp();
    #if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    void initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window);
    #endif
};