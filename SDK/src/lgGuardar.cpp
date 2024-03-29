#include "lgIMGUI.h"
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
#define IM_CLAMP(V, MN, MX) ((V) < (MN) ? (MN) : (V) > (MX) ? (MX) : (V))

void lgGUI::guardar(bool *p, Ogre::SceneManager *manager)
{
    ImGuiWindowFlags window_flags = NULL;
    //propiedades de la escena
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);

    ImGui::Begin("Guardar proyeco", p, window_flags);
    static float progress_ini = 0.0f, progress_fin = 1.0f;

    static bool animate = false;
    if (animate == true)
    {
        progress_ini += progress_fin * 0.5f * ImGui::GetIO().DeltaTime;
        if (progress_ini >= +1.1f)
        {
            cout << "carga completa" << endl;
            animate = false;
            progress_ini = 0.0f;
            guardar_xml = false;
            recargar_escena = true;
        }
    }

    if (ImGui::Button("Guardar"))
    {

        int contador_ent = 0;
        //obteniendo entidades del SceneManager de ogre
        Ogre::SceneManager::MovableObjectIterator mIterator = manager->getMovableObjectIterator("Entity");
        while (mIterator.hasMoreElements())
        {
            e = static_cast<Ogre::Entity*>(mIterator.getNext());
            std::string objeto = e->getName();
            Ogre::SceneNode *_nodeImgui = e->getParentSceneNode();
            const char *c = objeto.c_str();

            guardarXMLE(e);

            ImGui::Text(c);
            ImGui::Separator();
            contador_ent = contador_ent + 1;
        }
        int contador_l = 0;
        //obteniendo luces
        Ogre::SceneManager::MovableObjectIterator lIterator = manager->getMovableObjectIterator("Light");
        while (lIterator.hasMoreElements())
        {
            l = static_cast<Ogre::Light*>(lIterator.getNext());
            std::string objeto = l->getName();
            Ogre::SceneNode *_nodeImgui = l->getParentSceneNode();
            const char *c = objeto.c_str();
            guardarXMLL(l);

            contador_l = contador_l + 1;
        }
        animate = true;
        //escena_carga = true;
    }
    ImGui::ProgressBar(progress_ini, ImVec2(0.0f, 0.0f));
    ImGui::PopStyleVar();
    ImGui::End();
}
#endif