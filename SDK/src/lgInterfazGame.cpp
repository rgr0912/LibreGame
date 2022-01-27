#include "lgIMGUI.h"
bool p;
void lgGUI::lgInterfazGame(bool *p, Ogre::RenderWindow *win, Ogre::String inf)
{
    ImGuiWindowFlags window_flags2 = ImGuiWindowFlags_NoTitleBar;
    window_flags2 |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags2 |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    int h = win->getHeight();
    int w = win->getWidth();
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2(w, 150));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));

    ImGui::Begin("Interfaz", p, window_flags2);
    ImGui::SetWindowFontScale(2.0f);

    ImGui::TextWrapped("Busca la puerta para salir, pero antes debes encontrar las llaves que estan ocultas en la casa.");
    ImGui::Text("%s", inf.c_str());

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(3);
}