/*
--------------------------------------------------------------------------------------------
LibreGame engine es un proyecto de software libre basado en difetentes librerias OpenSourcce
este puede ser utilizado siempre y cuando tenga en cuenta que es un proyecto en fase beta, y 
tiene diferentes carencias de programacion.

Para mayor informacion poner se en contacto a travez de la pagina de descarga.
--------------------------------------------------------------------------------------------
*/

#include "lgOgre.h"
#include "lgSDL.h"
#include "lgIMGUI.h"
#include "lgBullet.h"

#include "oggsound/include/OgreOggSound.h"
#include "oggsound/include/OgreOggISound.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/configuration.h>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/input.h>
#endif

class LibreGame : public lgOgre, public lgSDL, public lgGUI, public lgBullet
{

public:
    LibreGame();
    void initApp();
    void initRender();
    void captEvento();
    void moverCamara(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom);  
    Ogre::Real deltaT;
    Ogre::SceneNode *lgPlayer;
    Ogre::Entity *ePlayer;
    Ogre::AnimationState *lgAnimacion;
    Ogre::Vector3 mKeyDirection;
    Ogre::Vector3 mGoalDirection;
    Ogre::SceneNode *mCameraPivot;
    Ogre::SceneNode *mCameraGoal;
    Ogre::Real mPivotPitch;
    bool p56 = false;
    int dx2 = 0;
    int d = 0;
    int dy = 0;
    lgEntidad2 *ent2;
    btRigidBody *lMundo(const Ogre::String &nombre, const Ogre::String &mesh, int x, int y, int z, double tamano, int escalar);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    virtual void initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window){};
#endif
private:
    virtual bool frameStarted(const Ogre::FrameEvent &evt);
    virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt);
    virtual bool frameRenderingQueued(const Ogre::FrameEvent &evt);
    virtual bool mouseMoved(const OgreBites::MouseMotionEvent &evt);
    virtual bool mousePressed(const OgreBites::MouseButtonEvent &evt);
    virtual bool mouseReleased(const OgreBites::MouseButtonEvent &evt);
    virtual bool touchMoved(const OgreBites::TouchFingerEvent &evt);
    virtual bool touchPressed(const OgreBites::TouchFingerEvent &evt);
    virtual bool touchReleased(const OgreBites::TouchFingerEvent &evt);
    OgreOggSound::OgreOggSoundManager *mSoundManager;
    OgreOggSound::OgreOggISound *sound;

};