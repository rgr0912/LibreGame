#include "lgIMGUI.h"
/**
bool *w;
void lgGUI::SelectorColorLuz(Ogre::Light *l)
{
    ImGuiWindowFlags window_flags = NULL;
    //editor de color de las luces en difuso y especular
    ImGui::Begin("Propiedades de luz", w, window_flags);
    ImGui::BeginChild("Color vista", ImVec2(180, 0), true);

    static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
    static ImVec4 color_especular = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

    static bool alpha_preview = false;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = false;
    static bool options_menu = false;
    static bool hdr = false;

    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    static bool ref_color = false;
    static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
    static int display_mode = 0;
    static int picker_mode = 2;

    ImGuiColorEditFlags flags = misc_flags;

    if (picker_mode == 2)
        flags |= ImGuiColorEditFlags_PickerHueWheel;

    ImGui::Text("Difuso");
    color.x = l->getDiffuseColour().r;
    color.y = l->getDiffuseColour().g;
    color.z = l->getDiffuseColour().b;
    color.w = l->getDiffuseColour().a;

    ImGui::ColorPicker4("##d", (float *)&color, flags, ref_color ? &ref_color_v.x : NULL);
    l->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
    ImGui::Separator();

    ImGui::Text("Especular");
    color_especular.x = l->getSpecularColour().r;
    color_especular.y = l->getSpecularColour().g;
    color_especular.z = l->getSpecularColour().b;
    color_especular.w = l->getSpecularColour().a;

    ImGui::ColorPicker4("##s", (float *)&color_especular, flags, ref_color ? &ref_color_v.x : NULL);
    l->setSpecularColour(Ogre::ColourValue(color_especular.x, color_especular.y, color_especular.z, color_especular.w));
    ImGui::Separator();

    ImGui::Text("Tipo de luz");
    const char *tipo_luz[] = {"Punto", "Foco", "Direccional"};
    static int luz_actual = 0;
    ImGui::Combo("##luces", &luz_actual, tipo_luz, IM_ARRAYSIZE(tipo_luz));
    if (luz_actual == 0)
    {
        l->setType(Ogre::Light::LT_POINT);
    }
    else if (luz_actual == 1)
    {
        l->setType(Ogre::Light::LT_SPOTLIGHT);
    }
    else if (luz_actual == 2)
    {
        l->setType(Ogre::Light::LT_DIRECTIONAL);
    }
    ImGui::EndChild();
    ImGui::End();
}
void lgGUI::SelectorColorVista(bool *w, Ogre::Viewport *vp, Ogre::SceneManager *sm)
{
    ImGuiWindowFlags window_flags = NULL;

    //editar color de fondo
    ImGui::Begin("Propiedades de ventana", w, window_flags);

    static bool alpha_preview = false;
    static bool alpha_half_preview = false;
    static bool drag_and_drop = false;
    static bool options_menu = false;
    static bool hdr = false;

    ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

    static bool ref_color = false;
    static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
    static int display_mode = 0;
    static int picker_mode = 2;

    ImGuiColorEditFlags flags = misc_flags;

    if (picker_mode == 2)
        flags |= ImGuiColorEditFlags_PickerHueWheel;

    static ImVec4 color_fondo = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
    static ImVec4 color_ambiente_luz = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

    ImGui::Columns(2);
    ImGui::Text("Color de fondo");
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Color vista", ImVec2(200, 0), true);

    ImGui::ColorPicker4("##f", (float *)&color_fondo, flags, ref_color ? &ref_color_v.x : NULL);
    vp->setBackgroundColour(Ogre::ColourValue(color_fondo.x, color_fondo.y, color_fondo.z, color_fondo.w));

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::NextColumn();
    ImGui::Text("Luz de ambiente");
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 5.0f);
    ImGui::BeginChild("Color ambiente", ImVec2(200, 0), true);
    ImGui::ColorPicker4("##a", (float *)&color_ambiente_luz, flags, ref_color ? &ref_color_v.x : NULL);
    sm->setAmbientLight(Ogre::ColourValue(color_ambiente_luz.x, color_ambiente_luz.y, color_ambiente_luz.z, color_ambiente_luz.w));

    ImGui::EndChild();
    ImGui::PopStyleVar();

    ImGui::End();
}
void lgGUI::Propiedades(bool *p, Ogre::RenderWindow *win, Ogre::Entity *ent, Ogre::String nombre)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_HorizontalScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

    int h = win->getHeight();
    int w = win->getWidth();
    ImGui::SetNextWindowPos(ImVec2(w - 225, tmenubar + tmenuherrmientas));
    ImGui::SetNextWindowSize(ImVec2(225, h - (tmenubar + tmenuherrmientas)));

    //obteniendo y editando propiedades de las entidades
    ImGui::Begin("Propiedades de: ", p, window_flags);
    if (nombre != "no")
    {
        Ogre::String nombre = ent->getName();
        const char *c = nombre.c_str();
        ImGui::Text(c);
        Ogre::Vector3 v = ent->getParentNode()->getPosition();
        static float x, y, z;
        x = v.x;
        y = v.y;
        z = v.z;

        //posision de la entidad
        ImGui::Text("Posicion");
        ImGui::DragFloat("X##pos", &x);
        ImGui::DragFloat("Y##pos", &y);
        ImGui::DragFloat("Z##pos", &z);
        ent->getParentNode()->setPosition(x, y, z);
        ImGui::Separator();

        //escala de la entidad
        ImGui::Text("Escala");
        Ogre::Vector3 s = ent->getParentNode()->getScale();
        static float x_s, y_s, z_s;
        x_s = s.x;
        y_s = s.y;
        z_s = s.z;
        ImGui::InputFloat("X##esc", &x_s);
        ImGui::InputFloat("Y##esc", &y_s);
        ImGui::InputFloat("Z##esc", &z_s);
        ent->getParentNode()->setScale(x_s, y_s, z_s);

        //asignar material
        ImGui::Separator();
        ImGui::Text("Material");
        const char *tipo_material[] = {"Punto", "Foco", "Direccional"};
        static int material_actual = 0;
        ImGui::Combo("##material", &material_actual, tipo_material, IM_ARRAYSIZE(tipo_material));
        if (material_actual == 0)
        {
            //ent->setMaterialName("Material");
        }
        else if (material_actual == 1)
        {
            //ent->setMaterialName("RTSS/NormalMapping_MultiPass");
        }
        else if (material_actual == 2)
        {
            //ent->setMaterialName("RTSS/NormalMapping_SinglePass");
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}
void lgGUI::PropiedadesLuz(bool *p, Ogre::RenderWindow *win, Ogre::Light *l, Ogre::String nombre)
{
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus | ImGuiWindowFlags_HorizontalScrollbar;

    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

    int h = win->getHeight();
    int w = win->getWidth();
    ImGui::SetNextWindowPos(ImVec2(w - 225, tmenubar + tmenuherrmientas));
    ImGui::SetNextWindowSize(ImVec2(225, h - (tmenubar + tmenuherrmientas)));

    //obteniendo y editando propiedades de las entidades
    ImGui::Begin("Propiedades de: ", p, window_flags);
    if (ImGui::CollapsingHeader("Entidades"))
    {
        if (nombre != "no")
        {
            Ogre::String nombre = l->getName();
            const char *c = nombre.c_str();
            ImGui::Text(c);
            ///
        Ogre::Vector3 v = l->getParentNode()->getPosition();
        static float x, y, z;
        x = v.x;
        y = v.y;
        z = v.z;

        //posision de la entidad
        ImGui::Text("Posicion");
        ImGui::DragFloat("X##pos", &x);
        ImGui::DragFloat("Y##pos", &y);
        ImGui::DragFloat("Z##pos", &z);
        l->getParentNode()->setPosition(x, y, z);
        ImGui::Separator();

        //escala de la entidad
        ImGui::Text("Escala");
        Ogre::Vector3 s = l->getParentNode()->getScale();
        static float x_s, y_s, z_s;
        x_s = s.x;
        y_s = s.y;
        z_s = s.z;
        ImGui::InputFloat("X##esc", &x_s);
        ImGui::InputFloat("Y##esc", &y_s);
        ImGui::InputFloat("Z##esc", &z_s);
        l->getParentNode()->setScale(x_s, y_s, z_s);
        ///
            //asignar material
            ImGui::BeginChild("Color vista", ImVec2(190, 0), true);

            static ImVec4 color = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
            static ImVec4 color_especular = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

            static bool alpha_preview = false;
            static bool alpha_half_preview = false;
            static bool drag_and_drop = false;
            static bool options_menu = false;
            static bool hdr = false;

            ImGuiColorEditFlags misc_flags = (hdr ? ImGuiColorEditFlags_HDR : 0) | (drag_and_drop ? 0 : ImGuiColorEditFlags_NoDragDrop) | (alpha_half_preview ? ImGuiColorEditFlags_AlphaPreviewHalf : (alpha_preview ? ImGuiColorEditFlags_AlphaPreview : 0)) | (options_menu ? 0 : ImGuiColorEditFlags_NoOptions);

            static bool ref_color = false;
            static ImVec4 ref_color_v(1.0f, 0.0f, 1.0f, 0.5f);
            static int display_mode = 0;
            static int picker_mode = 2;

            ImGuiColorEditFlags flags = misc_flags;

            if (picker_mode == 2)
                flags |= ImGuiColorEditFlags_PickerHueWheel;

            ImGui::Text("Difuso");
            color.x = l->getDiffuseColour().r;
            color.y = l->getDiffuseColour().g;
            color.z = l->getDiffuseColour().b;
            color.w = l->getDiffuseColour().a;

            ImGui::ColorPicker4("##d", (float *)&color, flags, ref_color ? &ref_color_v.x : NULL);
            l->setDiffuseColour(Ogre::ColourValue(color.x, color.y, color.z, color.w));
            ImGui::Separator();

            ImGui::Text("Especular");
            color_especular.x = l->getSpecularColour().r;
            color_especular.y = l->getSpecularColour().g;
            color_especular.z = l->getSpecularColour().b;
            color_especular.w = l->getSpecularColour().a;

            ImGui::ColorPicker4("##s", (float *)&color_especular, flags, ref_color ? &ref_color_v.x : NULL);
            ImGui::ColorEdit3("clear color", (float*)&color_especular);
            l->setSpecularColour(Ogre::ColourValue(color_especular.x, color_especular.y, color_especular.z, color_especular.w));
            ImGui::EndChild();
            ImGui::Separator();

            ImGui::Text("Tipo de luz");
            const char *tipo_luz[] = {"Punto", "Foco", "Direccional"};
            static int luz_actual = 0;
            ImGui::Combo("##luces", &luz_actual, tipo_luz, IM_ARRAYSIZE(tipo_luz));
            if (luz_actual == 0)
            {
                l->setType(Ogre::Light::LT_POINT);
            }
            else if (luz_actual == 1)
            {
                l->setType(Ogre::Light::LT_SPOTLIGHT);
            }
            else if (luz_actual == 2)
            {
                l->setType(Ogre::Light::LT_DIRECTIONAL);
            }
        }
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}
void lgGUI::PropiedadesCamara(bool *w, Ogre::Camera *cam)
{
    ImGuiWindowFlags window_flags = NULL;
    //propiedades de camara root
    ImGui::Begin("Propiedades de camara", w, window_flags);

    Ogre::Vector3 vc = cam->getParentNode()->getPosition();
    static float x, y, z;
    x = vc.x;
    y = vc.y;
    z = vc.z;

    ImGui::Text("Posicion: ");
    ImGui::BeginChild("ChildC", ImVec2(0, 100), true, window_flags);
    ImGui::Columns(3);
    ImGui::DragFloat("X##pos", &x);
    ImGui::NextColumn();
    ImGui::DragFloat("Y##pos", &y);
    ImGui::NextColumn();
    ImGui::DragFloat("Z##pos", &z);
    cam->getParentNode()->setPosition(x, y, z);
    ImGui::EndChild();

    static float cercania, lejania;
    ImGui::Text("Distancia focal: ");
    ImGui::BeginChild("DistanciaCamara", ImVec2(0, 100), true, window_flags);
    ImGui::Columns(2);
    ImGui::InputFloat("Cercania##pos", &cercania);
    if (ImGui::Button("Aplicar##1"))
        cam->setNearClipDistance(cercania);
    ImGui::NextColumn();
    ImGui::InputFloat("Lejania##pos", &lejania);
    if (ImGui::Button("Aplicar##2"))
        cam->setFarClipDistance(lejania);

    static float focal;
    ImGui::InputFloat("F##foc", &focal);
    if (ImGui::Button("Aplicar##3"))
        cam->setFOVy(Ogre::Radian(focal));
    ImGui::EndChild();

    ImGui::End();
}
void lgGUI::RenderCamara(bool *w)
{
    ImGuiWindowFlags window_flags = NULL;
    //previsualizacion del render de la camara del player
    ImGui::Begin("Render camara", w, window_flags);
    int w_render = ImGui::GetWindowWidth();
    int h_render = ImGui::GetWindowHeight();

    ImGui::BeginChild("Contenedor camara render player", ImVec2(0, 0), true, window_flags);
    ImGui::Image((ImTextureID)RenderOgre(), ImVec2(w_render - 50, h_render - 60));

    ImGui::EndChild();

    ImGui::End();
}
Ogre::ResourceHandle lgGUI::RenderOgre()
{
    //obteniendo la textura con el render de la escena para el render de la camara
    Ogre::TexturePtr t = Ogre::TextureManager::getSingleton().getByName("RttTex", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    t->setFSAA(4, "");
    Ogre::ResourceHandle hdl = t->getHandle();

    return hdl;
}
**/