#include "LibreGame.h"
#include "OgreRTShaderSystem.h"

LibreGame::LibreGame() : lgSDL(), lgBullet(lgManager)
{
    lgRoot = NULL;
    lgManager = NULL;
    lgTmanager = NULL;
    lgRayManager = NULL;
    CamPlayer = NULL;
    renderTexture = NULL;
#if EXPORTAR == 0
    interfaz_gui = true;
    play = false;
    rotar_camara = false;
#else
    interfaz_gui = false;
    play = true;
    rotar_camara = false;
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    lgManagerAndroid = NULL;
    lgConfigAndroid = NULL;
#endif
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    mMaterialMgrListener = NULL;
    mShaderGenerator = NULL;
#endif
}
void LibreGame::initApp()
{
    crearRoot();
    if (!configuracion()) return;   
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    // if the context was reconfigured, set requested renderer
    //lgRoot->setRenderSystem(lgRoot->getRenderSystemByName(mNextRenderer));
#endif
    lgRoot->initialise(false, "LibreGame");
    
    crearVentana("LibreGameSDL", lgRoot);
    lgVentana = getRenderWindow();

    addInputListener(this);
    lgSDL::windowResized(lgVentana);
    lgVentana->removeAllViewports();

    lgManager = lgRoot->createSceneManager();
    lgManager->setShadowTechnique(Ogre::ShadowTechnique::SHADOWTYPE_STENCIL_MODULATIVE);
    lgManager->addRenderQueueListener(getOverlaySystem());
    lgManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));

    lgMundo = lgMundoDinamico(lgManager);

    lgCam = lgManager->createCamera("lgCamara");
    lgCam->setNearClipDistance(0.5);
    lgCam->setFarClipDistance(5000);
    nodeCam = lgManager->getRootSceneNode()->createChildSceneNode();
    nodeCam->attachObject(lgCam);
    nodeCam->setPosition(0, 0, 0);

    CamPlayer = lgManager->createCamera("lgCamaraPlayer");
    CamPlayer->setNearClipDistance(0.5);
    CamPlayer->setFarClipDistance(5000);
    nodeCamPlayer = lgManager->getRootSceneNode()->createChildSceneNode();
    nodeCamPlayer->attachObject(CamPlayer);

    pos_camara = lgManager->getRootSceneNode()->createChildSceneNode();
    //nodeCam->attachObject(lgCam);
    pos_camara->setPosition(0, 0, 0);

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
#if EXPORTAR == 0
    lgVista = lgVentana->addViewport(lgCam);
#else
    lgVista = lgVentana->addViewport(CamPlayer);
#endif
#ifdef OGRE_BUILD_COMPONENT_RTSHADERSYSTEM
    lgVista->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);
#endif
    lgVista->setBackgroundColour(Ogre::ColourValue(0.2, 0.2, 0.2));
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID || EXPORTAR
    //regilla();
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKFileSystemArchiveFactory(lgManagerAndroid));
    Ogre::ArchiveManager::getSingleton().addArchiveFactory(new Ogre::APKZipArchiveFactory(lgManagerAndroid));
