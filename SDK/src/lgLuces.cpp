#include "lgLuces.h"

int px, py, pz;
static ImVec4 color_difuso = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
static ImVec4 color_especular = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
float r_p = 0, r_y = 0, r_r = 0;
void lgLuces::setNombre(Ogre::String nombre)
{
    nombre_luz = nombre;
}
void lgLuces::setTipo(Ogre::String tipo)
{
    if(tipo=="Foco"){
        tipo_luz = 0;
    }else if(tipo=="Punto"){
        tipo_luz = 1;
    }else if(tipo=="Direccional"){
        tipo_luz = 2;
    }
}
void lgLuces::mostrar(Ogre::SceneManager *sm)
{
    sm_luz = sm;
    luz = luzOgre();
}
int lgLuces::tipo(int i){

    return i;
}
Ogre::Light *lgLuces::luzOgre()
{
    Ogre::Light *l = sm_luz->createLight(nombre_luz);
    
    switch(tipo_luz){
        case 0 : l->setType(Ogre::Light::LT_SPOTLIGHT); break;
        case 1 : l->setType(Ogre::Light::LT_POINT); break;
        case 2 : l->setType(Ogre::Light::LT_DIRECTIONAL); break;
    }

    l->setDiffuseColour(Ogre::ColourValue(color_difuso.x,color_difuso.y,color_difuso.z,color_difuso.w));
    l->setSpecularColour(Ogre::ColourValue(color_especular.x,color_especular.y,color_especular.z,color_especular.w));
    Ogre::ManualObject *f = sm_luz->createManualObject();

    f->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
    f->position(2, 0, 0);
    f->position(-2, 0, 0);
    f->index(0);
    f->index(1);
    f->end();

    f->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
    f->position(0, 0, 2);
    f->position(0, 0, -2);
    f->index(0);
    f->index(1);
    f->end();

    f->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
    f->position(0, 2, 0);
    f->position(0, -2, 0);
    f->index(0);
    f->index(1);
    f->end();

    nodo = sm_luz->getRootSceneNode()->createChildSceneNode(nombre_luz);
    nodo->attachObject(l);
    nodo->attachObject(f);
    nodo->setPosition(px, py, pz);
    nodo->pitch(Ogre::Degree(r_p));
    nodo->yaw(Ogre::Degree(r_y));
    nodo->roll(Ogre::Degree(r_r));
    return l;
}
void lgLuces::setPosicion(int x, int y, int z)
{
    px = x, py = y, pz = z;
}
void lgLuces::setRotacion(float pitch, float yaw, float roll){
    r_p = pitch, r_y = yaw, r_r = roll;
}
void lgLuces::setColorDifuso(ImVec4 color){
    color_difuso.x = color.x,color_difuso.y = color.y,color_difuso.z = color.z,color_difuso.w = color.w;
}
void lgLuces::setColorEspecular(ImVec4 color){
    color_especular.x = color.x,color_especular.y = color.y,color_especular.z = color.z,color_especular.w = color.w;
}