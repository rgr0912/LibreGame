#include "lgIMGUI.h"
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
//define carpeta raiz del usuario
static char pwd[512] = "/home";

//aplicacion para abril los archivos de edicion
std::string base("gedit");

std::string archivo_seleccion;
//visualizar de archivos
void lgGUI::archivos(bool *p, Ogre::String extension, Ogre::SceneManager *sm)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    ImGui::SetNextWindowSize(ImVec2(500, 440), ImGuiCond_FirstUseEver);
    static std::string directorio_selecionado = "/home";
    if (ImGui::Begin("Explorador de archivos", p))
    {
        ImGui::BeginChild("raiz", ImVec2(0, 100), true);
        ImGui::Text("Volver");
        ImGui::SameLine;
        static bool inicio;
        //restablce la ruta predefinida
        if (ImGui::Selectable("Inicio", inicio))
            directorio_selecionado = "/home";
        ImGui::Separator();
        ImGui::Text("Ruta actual: ");
        ImGui::SameLine;
        //muestra la ruta de la carpeta seleccionada
        ImGui::Text(directorio_selecionado.c_str());
        ImGui::EndChild();

        //estable la ruta de la nueva carpta seleccionada
        const char *pwd1 = directorio_selecionado.c_str();

        //se muestra todas las carpetas de la ruta actual
        ImGui::BeginChild("panel izquierdo", ImVec2(350, 250), true);
        for (auto &p : std::experimental::filesystem::directory_iterator(pwd1))
        {
            char label[128];
            if (p.path().extension() == "")
            {
                sprintf(label, p.path().c_str(), p.path().c_str());
                if (ImGui::Selectable(label, directorio_selecionado == p.path().c_str()))
                    directorio_selecionado = p.path().c_str();
            }
        }
        ImGui::EndChild();
        ImGui::SameLine();
        //se muestra todos los archivos que estan dentro de la carpta por extension de archivo
        //ImGui::BeginChild("visualizador de archivos", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
        const char *pwd2 = directorio_selecionado.c_str();
        static std::string subdirectorio_seleccionado;

        ImGui::BeginChild("panel derecho", ImVec2(0, 250), true);
        //ImGui::Text("Lista de archivos");
        for (auto &p : std::experimental::filesystem::directory_iterator(pwd2))
        {
            if (p.path().extension() == extension)
            {
                char label[128];
                if (extension == ".material")
                    sprintf(label, p.path().c_str(), p.path().c_str());
                if (extension == ".mesh")
                    sprintf(label, p.path().filename().c_str(), p.path().filename().c_str());
                if (ImGui::Selectable(label, subdirectorio_seleccionado == p.path() | subdirectorio_seleccionado == p.path().filename()))
                {
                    if (extension == ".material")
                        subdirectorio_seleccionado = p.path().c_str();
                    if (extension == ".mesh")
                        subdirectorio_seleccionado = p.path().filename().c_str();
                    //archivo_seleccion = "";
                }
                else
                {
                    archivo_seleccion = subdirectorio_seleccionado;
                }
            }
        }
        ImGui::EndChild();

        //ImGui::EndChild();
        //se abre el archivo seleccionado con el editor del sistema
        ImGui::BeginChild("panel inferior", ImVec2(0, 40), true);
        static char nombre[254] = "";
        static char tipo[254] = "";
        if (extension == ".mesh")
        {
            ImGui::InputText("##1", nombre, IM_ARRAYSIZE(nombre));
        }
        else if (extension == "luz")
        {
            ImGui::InputText("##1", nombre, IM_ARRAYSIZE(nombre));
            ImGui::InputText("##2", tipo, IM_ARRAYSIZE(tipo));
        }
        ImGui::SameLine();
        if (ImGui::Button("Abrir"))
        {
            if (extension == ".material")
            {
                base = +"gedit " + archivo_seleccion;
                const char *c = base.c_str();
                std::system(c);
            }
            else if (extension == ".mesh")
            {
                agregarMesh(nombre, archivo_seleccion, sm);
                abrir_mesh = false;
            }
            else if (extension == "luz")
            {
                agregarLuz(nombre, tipo, sm);
                agregar_luz = false;
            }
        }
        ImGui::EndChild();
    }
    ImGui::End();
#endif
}
//visualizar de archivos
void lgGUI::agregarLuzGUI(bool *p, Ogre::String extension, Ogre::SceneManager *sm)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    ImGui::SetNextWindowSize(ImVec2(0, 0), ImGuiCond_FirstUseEver);

    if (ImGui::Begin("Agregar luz a la escena", p))
    {
        //ImGui::EndChild();
        //se abre el archivo seleccionado con el editor del sistema
        ImGui::BeginChild("panel inferior", ImVec2(0, 0), true);
        static char nombre[254] = "";
        //static char tipo[254] = "";
        ImGui::LabelText("", "Nombre del objeto");
        ImGui::InputText("##1", nombre, IM_ARRAYSIZE(nombre));
        //ImGui::InputText("##2", tipo, IM_ARRAYSIZE(tipo));
        ImGui::LabelText("", "Tipo de objeto");
        const char *luz[] = {"Foco", "Punto", "Direccional"};
        static int tipo_luz = 0;

        ImGui::Combo("", &tipo_luz, luz, IM_ARRAYSIZE(luz));

        std::string tipo;
        
        switch(tipo_luz){
            case 0 : tipo = "Foco"; break;
            case 1 : tipo = "Punto"; break;
            case 2 : tipo = "Direccional"; break;
        }

        if (ImGui::Button("Agregar"))
        {

            agregarLuz(nombre, tipo, sm);
        }
        ImGui::EndChild();
    }
    ImGui::End();
#endif
}
#endif