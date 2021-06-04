#include "Ogre.h"
#include "OgreFileSystemLayer.h"
#include "OgreConfigPaths.h"
#include "OgreOverlaySystem.h"
#include "OgreStaticPluginLoader.h"
#include "OgreBitesConfigDialog.h"
#include "OgrePlugin.h"
#include "OgreRoot.h"
#include "OgreArchiveManager.h"
#include "OgreFileSystem.h"
#include "OgreZip.h"
#include "OgreRenderTargetListener.h"
#include "OgreWindowEventUtilities.h"
#include "OgreTrays.h"
#include "OgreImGuiOverlay.h"
#include "OgreApplicationContextBase.h"
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
#include "OgreSGTechniqueResolverListener.h"
#endif // INCLUDE_RTSHADER_SYSTEM

#include "oggsound/include/OgreOggSoundPlugin.h"

namespace Ogre
{
    class OverlaySystem;
}

class lgOgre : public Ogre::FrameListener, public Ogre::RenderTargetListener, public Ogre::WindowEventListener
{
public:
    lgOgre();
    //funciones publicas

    //objetos publicos
    Ogre::Root *getRoot() const
    {
        return lgRoot;
    }
    Ogre::OverlaySystem *getOverlaySystem() const
    {
        return lgOverlaySystem;
    }
    Ogre::Camera *getCam() const
    {
        return lgCam;
    }
    Ogre::FileSystemLayer &getSistemaArchivos() { return *sistemaArchivos; }
    Ogre::OverlaySystem *lgOverlaySystem;
    Ogre::Camera *lgCam;
    Ogre::SceneNode *nodeCam;
    //OgreBites::TrayManager *lgTmanager;

    //funciones protegidas
    virtual void crearRoot();
    virtual bool configuracion();
    virtual void reconfigurar(const Ogre::String &renderer, Ogre::NameValuePairList &options);
    virtual void cargarRecursos();
    virtual bool inicializarRTShaderSystem();
    virtual void inicializarRecursos();
    //virtual mouseMoved(const OgreBites::MouseMotionEvent& evt);
    //objectos protegios
    Ogre::Root *lgRoot;
    Ogre::SceneManager *sm;
    Ogre::RenderWindow* lgVentana;
    Ogre::FileSystemLayer *sistemaArchivos;
    //OgreBites::TrayManager *lgTmanager;
    std::unique_ptr<OgreBites::TrayManager> lgTmanager;
    OgreBites::StaticPluginLoader lgStaticPluginLoader;
    Ogre::Viewport *lgVista;
    static Ogre::String getDefaultMediaDir();
    Ogre::String mNextRenderer;
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    Ogre::RTShader::ShaderGenerator *mShaderGenerator;            // The Shader generator instance.
    OgreBites::SGTechniqueResolverListener *mMaterialMgrListener; // Shader generator material manager listener.
#endif                                                            // INCLUDE_RTSHADER_SYSTEM
    OgreOggSound::OgreOggSoundPlugin* mplugins;
};