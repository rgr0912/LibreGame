#include "lgIMGUI.h"
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
Ogre::String objeto;
Ogre::SceneNode* _nodeImgui;
Ogre::SceneNode* _nodeImguiL;
static int rb = 1;
static int rl = 1;

void lgGUI::Explorador(bool* p, Ogre::RenderWindow* win, Ogre::SceneManager* manager){
    ImGuiWindowFlags window_flags =  ImGuiWindowFlags_NoTitleBar;
    window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
    window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10,10));

    //obteniendo las dimensiones de la venta para asignar tamaño
    int h = win->getHeight();
    int w = win->getWidth();
    ImGui::SetNextWindowPos(ImVec2(0,tmenubar+tmenuherrmientas));
    ImGui::SetNextWindowSize(ImVec2(200,h));

    //explorador de entidades y luces
    ImGui::Begin("Explorador",p, window_flags);
    if (ImGui::CollapsingHeader("Entidades")){
        ImGuiIO& io = ImGui::GetIO();

        int contador_ent = 0;
        //obteniendo entidades del SceneManager de ogre
	    Ogre::SceneManager::MovableObjectIterator mIterator = manager->getMovableObjectIterator("Entity");
	    while(mIterator.hasMoreElements()){
            e = static_cast<Ogre::Entity*>(mIterator.getNext());
            objeto = e->getName();
            _nodeImgui = e->getParentSceneNode();
            const char* c = objeto.c_str();
            ImGui::RadioButton(c, &rb, contador_ent);
            if(rb == contador_ent){
                Propiedades(p,win,e,c);
                _nodeImgui->showBoundingBox(true);
                seleccion = "entidad";
            }else{
                _nodeImgui->showBoundingBox(false);
            }
            ImGui::Separator();
            contador_ent = contador_ent + 1;
            }
        if(!mIterator.hasMoreElements()){
        }
        
    }
    if (ImGui::CollapsingHeader("Luces")){
        ImGuiIO& io = ImGui::GetIO();

        int contador_l = 0;
        //obteniendo luces
	    Ogre::SceneManager::MovableObjectIterator mIterator = manager->getMovableObjectIterator("Light");
	    while(mIterator.hasMoreElements()){
            l = static_cast<Ogre::Light*>(mIterator.getNext());
            objeto = l->getName();
            _nodeImguiL = l->getParentSceneNode();
            const char* c = objeto.c_str();
        
            ImGui::RadioButton(c, &rl, contador_l);
            if(rl == contador_l){
                PropiedadesLuz(p,win,l,c);
                _nodeImguiL->showBoundingBox(true);
                seleccion ="luz";
            }else{
                _nodeImguiL->showBoundingBox(false);
            }
            ImGui::Separator();
            contador_l = contador_l + 1;
            }
        if(!mIterator.hasMoreElements()){
        }
        
    }
    ImGui::End();
    ImGui::PopStyleVar(2);
}
#endif