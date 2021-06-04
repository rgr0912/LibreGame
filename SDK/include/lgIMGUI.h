#include "imgui.h"
#include "Ogre.h"
#include "OgreTrays.h"

#include <filesystem>
#include <iostream>
#include <experimental/filesystem>
#include <experimental/string>

#include "lgBoton.h"

//#include "lgXML.h"

using namespace std;

//Clase encargada de la parte grafica del programa usando IMGUI

class lgGUI {
    public:
    bool abrir_mesh;
    bool agregar_luz;
    bool boton_1;
    bool boton_2;
    bool press;
    static bool msdl;
    void mois(bool* p, int x, int y);
    lgBoton boton1;
    lgBoton boton2;
    lgBoton boton3;
    lgBoton boton4;
    lgBoton boton5;
    lgBoton boton6;
    private:
    Ogre::Light* l;
    Ogre::Entity* e;
    int tmenubar;
    int tmenuherrmientas;
    bool luz_seleccion = false;
    protected:
    void MenuBar(bool* p);
    
    void Explorador(bool* p, Ogre::RenderWindow* win, Ogre::SceneManager* manager);
    void RenderCamara(bool* w);
    void MenuHerramientas(bool* p, Ogre::RenderWindow* win, Ogre::Viewport* vp, Ogre::SceneManager* sm, Ogre::Camera* cam);
    void Propiedades(bool* p, Ogre::RenderWindow* win, Ogre::Entity* ent, Ogre::String nombre);
    void PropiedadesLuz(bool* p, Ogre::RenderWindow* win, Ogre::Light* l, Ogre::String nombre);
    void archivos(bool* p, Ogre::String extension, Ogre::SceneManager* mg);
    void guardar(bool* p, Ogre::SceneManager* sm);
    static void SelectorColorVista(bool* w, Ogre::Viewport* vp, Ogre::SceneManager* sm);
    static void PropiedadesCamara(bool* w, Ogre::Camera* cam);
    static void SelectorColorLuz(Ogre::Light* l);
    void CrearControlGui(bool* p, Ogre::RenderWindow* win);
    void info(bool* p);
    void lgInterfazGame(bool* p,Ogre::RenderWindow* win, Ogre::String inf);
    void miBoton(int px, int py);
    bool escena_carga = false;
    Ogre::ResourceHandle RenderOgre();
};
