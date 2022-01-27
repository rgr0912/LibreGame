#include "imgui.h"
#include "Ogre.h"

#include <iostream>

class lgBoton
{
public:
    void Boton(std::string nombre, int estilo, int dir)
    {
        if (estilo == 0)
        {
            nombre_boton = "##" + nombre;
        }
        if (estilo == 1)
        {
            nombre_boton = "A##" + nombre;
        }
        if (estilo == 2)
        {
            nombre_boton = "B##" + nombre;
        }
        direccion = dir;

        estilo_boton = estilo;
    }
    void mostrar()
    {
        crearBoton(ventana);
        setPosicion(posicion_x, posicion_y);
        setDimension(w, h);
    }
    void setPosicion(int pos_x, int pos_y)
    {
        posicion_x = pos_x;
        posicion_y = pos_y;
    }
    void setDimension(int anc_x, int alt_y)
    {
        w = anc_x;
        h = alt_y;
    }
    int getRegionX()
    {
        region_x = posicion_x + w;
        return region_x;
    }
    int getRegionY()
    {
        region_y = posicion_y + h;
        return region_y;
    }
    int getPosicionX()
    {
        return posicion_x;
    }
    int getPosicionY()
    {
        return posicion_y;
    }
    void setValor(int x){
        valor_boton = x;
    }
    int getValor(){
        return valor_boton;
    }

    int region_x = 0;
    int region_y = 0;
    std::string nombre_boton;
    int posicion_x = 0;
    int posicion_y = 0;
    int w = 0;
    int h = 0;
    int estilo_boton = 0;
    int color_boton = 0;
    int redondez = 0;
    ImVec4 color;
    int direccion = 0;
    bool* ventana;
    bool presionar_boton;
    int valor_boton=0;
    void crearBoton(bool* p)
    {
        const float DISTANCE = 10.0f;
        static int corner = 2;
        ImGuiIO &io = ImGui::GetIO();
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;
        if (corner != -1)
        {
            window_flags |= ImGuiWindowFlags_NoMove;
            ImVec2 window_pos = ImVec2(posicion_x, posicion_y);
            ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
            ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
        }

        ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
        ImGui::Begin(nombre_boton.c_str(), p, window_flags);

        if (estilo_boton == 0)
        {
            redondez = 0;
            color = ImVec4(1, 1, 1, 0.2f);
        }
        if (estilo_boton == 1)
        {
            redondez = 20;
            color = ImVec4(1, 0.0f, 0.0f, 0.5f);
        }
        if (estilo_boton == 2)
        {
            redondez = 20;
            color = ImVec4(0, 0.0f, 1.0f, 0.5f);
        }

        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, redondez);
        ImGui::PushButtonRepeat(true);

        ImGui::PushStyleColor(ImGuiCol_Button, color);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(1.0f, 0.5f, 0.0f, 0.5f));
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(1.0f, 0.5f, 0.0f, 1.0f));

        if (ImGui::Button(nombre_boton.c_str(), ImVec2(w, h)) && presionar_boton==true)
        {
            setValor(25);
            //else if(direccion == 1) setValor(25);            
        }
        
        ImGui::PopStyleColor(3);
        ImGui::PopButtonRepeat();
        ImGui::PopStyleVar();
        ImGui::End();
    }
};