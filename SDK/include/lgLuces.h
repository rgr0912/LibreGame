#include "Ogre.h"
#include "imgui.h"
#include <iostream>

class lgLuces
{
public:
    lgLuces(Ogre::String nombre, Ogre::String tipo)
    {
        setNombre(nombre);
        setTipo(tipo);
        setPosicion(0,10,0);
    };
    void setNombre(Ogre::String nombre);
    void setTipo(Ogre::String tipo);
    void mostrar(Ogre::SceneManager* sm);
    void setPosicion(int x, int y, int z);
    void setColorDifuso(ImVec4 color);
    void setColorEspecular(ImVec4 color);
    void setRotacion(float pitch, float yaw, float roll);
    Ogre::SceneManager *sm_luz;
    Ogre::Light *luz;
    Ogre::SceneNode *nodo;
    Ogre::String nombre_luz;
    int tipo_luz;
    Ogre::Light* luzOgre();
    int tipo(int i);
    private:
};