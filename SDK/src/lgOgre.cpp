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
        rutaPlugins = Ogre::FileSystemLayer::resolveBundlePath(OGRE_CONFIG_DIR "/plugins.cfg");
    }
#endif

    lgRoot = OGRE_NEW Ogre::Root(rutaPlugins, sistemaArchivos->getWritablePath("ogre.cfg"), sistemaArchivos->getWritablePath("ogre.log"));
#endif

#ifdef OGRE_STATIC_LIB
    lgStaticPluginLoader.load();
#endif
    lgOverlaySystem = OGRE_NEW Ogre::OverlaySystem();
}
 bool lgOgre::configuracion()
{
    if (!lgRoot->restoreConfig())
    {
        lgRoot->showConfigDialog(OgreBites::getNativeConfigDialog());
    }

    return true;
}