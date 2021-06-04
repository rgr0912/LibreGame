#include "lgIMGUI.h"

void lgGUI::CrearControlGui(bool *p_open, Ogre::RenderWindow *win)
{

    int w = win->getWidth();
    int h = win->getHeight();
    
    boton1.Boton("boton1", 0, 0);
    boton1.setPosicion(20, h - (100));
    boton1.setDimension(60, 60);
    boton1.mostrar();

    boton2.Boton("boton2", 0, 1);
    boton2.setPosicion(100, h - (20));
    boton2.setDimension(60, 60);
    boton2.mostrar();

    boton3.Boton("boton3", 0, 1);
    boton3.setPosicion(180, h - (100));
    boton3.setDimension(60, 60);
    boton3.mostrar();

    boton4.Boton("boton4", 0, 0);
    boton4.setPosicion(100, h - (60 + 120));
    boton4.setDimension(60, 60);
    boton4.mostrar();

    boton5.Boton("boton5", 1, 0);
    boton5.setPosicion(w - 100, h - (20));
    boton5.setDimension(60, 60);
    boton5.mostrar();

    boton6.Boton("boton6", 2, 0);
    boton6.setPosicion(w - 180, h - (100 + 20));
    boton6.setDimension(60, 60);
    boton6.mostrar();
}
void lgGUI::info(bool *p)
{
    const float DISTANCE = 10.0f;
    static int corner = 2;
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    if (corner != -1)
    {
        window_flags |= ImGuiWindowFlags_NoMove;
        ImVec2 window_pos = ImVec2(500, 500);
        ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
        ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
    }

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGui::Begin("info", p, window_flags);
    //ImGui::Text("valor %d",boton1->getContador());
    ImGui::End();
}