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
    nodeCam->setPosition(0, 0, 0);
    lgVista = ventana_lista[0].ventana_ogre->addViewport(cam);
    lgVista->setBackgroundColour(Ogre::ColourValue(0.1, 0.2, 0.3));
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(lgManagerAndroid));
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(lgManagerAndroid));
#endif
    cargarRecursos();
    inicializarRTShaderSystem();
    mShaderGenerator->addSceneManager(sm);
    inicializarRecursos();
    Ogre::Entity *sinbad = sm->createEntity("Sinbad", "Sinbad.mesh");
    Ogre::SceneNode *nodeS = sm->getRootSceneNode()->createChildSceneNode();
    sm->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    nodeS->attachObject(sinbad);
    nodeS->setPosition(0, 0, -20);
    Ogre::Light *l = sm->createLight("l1");
    Ogre::SceneNode *nodeL = sm->getRootSceneNode()->createChildSceneNode();
    nodeL->attachObject(l);
    nodeL->setPosition(0, 0, 20);
    lgRoot->startRendering();
}