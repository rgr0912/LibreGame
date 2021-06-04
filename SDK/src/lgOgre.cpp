#include "lgOgre.h"

lgOgre::lgOgre()
{
    sistemaArchivos = new Ogre::FileSystemLayer("OGRE");

    if (char *val = getenv("OGRE_CONFIG_DIR"))
    {
        Ogre::String configDir = Ogre::StringUtil::standardisePath(val);
        sistemaArchivos->setConfigPaths({configDir});
    }

    lgRoot = NULL;
    lgOverlaySystem = NULL;
    mShaderGenerator = NULL;
    lgTmanager = NULL;
}
//inicializacion de Ogre 3d
void lgOgre::crearRoot()
{
    //creacion de root para android
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    lgRoot = OGRE_NEW Ogre::Root("");
#else
    //ruta de los plugins
    Ogre::String rutaPlugins;
#ifndef OGRE_STATIC_LIB
    rutaPlugins = sistemaArchivos->getConfigFilePath("plugins.cfg");

    if (!Ogre::FileSystemLayer::fileExists(rutaPlugins))
    {
        rutaPlugins = Ogre::FileSystemLayer::resolveBundlePath(OGRE_CONFIG_DIR "/plugins.cfg");
    }
#endif
    //creacion de root para linux
    lgRoot = OGRE_NEW Ogre::Root(rutaPlugins, sistemaArchivos->getWritablePath("ogre.cfg"), sistemaArchivos->getWritablePath("ogre.log"));
#endif
    //carga de plugins estaticos
#ifdef OGRE_STATIC_LIB
    mplugins = OGRE_NEW OgreOggSound::OgreOggSoundPlugin();
    lgRoot->installPlugin(mplugins);
    lgStaticPluginLoader.load();
#endif
    lgOverlaySystem = new Ogre::OverlaySystem();
}
bool lgOgre::configuracion()
{
    //primera configuracion de ogre en dialogo
    if (!lgRoot->restoreConfig())
    {
        lgRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
    }

    return true;
}