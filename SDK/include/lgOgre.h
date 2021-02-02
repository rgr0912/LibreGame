#include "Ogre.h"
#include "OgreFileSystemLayer.h"
#include "OgreConfigPaths.h"
#include "OgreOverlaySystem.h"
#include "OgreStaticPluginLoader.h"
#include "OgreBitesConfigDialog.h"

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
    virtual void cargarRecursos();
protected:
    Ogre::Root *lgRoot;
    Ogre::FileSystemLayer *sistemaArchivos;
    Ogre::OverlaySystem *lgOverlaySystem;
    OgreBites::StaticPluginLoader lgStaticPluginLoader;
    Ogre::Viewport *lgVista;
    static Ogre::String getDefaultMediaDir();
};