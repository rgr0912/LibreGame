#include "LibreGame.h"

bool LibreGame::mouseMoved(const OgreBites::MouseMotionEvent &evt)
{
    lgTmanager->mouseMoved(evt);
    moverCamara(-0.5f * evt.xrel, -0.5f * evt.yrel, 0);

    return true;
}
bool LibreGame::mousePressed(const OgreBites::MouseButtonEvent &evt)
{
    lgTmanager->mousePressed(evt);
    boton1.presionar_boton = true;
    boton2.presionar_boton = true;
    boton3.presionar_boton = true;
    boton4.presionar_boton = true;

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