#include "LibreGame.h"

bool reset = false;
int i = 0;
int dir;
Ogre::Real yawTo;

bool *p_open;

void LibreGame::captEvento()
{
    Ogre::LogManager::getSingletonPtr()->logMessage("*** Inicializado LibreGameSDK ***");
    Ogre::WindowEventUtilities::addWindowEventListener(lgVentana, this);
}

void LibreGame::preViewportUpdate(const Ogre::RenderTargetViewportEvent &evt)
{
   Ogre::ImGuiOverlay::NewFrame();
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    play = true;
#endif
    if (play == true)
    {
        CrearControlGui(p_open, lgVentana);
        lgInterfazGame(p_open, lgVentana, getColision);
    }
    if (interfaz_gui == true)
    {
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
        MenuBar(p_open, lgManager);
        MenuHerramientas(p_open, lgVentana, lgVista, lgManager, lgCam);
        Explorador(p_open, lgVentana, lgManager);
#endif
    }
    //ImGui::ShowDemoWindow();
    if (nueva_escena_xml == true)
    {
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
        nuevaEscenaXml(p_open);
#endif
    }
}
int pa = 0;
bool LibreGame::frameRenderingQueued(const Ogre::FrameEvent &evt)
{
    for (InputListenerList::iterator it = mInputListeners.begin();
         it != mInputListeners.end(); ++it)
    {
        it->second->frameRendered(evt);
    }
    lgTmanager->frameRendered(evt);
    deltaT = (0.016 + 0.017) / 2;
    
    if (play)
    {
        sdlEventos(lgRoot, ventana_lista[0].ventana_ogre, CamPlayer, lgRayManager);
    }
    else
    {
        sdlEventos(lgRoot, ventana_lista[0].ventana_ogre, lgCam, lgRayManager);
    }

    if (boton1.getValor() > 0)
    {
        //d = boton1.getValor();
        //yawTo = 1;

        rotar(lgEntidadv[1].ent->getBody(), 0.05f);
    }
    else if (boton2.getValor() > 0)
    {
        //d = boton2.getValor() * -1;

        //caminar(ent2->getBody(),d);
        saltar(lgEntidadv[1].ent->getBody(), 0);
    }
    else if (boton3.getValor() > 0)
    {
        //d = boton3.getValor() * -1;

        rotar(lgEntidadv[1].ent->getBody(), -0.05f);
    }
    else if (boton4.getValor() > 0)
    {
        //d = boton4.getValor();

        caminar(lgEntidadv[1].ent->getBody(), boton4.getValor());
        //ent2->setAnimacion("RunBase");
        /*if (pa == 0)
        {
            pa = 1;
        }*/
        //mgaudio->stopAudio(audioId);
    }
    else
    {
        //d = 0;
        //dy = 0;
        //dir = 0;

        //pa = 0;

        //ent2->setAnimacionAlto("RunBase", "IdleTop");
    }

    pos_camara->translate(d * deltaT, dy * deltaT, dz * deltaT, Ogre::Node::TS_LOCAL);
    mCameraPivot->setPosition(pos_camara->getPosition() + Ogre::Vector3::UNIT_Y * 2);
    Ogre::Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - nodeCam->getPosition();
    nodeCam->translate(goalOffset * deltaT * 9.0f);
    nodeCam->lookAt(mCameraPivot->_getDerivedPosition(), Ogre::Node::TS_PARENT);

    if (play == true)
     nodeCamPlayer->setPosition(lgEntidadv[1].ent->nodo->getPosition().x, lgEntidadv[1].ent->nodo->getPosition().y + 2, lgEntidadv[1].ent->nodo->getPosition().z + 20);

    //lgEntidadv[0].ent->setTiempo(deltaT);

    if (play == true)
    {
        dbdraw->setDebugMode(false);
        updateBody(total_body);
        lgMundo->stepSimulation(deltaT, 5);
       // sound->play();
    }
    else
    {
       // mSoundManager->pauseAllSounds();
    }
    if (interfaz_gui == true)
    {
        dbdraw->setDebugMode(true);
    }
    m_dynamicsWorld->debugDrawWorld();
    dbdraw->step();
    //mSoundManager->update(evt.timeSinceLastFrame);

    return true;
}
void LibreGame::moverCamara(Ogre::Real deltaYaw, Ogre::Real deltaPitch, Ogre::Real deltaZoom)
{
    mCameraPivot->yaw(Ogre::Degree(deltaYaw), Ogre::Node::TS_PARENT);

    // bound the pitch
    if (!(mPivotPitch + deltaPitch > 25 && deltaPitch > 0) &&
        !(mPivotPitch + deltaPitch < -60 && deltaPitch < 0))
    {
        mCameraPivot->pitch(Ogre::Degree(deltaPitch), Ogre::Node::TS_LOCAL);
        mPivotPitch += deltaPitch;
    }

    Ogre::Real dist = mCameraGoal->_getDerivedPosition().distance(mCameraPivot->_getDerivedPosition());
    Ogre::Real distChange = deltaZoom * dist;

    // bound the zoom
    if (!(dist + distChange < 8 && distChange < 0) &&
        !(dist + distChange > 25 && distChange > 0))
    {
        mCameraGoal->translate(0, 0, distChange, Ogre::Node::TS_LOCAL);
    }
}
bool LibreGame::frameStarted(const Ogre::FrameEvent &evt)
{
//oyente de ogre
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    //lgMundo->stepSimulation(0.1,5);
    ImGui_ImplSDL2_NewFrame(ventana_lista[0].ventana_nativa);
#endif
    return true;
}