#include "lgOgre.h"

#define LIBREGAME_CONFIG_DIR "/home/rgr/LibreGame/Media/"
#define LIBREGAME_MEDIA_DIR "/home/rgr/LibreGame/Media/"
#define LIBREGAME_PLUGIN_DIR "/home/rgr/LibreGame/Media/" 

lgOgre::lgOgre()
{
    sistemaArchivos = new Ogre::FileSystemLayer("OGRE");

    if (char *val = getenv("LIBREGAME_CONFIG_DIR"))
    {
        Ogre::String configDir = Ogre::StringUtil::standardisePath(val);
        sistemaArchivos->setConfigPaths({configDir});
    }

    lgRoot = NULL;
    lgOverlaySystem = NULL;
    mShaderGenerator = NULL;
    lgTmanager = NULL;
}
void lgOgre::crearRoot()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    lgRoot = OGRE_NEW Ogre::Root("");
#else
    Ogre::String rutaPlugins;
#ifndef OGRE_STATIC_LIB
    rutaPlugins = sistemaArchivos->getConfigFilePath("plugins.cfg");

    if (!Ogre::FileSystemLayer::fileExists(rutaPlugins))
    {
        rutaPlugins = Ogre::FileSystemLayer::resolveBundlePath(LIBREGAME_CONFIG_DIR "/plugins.cfg");
    }
#endif
    lgRoot = OGRE_NEW Ogre::Root(rutaPlugins, sistemaArchivos->getWritablePath("ogre.cfg"), sistemaArchivos->getWritablePath("ogre.log"));
#endif
#ifdef OGRE_STATIC_LIB
    mplugins = OGRE_NEW OgreOggSound::OgreOggSoundPlugin();
    lgRoot->installPlugin(mplugins);
    lgStaticPluginLoader.load();
#endif
    lgOverlaySystem = new Ogre::OverlaySystem();
}
bool lgOgre::configuracion()
{
    if (!lgRoot->restoreConfig())
    {
        lgRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
    }

    return true;
}