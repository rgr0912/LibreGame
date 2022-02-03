/*
LibreGame es un proyecto que intenta crear un game engine a partir de librerias libres.
*/
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
#include "OgreVector3.h"
#include "OgreRenderTargetListener.h"

#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
#include "OgreSGTechniqueResolverListener.h"
#endif // INCLUDE_RTSHADER_SYSTEM

//#include "oggsound/include/OgreOggSoundPlugin.h"

namespace Ogre
{
    class OverlaySystem;
}

///Esta clase se encargada del render utilizano la libreria de Ogre3d,
///La mayoria de las funciones son del Applicationcontext de ogre,
///para una mayor explicacion revisar la documentacion de Ogre.
class lgOgre : public Ogre::FrameListener, public Ogre::RenderTargetListener, public Ogre::WindowEventListener
{
public:
    lgOgre();
    Ogre::Root *lgRoot;
    Ogre::SceneManager *lgManager;
    Ogre::RenderWindow *lgVentana;
    Ogre::RaySceneQuery *lgRayManager;
    std::unique_ptr<OgreBites::TrayManager> lgTmanager;

//protected:
    void crearRoot();
    bool configuracion();
    void reconfigurar(const Ogre::String &renderer, Ogre::NameValuePairList &options);
    void cargarRecursos();
    bool inicializarRTShaderSystem();
    void setRTSSWriteShadersToDisk(bool write);
    void enableShaderCache() const;
    void inicializarRecursos();
    Ogre::Camera *lgCam;
    Ogre::SceneNode *nodeCam;
    OgreBites::StaticPluginLoader lgStaticPluginLoader;
    Ogre::Viewport *lgVista;
    static Ogre::String getDefaultMediaDir();
    Ogre::String mNextRenderer;
    Ogre::RaySceneQueryResult::iterator lgItr;
    Ogre::FileSystemLayer *sistemaArchivos;
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    Ogre::RTShader::ShaderGenerator *mShaderGenerator;
    OgreBites::SGTechniqueResolverListener *mMaterialMgrListener;
#endif
    //OgreOggSound::OgreOggSoundPlugin *mplugins;
    Ogre::FileSystemLayer &getSistemaArchivos() { return *sistemaArchivos; }
    Ogre::OverlaySystem *lgOverlaySystem;
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
};