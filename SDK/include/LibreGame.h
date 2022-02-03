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
#include "lgSDLmixer.h"
#include "lgIMGUI.h"
#include "lgBullet.h"
#include "lgEntidad.h"
#include "lgLuces.h"

#include "config.h"

//#include "oggsound/include/OgreOggSound.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/configuration.h>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/input.h>
#endif

struct entidad{
    lgEntidad2* ent;
};
struct luz{
    lgLuces* l;
};

class LibreGame : public lgOgre, public lgSDL, public lgGUI, public lgBullet 
{

public:
    LibreGame();
    void initApp();
    void initRender();
    void captEvento();
    void moverCamara(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom);
    virtual void preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
    virtual void postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte);
    std::vector<entidad> lgEntidadv;
    std::vector<luz> lgLucesv;  
    Ogre::Real deltaT;
    Ogre::SceneNode *lgPlayer;
    Ogre::Entity *ePlayer;
    Ogre::Vector3 mKeyDirection;
    Ogre::Vector3 mGoalDirection;
    Ogre::SceneNode *mCameraPivot;
    Ogre::SceneNode *mCameraGoal;
    Ogre::Real mPivotPitch;
    Ogre::RenderTexture* renderTexture;
    Ogre::MaterialPtr renderMaterial;
    Ogre::TexturePtr rttTexture;
    Ogre::Rectangle2D* miPantalla;
    bool p56 = false;
    int dx2 = 0;
    int d = 0;
    int dy = 0;
    int dz = 0;
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
    virtual bool keyPressed(const OgreBites::KeyboardEvent& evt);
    virtual bool keyReleased(const OgreBites::KeyboardEvent& evt);
    //OgreOggSound::OgreOggSoundManager *mSoundManager;
    //OgreOggSound::OgreOggISound *sound;
    bool m_cam_interfaz;
    bool play;
    bool rotar_camara;
    void cargarEscena();
    void regilla();
    Ogre::SceneNode *pos_camara;
    int total_body = 1;
    Ogre::SceneNode *nodeCamPlayer;
    Ogre::Camera *CamPlayer;
    void camPlayer();
    void camRoot();
};