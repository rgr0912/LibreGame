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
    //ImGui::ShowDemoWindow();
    CrearControlGui(p_open, lgVentana);
    lgInterfazGame(p_open, lgVentana, getColision);
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
    sdlEventos(lgRoot, ventana_lista[0].ventana_ogre, lgCam);

    if (boton1.getValor() > 0)
    {
        d = boton1.getValor();
        yawTo = 1;

        rotar(ent2->getBody(), 0.05f);
    }
    else if (boton2.getValor() > 0)
    {
        d = boton2.getValor() * -1;

        //caminar(ent2->getBody(),d);
        saltar(ent2->getBody(), 0);
    }
    else if (boton3.getValor() > 0)
    {
        d = boton3.getValor() * -1;

        rotar(ent2->getBody(), -0.05f);
    }
    else if (boton4.getValor() > 0)
    {
        d = boton4.getValor();

        caminar(ent2->getBody(), d);
        ent2->setAnimacion("RunBase");
        if (pa == 0)
        {
            pa = 1;
        }
        //mgaudio->stopAudio(audioId);
    }
    else
    {
        d = 0;
        dy = 0;
        dir = 0;

        pa = 0;

        ent2->setAnimacionAlto("RunBase", "IdleTop");
    }

    //lgPlayer->translate(d * deltaT,0,dy*deltaT, Ogre::Node::TS_LOCAL);
    mCameraPivot->setPosition(ent2->getEntidad()->getParentSceneNode()->getPosition() + Ogre::Vector3::UNIT_Y * 2);
    Ogre::Vector3 goalOffset = mCameraGoal->_getDerivedPosition() - nodeCam->getPosition();
    nodeCam->translate(goalOffset * deltaT * 9.0f);
    nodeCam->lookAt(mCameraPivot->_getDerivedPosition(), Ogre::Node::TS_PARENT);
    //lgAnimacion->addTime(deltaT);

    ent2->setTiempo(deltaT);

    lgMundo->stepSimulation(deltaT, 5);
    m_dynamicsWorld->debugDrawWorld();
    dbdraw->step();
    updateBody();

    mSoundManager->update(evt.timeSinceLastFrame);

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