#include "LibreGame.h"

class GameApp : public LibreGame{
    public:
    GameApp();
    void escena();
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    virtual void initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window);
#endif
    btRigidBody* miEnt;
    btRigidBody*  miEnt2;
    //lgEntidad2* ent2;
    lgEntidad2* ent3;
};