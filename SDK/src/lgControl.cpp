#include "LibreGame.h"

bool LibreGame::keyPressed(const OgreBites::KeyboardEvent &evt)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
#if EXPORTAR == 0
    KeyCode key = evt.keysym.sym;
    if (key == SDLK_i)
    {
        if (interfaz_gui == true)
        {
            interfaz_gui = false;
        }
        else
        {
            interfaz_gui = true;
        }
    }
    if (key == SDLK_p)
    {
        
        if (play == false)
        {
            camPlayer();
            play = true;
            interfaz_gui = false;
        }
        else
        {
            camRoot();
            play = false;
            interfaz_gui = true;
        }
    }
    if (key == SDLK_r)
    {
        if (rotar_camara == false && interfaz_gui == true)
        {
            rotar_camara = true;
            interfaz_gui = false;
        }
        else
        {
            rotar_camara = false;
            interfaz_gui = true;
        }
    }
    if (key == SDLK_c)
    {
        camPlayer();
    }
    if (interfaz_gui == false)
    {
        if (key == SDLK_b)
        {
            d += 10;
        }
        else if (key == SDLK_n)
        {
            d -= 10;
        }

        if (key == SDLK_t)
        {
            dy += 10;
        }
        else if (key == SDLK_g)
        {
            dy -= 10;
        }

        if (key == SDLK_h)
        {
            dz += 10;
        }
        else if (key == SDLK_j)
        {
            dz -= 10;
        }
    }
#endif
#endif
    return false;
}
bool LibreGame::keyReleased(const OgreBites::KeyboardEvent &evt)
{
    d = 0;
    dy = 0;
    dz = 0;
    return false;
}
bool selec = false;
bool tp = false;
bool LibreGame::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    lgTmanager->mouseMoved(evt);

    if (m_cam_interfaz == true && rotar_camara == true)
    {
        moverCamara(-0.5f * evt.xrel, -0.5f * evt.yrel, 0);
    }
    if (tp == true)
    {
        sdl_seleccion(lgVentana, lgCam, selec, lgRayManager, evt.x, evt.yrel, evt.xrel);
    }
#endif
    return true;
}
bool LibreGame::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
    lgTmanager->mousePressed(evt);
    boton1.presionar_boton = true;
    boton2.presionar_boton = true;
    boton3.presionar_boton = true;
    boton4.presionar_boton = true;

    m_cam_interfaz = true;
    tp = true;

    return true;
}
bool LibreGame::mouseReleased(const OgreBites::MouseButtonEvent &evt)
{
    lgTmanager->mouseReleased(evt);
    boton1.setValor(0);
    boton2.setValor(0);
    boton3.setValor(0);
    boton4.setValor(0);
    boton1.presionar_boton = false;
    boton2.presionar_boton = false;
    boton3.presionar_boton = false;
    boton4.presionar_boton = false;

    m_cam_interfaz = false;
    tp = false;

    return true;
}
bool LibreGame::touchPressed(const OgreBites::TouchFingerEvent &evt)
{
    lgTmanager->touchPressed(evt);
    boton1.presionar_boton = true;
    boton2.presionar_boton = true;
    boton3.presionar_boton = true;
    boton4.presionar_boton = true;
    //boton1->getPres();

    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[0] = true;

    return true;
}
bool LibreGame::touchReleased(const OgreBites::TouchFingerEvent &evt)
{
    lgTmanager->touchReleased(evt);
    boton1.setValor(0);
    boton2.setValor(0);
    boton3.setValor(0);
    boton4.setValor(0);
    boton1.presionar_boton = false;
    boton2.presionar_boton = false;
    boton3.presionar_boton = false;
    boton4.presionar_boton = false;

    ImGuiIO &io = ImGui::GetIO();
    io.MouseDown[0] = false;
    return true;
}
bool LibreGame::touchMoved(const OgreBites::TouchFingerEvent &evt)
{
    return true;
}