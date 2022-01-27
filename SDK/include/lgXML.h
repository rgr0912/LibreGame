#include "tinyxml2.h"
#include <iostream>
#include <vector>
#include <unistd.h>
#include <filesystem>

#include "imgui.h"

#include "Ogre.h"

using namespace std;
using namespace tinyxml2;

class lgXML
{
public:
    Ogre::String mString;
    bool cargarXML();
    bool escenaXML();
    void agregarMesh(std::string nombre, std::string ruta, Ogre::SceneManager *sm);
    void agregarLuz(std::string nombre, std::string tipo, Ogre::SceneManager *sm);
    void nuevaEscenaXml(bool *p);
    void guardarXMLE(Ogre::Entity *ent);
    void guardarXMLL(Ogre::Light *l);
    const char *xmlfile = "../SDK/configsys.xml ";
    XMLError errorXML;
    XMLDocument lgDoc;
    bool nueva_escena_xml;
    bool guardar_xml;
    bool recargar_escena;
    string ruta_root;
    string ruta_xml_file;
};