#endif
    cargarRecursos();
    inicializarRTShaderSystem();
    mShaderGenerator->addSceneManager(lgManager);
    inicializarRecursos();

    lgVentana->addListener(this);
    lgRoot->addFrameListener(this);
    

    lgTmanager.reset(new OgreBites::TrayManager("SampleControls", lgVentana, this));
    //lgTmanager->showLogo(OgreBites::TL_TOPRIGHT);

    auto imguiOverlay = new Ogre::ImGuiOverlay();
    imguiOverlay->setZOrder(300);
    imguiOverlay->show();
    Ogre::OverlayManager::getSingleton().addOverlay(imguiOverlay);

    sdlImguiListener.reset(new OgreBites::ImGuiInputListener());
    sdlListenerChain = OgreBites::InputListenerChain({lgTmanager.get(), sdlImguiListener.get()});

    lgTmanager->hideCursor();

    ImGuiIO &io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    io.MousePos = ImVec2(0, 0);

    

    lgRayManager = lgManager->createRayQuery(Ogre::Ray());
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    cargarXML();
#endif
    if (nueva_escena_xml == false)
        cargarEscena();

    //btRigidBody *piso = lMundo("piso", "Wall.mesh", 0, -0.5f, 0, 250,1);
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    rttTexture = Ogre::TextureManager::getSingleton().createManual("RttTex",Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, Ogre::TEX_TYPE_2D, lgVentana-> getWidth(), lgVentana->getHeight(),0,Ogre::PF_R8G8B8,Ogre::TU_RENDERTARGET);
    renderTexture = rttTexture->getBuffer()->getRenderTarget();
 
    renderTexture->addViewport(CamPlayer);
    renderTexture->getViewport(0)->setClearEveryFrame(true);
    renderTexture->getViewport(0)->setBackgroundColour(Ogre::ColourValue::Black);
    renderTexture->getViewport(0)->setOverlaysEnabled(false);

    renderTexture->update(); 
    renderTexture->writeContentsToFile("Play.png");
    renderTexture->setFSAA(4,"FSAA");
    renderTexture->addListener(this);
#endif
    lgAudio audio;
    audio.cargarAudio("/home/rgr/LibreGame/Media/fondo.wav");
    audio.playAudio(0);

    lgEntidadv[0].ent->entidad->setCastShadows(false); 
    lgEntidadv[1].ent->entidad->setMaterialName("mate_piso"); 
    lgEntidadv[1].ent->entidad->setCastShadows(true);  
    lgEntidadv[1].ent->entidad->setMaterialName("Material.Player");
    
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
    entity = lgManager->createEntity(nombre, mesh);
    entity->setCastShadows(false);
    Ogre::SceneNode *node;
    node = lgManager->getRootSceneNode()->createChildSceneNode();
    node->scale(Ogre::Vector3(escalar, 0, escalar));
    node->attachObject(entity);
    btRigidBody *body;
    body = plano(node, x, y, z, tamano);
    m_dynamicsWorld->addRigidBody(body);
    return body;
}
void LibreGame::camPlayer()
{
    lgManager->destroyCamera("lgCamaraPlayer");
    lgVentana->removeAllViewports();

    lgManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    
    CamPlayer = lgManager->createCamera("lgCamaraPlayer");
    CamPlayer->setNearClipDistance(0.5);
    CamPlayer->setFarClipDistance(5000);
    nodeCamPlayer = lgManager->getRootSceneNode()->createChildSceneNode();
    nodeCamPlayer->attachObject(CamPlayer);
    //nodeCamPlayer->lookAt(lgEntidadv[3].ent->nodo->_getDerivedPosition(), Ogre::Node::TS_PARENT);
    //nodeCamPlayer->pitch(Ogre::Degree(-15));   

    lgVentana->removeAllViewports();
    lgVista = lgVentana->addViewport(CamPlayer);
    lgVista->setBackgroundColour(Ogre::ColourValue(0.8, 0.8, 0.8));
}
void LibreGame::camRoot()
{
    lgManager->destroyCamera("lgCamaraPlayer");
    lgVentana->removeAllViewports();

    lgManager->setAmbientLight(Ogre::ColourValue(0.5, 0.5, 0.5));
    lgVista = lgVentana->addViewport(lgCam);
    lgVista->setBackgroundColour(Ogre::ColourValue(0.2, 0.2, 0.2));
}
void LibreGame::preRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
    //ImGui::End();

    //sdlImguiListener.reset(new OgreBites::ImGuiInputListener());
    //sdlListenerChain = OgreBites::InputListenerChain({lgTmanager.get(), sdlImguiListener.get()});
  //miPantalla->setVisible(false);
}

void LibreGame::postRenderTargetUpdate(const Ogre::RenderTargetEvent& rte)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
  ImGui::Render();
#endif
  //miPantalla->setVisible(true);
}