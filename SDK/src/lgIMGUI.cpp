#include "lgIMGUI.h"

void lgGUI::MenuBar(bool *p_open)
{
    ImGuiIO io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    int sx = ImGui::GetMousePos().x;
    int sy = ImGui::GetMousePos().y;
    ImGuiWindowFlags window_flags = NULL;
    //propiedades de camara root
    ImGui::Begin("Propiedades de camara ", p_open, window_flags);
    ImGui::Text(">>> >>>>>>>>>>>>>>>>>>>>>>>>>");
    ImGui::Text(">>> %d   <<<  %d", sx, sy);
    ImGui::End();
}
void lgGUI::mois(bool *p_open, int x, int y)
{
    ImGuiIO io = ImGui::GetIO();
    io.MouseDrawCursor = true;
    int sx = ImGui::GetMousePos().x;
    ImGuiWindowFlags window_flags = NULL;
    //propiedades de camara root
    ImGui::Begin("Propiedades de camara ", p_open, window_flags);
    ImGui::Text(">>> %d   <<<  %d", x,y);
    ImGui::End();
}
void lgGUI::MenuHerramientas(bool *p, Ogre::RenderWindow *win, Ogre::Viewport *vp, Ogre::SceneManager *sm, Ogre::Camera *cam)
{
    /**ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    int h = win->getHeight();
    int w = win->getWidth();
    ImGui::SetNextWindowPos(ImVec2(0, tmenubar));
    ImGui::SetNextWindowSize(ImVec2(w, 70));

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.11f, 0.95f));

    ImGui::Begin("Debug", p, window_flags);
    tmenuherrmientas = ImGui::GetWindowHeight();

    if (nueva_escena_xml)nuevaEscenaXml(&nueva_escena_xml);

    static bool mostrar_propiedades_ventana = false;
    if (mostrar_propiedades_ventana)
        SelectorColorVista(&mostrar_propiedades_ventana, vp, sm);

    static bool mostrar_propiedades_camara = false;
    if (mostrar_propiedades_camara)
        PropiedadesCamara(&mostrar_propiedades_camara, cam);

    static bool mostrar_explorador_entidades = false;
    if (mostrar_explorador_entidades)
        Explorador(&mostrar_explorador_entidades, win, sm);

    static bool mostrar_render_camara = false;
    if (mostrar_render_camara)
        RenderCamara(&mostrar_render_camara);

    static std::string etiqueta;

    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("ChildR", ImVec2(400, 0), true);

    ImGui::Columns(2);
    if (ImGui::Selectable("Explorador de entidades", etiqueta == "Explorador de entidades"))
    {
        etiqueta = "Explorador de entidades";
        if (mostrar_explorador_entidades == true)
        {
            mostrar_explorador_entidades = false;
        }
        else
        {
            mostrar_explorador_entidades = true;
        }
    }
    if (ImGui::Selectable("Propiedades de fondo", etiqueta == "Propiedades de fondo"))
    {
        etiqueta = "Propiedades de fondo";
        mostrar_propiedades_ventana = true;
    }
    ImGui::NextColumn();
    if (ImGui::Selectable("Mostrar render camara player", etiqueta == "Mostrar render camara player"))
    {
        etiqueta = "Mostrar render camara player";
        if (mostrar_render_camara == true)
        {
            mostrar_render_camara = false;
        }
        else
        {
            mostrar_render_camara = true;
        }
    }
    if (ImGui::Selectable("Propiedades de camara", etiqueta == "Propiedades de camara"))
    {
        etiqueta = "Propiedades de camara";
        mostrar_propiedades_camara = true;
    }
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::SameLine();
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 0.0f);
    ImGui::BeginChild("ChildL", ImVec2(0, 0), true, window_flags);
    ImGui::Columns(2);
    //ImGui::Checkbox("Activar edicion sdl",&msdl);
    const Ogre::RenderTarget::FrameStats &stats = win->getStatistics();
    ImGui::NextColumn();
    ImGui::Text("Rendimietno cuadros x segundo: %F", stats.lastFPS);
    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);**/
}