#include "LibreGame.h"

LibreGame::LibreGame() : lgSDL()
{
    lgRoot = NULL;
    #if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    lgManagerAndroid = NULL;
    lgConfigAndroid = NULL;
    #endif
}
void LibreGame::initApp()
{
    crearRoot();
    if (!configuracion())
        return;
    lgRoot->initialise(false, "LibreGameEngine");
    crearVentana("LibreGameSDL", lgRoot);
    ventana_lista[0].ventana_ogre->removeAllViewports();
    Ogre::SceneManager *sm = lgRoot->createSceneManager("DefaultSceneManager", "DummyScene");
    sm->setAmbientLight(Ogre::ColourValue(0.9, 0.9, 0.9));
    Ogre::Camera *cam = sm->createCamera("DummyCamera");
    cam->setFarClipDistance(1500);
    cam->setNearClipDistance(0.5);
    Ogre::SceneNode *nodeCam = sm->getRootSceneNode()->createChildSceneNode();
    nodeCam->attachObject(cam);
    nodeCam->setPosition(0, 0, 227);
    lgVista = ventana_lista[0].ventana_ogre->addViewport(cam);
    lgVista->setBackgroundColour(Ogre::ColourValue(1, 0.2, 0.3));
    cargarRecursos();
    lgRoot->startRendering();
}