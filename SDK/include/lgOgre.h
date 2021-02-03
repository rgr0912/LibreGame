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
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
#include "OgreSGTechniqueResolverListener.h"
#endif // INCLUDE_RTSHADER_SYSTEM

namespace Ogre
{
    class OverlaySystem;
}
class lgOgre
{
public:
    lgOgre();
    virtual void crearRoot();
    Ogre::Root *getRoot() const
    {
        return lgRoot;
    }
    Ogre::FileSystemLayer &getSistemaArchivos() { return *sistemaArchivos; }
    virtual bool configuracion();
    virtual void reconfigurar(const Ogre::String &renderer, Ogre::NameValuePairList &options);
    virtual void cargarRecursos();
    virtual bool inicializarRTShaderSystem();
    virtual void inicializarRecursos();

protected:
    Ogre::Root *lgRoot;
    Ogre::FileSystemLayer *sistemaArchivos;
    Ogre::OverlaySystem *lgOverlaySystem;
    OgreBites::StaticPluginLoader lgStaticPluginLoader;
    Ogre::Viewport *lgVista;
    static Ogre::String getDefaultMediaDir();
    Ogre::String mNextRenderer;
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    Ogre::RTShader::ShaderGenerator *mShaderGenerator; // The Shader generator instance.
    OgreBites::SGTechniqueResolverListener *mMaterialMgrListener; // Shader generator material manager listener.
#endif                                                 // INCLUDE_RTSHADER_SYSTEM
};