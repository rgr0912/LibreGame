#include "LibreGame.h"

LibreGame::LibreGame() : lgSDL(), lgBullet(sm)
{
    lgRoot = NULL;
    sm = NULL;
    lgTmanager = NULL;
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
    lgVentana = getRenderWindow();

    addInputListener(this);
    lgSDL::windowResized(lgVentana);
    lgVentana->removeAllViewports();

    sm = lgRoot->createSceneManager(Ogre::ST_GENERIC);
    sm->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_TEXTURE_ADDITIVE);
    sm->addRenderQueueListener(getOverlaySystem());
    sm->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    lgMundo = lgMundoDinamico(sm);

    lgCam = sm->createCamera("lgCamara");
    lgCam->setNearClipDistance(0.5);
    lgCam->setFarClipDistance(500);
    nodeCam = sm->getRootSceneNode()->createChildSceneNode();
    nodeCam->attachObject(lgCam);
    nodeCam->setPosition(0, 0, 0);

    mCameraPivot = lgCam->getSceneManager()->getRootSceneNode()->createChildSceneNode();
    // this is where the camera should be soon, and it spins around the pivot
    mCameraGoal = mCameraPivot->createChildSceneNode(Ogre::Vector3(0, 10, 20));
    // this is where the camera actually is
    nodeCam = lgCam->getParentSceneNode();
    nodeCam->setPosition(mCameraPivot->getPosition() + mCameraGoal->getPosition());

    mCameraPivot->setFixedYawAxis(true);
    mCameraGoal->setFixedYawAxis(true);
    nodeCam->setFixedYawAxis(true);

    mPivotPitch = 0;

    lgVista = lgVentana->addViewport(lgCam);
    lgVista->setBackgroundColour(Ogre::ColourValue(0.1, 0.2, 0.3));

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(lgManagerAndroid));
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(lgManagerAndroid));
#endif
    cargarRecursos();
    inicializarRTShaderSystem();
    mShaderGenerator->addSceneManager(sm);
    inicializarRecursos();

    lgVentana->addListener(this);
    lgRoot->addFrameListener(this);

    lgTmanager.reset(new OgreBites::TrayManager("SampleControls", lgVentana, this));
    //lgTmanager->showLogo(OgreBites::TL_TOPRIGHT);
    //lgTmanager->showFrameStats(OgreBites::TL_TOPLEFT);

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);

    mImguiListener.reset(new OgreBites::ImGuiInputListener());
    mListenerChain = OgreBites::InputListenerChain({lgTmanager.get(), mImguiListener.get()});

    lgTmanager->hideCursor();

    ImGuiIO &io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    io.MousePos = ImVec2(0, 0);

    ent2 = new lgEntidad2("df", "Sinbad.mesh");
    ent2->setPosicion(0, 96, 0);
    ent2->mostrar(sm, m_dynamicsWorld, 1);
    ent2->setAnimacion("IdleTop");

    Ogre::Light *l = sm->createLight("l1");
    l->setDiffuseColour(Ogre::ColourValue(0.0, 1, 0.5));
    l->setSpecularColour(Ogre::ColourValue(1, 0.5, 0.5));
    Ogre::SceneNode *nodeL = sm->getRootSceneNode()->createChildSceneNode();
    nodeL->attachObject(l);
    nodeL->setPosition(0, 50, 0);

    btRigidBody *piso = lMundo("piso", "Plane.mesh", 0, -0.5f, 0, 250, 80);

    mSoundManager = OgreOggSound::OgreOggSoundManager::getSingletonPtr();
    if(mSoundManager->init()){
        sound = 0;
        sound = mSoundManager->createSound("MiPista","fondo.ogg",true,true);
        if(sound){
            sound->play();
        }

    }

}
void LibreGame::initRender()
{
    captEvento();
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    lgRoot->startRendering();
    lgRoot->renderOneFrame();
    SDL_GL_SwapWindow(ventana_lista[0].ventana_nativa);
#endif
}
btRigidBody *LibreGame::lMundo(const Ogre::String &nombre, const Ogre::String &mesh, int x, int y, int z, double tamano, int escalar)
{
    Ogre::Entity *entity;
    entity = sm->createEntity(nombre, mesh);
    entity->setCastShadows(false);
    Ogre::SceneNode *node;
    node = sm->getRootSceneNode()->createChildSceneNode();
    node->scale(Ogre::Vector3(escalar, 0, escalar));
    node->attachObject(entity);
    btRigidBody *body;
    body = plano(node, x, y, z, tamano);
    m_dynamicsWorld->addRigidBody(body);
    return body;
}
