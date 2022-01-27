#include "lgIMGUI.h"

void lgGUI::FPS(bool *p_open, int x, int y)
{
    ImGui::Text(">>> %d   <<<  %d", x,y);
}
void lgGUI::setValor(int x, int y){
    ax = x, ay = y;
}
void lgGUI::setValorW(int x, int y){
    axw = x, ayw = y;
}
void lgGUI::CrearControlGui(bool *p_open, Ogre::RenderWindow *win)
{

    int w = win->getWidth();
    int h = win->getHeight();

    //ImGui::Text("RENDER WINDOW %d   <<<  %d", w,h);
    //ImGui::Text("RENDER ANDROID %d   <<<  %d", axw,ayw);
    //ImGui::Text(" valor en x : %d valor en y : %d  ",ax,ay);
    int b_w=100, b_h=100, sep=30;
    
    boton1.Boton("boton1", 0, 0);
    boton1.setPosicion(sep, h - (b_h+(sep*2)));
    boton1.setDimension(b_w, b_h);
    boton1.mostrar();

    boton2.Boton("boton2", 0, 1);
    boton2.setPosicion(b_w+(sep*2), h - (sep));
    boton2.setDimension(b_w, b_h);
    boton2.mostrar();

    boton3.Boton("boton3", 0, 1);
    boton3.setPosicion((b_w*2)+(sep*3), h - (b_h+(sep*2)));
    boton3.setDimension(b_w, b_h);
    boton3.mostrar();

    boton4.Boton("boton4", 0, 0);
    boton4.setPosicion(b_w+(sep*2), h - ((b_h*2)+(sep*3)));
    boton4.setDimension(b_w, b_h);
    boton4.mostrar();

    boton5.Boton("boton5", 1, 0);
    boton5.setPosicion(w - (b_w+(sep*2)), h - (sep));
    boton5.setDimension(b_w, b_h);
    boton5.mostrar();

    boton6.Boton("boton6", 2, 0);
    boton6.setPosicion(w - (b_w*3), h - ((b_h*2) + sep));
    boton6.setDimension(b_w, b_h);
    boton6.mostrar();
}
void lgGUI::info(bool *p)
{
    ImGuiIO &io = ImGui::GetIO();
    ImGuiWindowFlags window_flags = NULL;//ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
    ImVec2 window_pos = ImVec2(0, 0);

    ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
    ImGui::Begin("info", p, window_flags);
    ImGui::Text("valor %d",10);
    ImGui::End();
}