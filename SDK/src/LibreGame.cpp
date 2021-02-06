#include "LibreGame.h"

LibreGame::LibreGame() : lgSDL()
{
    lgRoot = NULL;
    std::unique_ptr<OgreBites::ImGuiInputListener> mImguiListener;
    OgreBites::InputListenerChain mListenerChain;
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
    lgCam = sm->createCamera("DummyCamera");
    lgCam->setNearClipDistance(0.5);
    lgCam->setFarClipDistance(500);
    Ogre::SceneNode *nodeCam = sm->getRootSceneNode()->createChildSceneNode();
    nodeCam->attachObject(lgCam);
    nodeCam->setPosition(0, 0, 0);
    lgVista = ventana_lista[0].ventana_ogre->addViewport(lgCam);
    lgVista->setBackgroundColour(Ogre::ColourValue(0.1, 0.2, 0.3));
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(lgManagerAndroid));
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(lgManagerAndroid));
#endif
    cargarRecursos();
    inicializarRTShaderSystem();
    mShaderGenerator->addSceneManager(sm);
    inicializarRecursos();

    sm->addRenderQueueListener(lgOverlaySystem);
    lgTmanager.reset(new OgreBites::TrayManager("SampleControls", ventana_lista[0].ventana_ogre));
    lgTmanager->showLogo(OgreBites::TL_BOTTOMLEFT);
    lgTmanager->showCursor();

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);
    
    

    Ogre::Entity *sinbad = sm->createEntity("Sinbad", "Sinbad.mesh");
    Ogre::SceneNode *nodeS = sm->getRootSceneNode()->createChildSceneNode();
    sm->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    nodeS->attachObject(sinbad);
    nodeS->setPosition(0, 0, -20);
    Ogre::Light *l = sm->createLight("l1");
    Ogre::SceneNode *nodeL = sm->getRootSceneNode()->createChildSceneNode();
    nodeL->attachObject(l);
    nodeL->setPosition(0, 0, 20);
    capturarEvento();
    lgRoot->startRendering();
}
void LibreGame::capturarEvento()
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Inicializado LibreGameSDK ***");

    //
    Ogre::WindowEventUtilities::addWindowEventListener(ventana_lista[0].ventana_ogre, this);
    lgRoot->addFrameListener(this);
    ventana_lista[0].ventana_ogre->addListener(this);
}
void LibreGame::preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt)
{
    if(!evt.source->getOverlaysEnabled()) return;
        if(!lgTmanager->getTraysLayer()->isVisible()) return;

        Ogre::ImGuiOverlay::NewFrame();

        ImGui::ShowDemoWindow();
}
bool LibreGame::frameStarted(const Ogre::FrameEvent &evt)
{
    //lgCam->setAspectRatio(Ogre::Real(getRenderWindow()->getWidth()/getRenderWindow()->getHeight()));
    sdlEventos(lgRoot, ventana_lista[0].ventana_ogre, lgCam);
    ImGui_ImplSDL2_NewFrame(ventana_lista[0].ventana_nativa);
    //oyente de ogre

    return true;
}