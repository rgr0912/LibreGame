#include "lgXML.h"

struct _player
{
    int id;
    const char *nombre;
    const char *ruta;
    const char *material_nombre;
    const char *fisica_nombre;
    int p_x, p_y, p_z;
    int e_x, e_y, e_z;
    double r_x, r_y, r_z;
};
std::vector<_player> Player;
bool lgXML::cargarXML()
{
    xmlfile = "../SDK/configsys.xml";

    errorXML = lgDoc.LoadFile(xmlfile);
    XMLElement *sistema = lgDoc.FirstChildElement("sistema");
    string usuario = sistema->Attribute("usuario");

    if (usuario == "nulo")
    {
        string usuario_sistema = getlogin();
        string fs = "/home/" + usuario_sistema + "/LibreGame/media/";

        sistema->SetAttribute("usuario", usuario_sistema.c_str());
        sistema->SetAttribute("ruta", fs.c_str());

        lgDoc.SaveFile(xmlfile);
        nueva_escena_xml = true;
        ruta_xml_file = sistema->Attribute("ruta");
        std::cout << "testing1 : " << ruta_root << std::endl;
    }
    else
    {
        ruta_xml_file = sistema->Attribute("ruta");
        nueva_escena_xml = false;
    }

    return lgDoc.LoadFile(xmlfile);
}
bool lgXML::escenaXML()
{
    errorXML = lgDoc.LoadFile(ruta_xml_file.c_str());
    XMLElement *escena = lgDoc.FirstChildElement("escena");
    XMLElement *entidad = escena->FirstChildElement("entidad");
    XMLElement *id = entidad->FirstChildElement("id");
    int id_i;
    id_i = escena->QueryIntText(&id_i);
    return false;
}
string home = "/home/";
string nombre_usuario = getlogin();
void lgXML::nuevaEscenaXml(bool *p)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    ImGuiWindowFlags window_flags3 = ImGuiWindowFlags_NoCollapse;
    //p = NULL;
    //propiedades de la escena
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 8.0f);
    ImGui::PushStyleColor(ImGuiCol_TitleBgActive, ImVec4(0.2f, 0.8f, 0.4f, 1.0f));
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.8f, 0.8f, 0.8f, 1.0f));
    ImGui::Begin("PROPIEDADES DEL PROYECTO", p, window_flags3);

    //parte grafica del formulario
    ImGui::Columns(2);
    ImGui::LabelText("##1", "Ruta del proyecto");
    ImGui::LabelText("##2", "Nombre de la escena");
    ImGui::LabelText("##3", "Nombre del archivo");
    ImGui::NextColumn();
    ImGui::Text(ruta_xml_file.c_str());
    static char escena_nombre[512] = "escena#1";
    ImGui::InputText("##1", escena_nombre, IM_ARRAYSIZE(escena_nombre));
    static char archivo_nombre[512] = "miXML";
    ImGui::InputText("##2", archivo_nombre, IM_ARRAYSIZE(archivo_nombre));

    if (ImGui::Button("Guardar"))
    {
        lgDoc.LoadFile(xmlfile);
        //parte interna del guardado en xml
        string filex(ruta_xml_file + archivo_nombre + ".xml");
        std::cout << "nombre : " << filex << std::endl;
        XMLElement *sistema = lgDoc.FirstChildElement("sistema");
        sistema->SetAttribute("ruta", filex.c_str());
        lgDoc.SaveFile(xmlfile);
        xmlfile = filex.c_str();
        XMLDocument lgDocN;
        XMLElement *escena = lgDocN.NewElement("escena");
        escena->SetAttribute("nombre", escena_nombre);
        escena->SetAttribute("id", 0);
        escena->SetAttribute("entidades", 0);
        escena->SetAttribute("luces", 0);
        lgDocN.InsertEndChild(escena);
        lgDocN.SaveFile(xmlfile);
        nueva_escena_xml = false;
    }
    ImGui::PopStyleColor(2);
    ImGui::PopStyleVar();
    ImGui::End();
#endif
}
void lgXML::agregarMesh(string nombre, string ruta, Ogre::SceneManager *sm)
{

    if (ruta_xml_file == "nulo")
        cargarXML();

    lgDoc.LoadFile(ruta_xml_file.c_str());
    XMLElement *rootxml = lgDoc.FirstChildElement("escena");
    int entidades = 0;
    rootxml->QueryIntAttribute("entidades", &entidades);

    int id = 0;

    id = entidades + 1;
    XMLElement *nEnt = lgDoc.NewElement("entidad");
    XMLElement *eId = nEnt->InsertNewChildElement("id");
    eId->SetText(id);
    std::stringstream id_t;
    id_t << id;
    std::string nombre_completo = id_t.str() + " : " + nombre;
    nEnt->SetAttribute("nombre", nombre_completo.c_str());
    nEnt->SetAttribute("ruta", ruta.c_str());
    nEnt->InsertEndChild(eId);
    XMLElement *pos_xml = nEnt->InsertNewChildElement("posicion");
    XMLElement *p_x = pos_xml->InsertNewChildElement("x");
    p_x->SetText(0);
    XMLElement *p_y = pos_xml->InsertNewChildElement("y");
    p_y->SetText(0);
    XMLElement *p_z = pos_xml->InsertNewChildElement("z");
    p_z->SetText(0);
    nEnt->InsertEndChild(pos_xml);

    XMLElement *esc_xml = nEnt->InsertNewChildElement("escala");
    XMLElement *e_x = esc_xml->InsertNewChildElement("x");
    e_x->SetText(1);
    XMLElement *e_y = esc_xml->InsertNewChildElement("y");
    e_y->SetText(1);
    XMLElement *e_z = esc_xml->InsertNewChildElement("z");
    e_z->SetText(1);

    XMLElement *rot_xml = nEnt->InsertNewChildElement("rotacion");
    XMLElement *pitch = rot_xml->InsertNewChildElement("x");
    pitch->SetText(0);
    XMLElement *yaw = rot_xml->InsertNewChildElement("y");
    yaw->SetText(0);
    XMLElement *roll = rot_xml->InsertNewChildElement("z");
    roll->SetText(0);
    nEnt->InsertEndChild(rot_xml);

    nEnt->InsertEndChild(esc_xml);
    rootxml->InsertEndChild(nEnt);
    rootxml->SetAttribute("entidades", id);
    lgDoc.SaveFile(ruta_xml_file.c_str());

    //construccion de vector sombras mesh
    Ogre::MeshPtr vectorMesh = Ogre::MeshManager::getSingleton().load(ruta, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
    unsigned short src, dest;
    if (!vectorMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
        vectorMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);

    Ogre::Entity *ent = sm->createEntity(nombre_completo, ruta);
    Ogre::SceneNode *nodo_ent = sm->getRootSceneNode()->createChildSceneNode();
    nodo_ent->attachObject(ent);
}
void lgXML::agregarLuz(string nombre, string tipo, Ogre::SceneManager *sm)
{
    if (escenaXML())
    {
        cout << "no hay archivo" << endl;
    }
    else
    {
        lgDoc.LoadFile(ruta_xml_file.c_str());
        XMLElement *rootxml = lgDoc.FirstChildElement("escena");
        //XMLElement *rootEnt = rootxml->FirstChildElement("entidad");

        int id;
        for (XMLElement *rootEnt = lgDoc.FirstChildElement("escena"); rootEnt != NULL; rootEnt = rootEnt->NextSiblingElement("escena"))
        {
            rootEnt->QueryIntAttribute("id", &id);
            cout << id << endl;
        }

        id = id + 1;
        XMLElement *nEnt = lgDoc.NewElement("luz");
        XMLElement *eId = nEnt->InsertNewChildElement("id");
        eId->SetText(id);
        nEnt->SetAttribute("nombre", nombre.c_str());
        nEnt->SetAttribute("tipo", tipo.c_str());
        nEnt->InsertEndChild(eId);

        XMLElement *pos_xml = nEnt->InsertNewChildElement("posicion");
        XMLElement *p_x = pos_xml->InsertNewChildElement("x");
        p_x->SetText(0);
        XMLElement *p_y = pos_xml->InsertNewChildElement("y");
        p_y->SetText(0);
        XMLElement *p_z = pos_xml->InsertNewChildElement("z");
        p_z->SetText(0);
        nEnt->InsertEndChild(pos_xml);

        XMLElement *rot_xml = nEnt->InsertNewChildElement("rotacion");
        XMLElement *pitch = rot_xml->InsertNewChildElement("x");
        pitch->SetText(0);
        XMLElement *yaw = rot_xml->InsertNewChildElement("y");
        yaw->SetText(0);
        XMLElement *roll = rot_xml->InsertNewChildElement("z");
        roll->SetText(0);
        nEnt->InsertEndChild(rot_xml);

        XMLElement *color_difuso_xml = nEnt->InsertNewChildElement("difuso");
        XMLElement *d_r = color_difuso_xml->InsertNewChildElement("r");
        d_r->SetText(255);
        XMLElement *d_g = color_difuso_xml->InsertNewChildElement("g");
        d_g->SetText(255);
        XMLElement *d_b = color_difuso_xml->InsertNewChildElement("b");
        d_b->SetText(255);
        XMLElement *d_a = color_difuso_xml->InsertNewChildElement("a");
        d_a->SetText(255);
        nEnt->InsertEndChild(color_difuso_xml);

        XMLElement *color_especular_xml = nEnt->InsertNewChildElement("especular");
        XMLElement *e_r = color_especular_xml->InsertNewChildElement("r");
        e_r->SetText(255);
        XMLElement *e_g = color_especular_xml->InsertNewChildElement("g");
        e_g->SetText(255);
        XMLElement *e_b = color_especular_xml->InsertNewChildElement("b");
        e_b->SetText(255);
        XMLElement *e_a = color_especular_xml->InsertNewChildElement("a");
        e_a->SetText(255);
        nEnt->InsertEndChild(color_especular_xml);

        rootxml->InsertEndChild(nEnt);
        rootxml->SetAttribute("id", id);
        lgDoc.SaveFile(ruta_xml_file.c_str());

        Ogre::Light *l = sm->createLight(nombre);
        Ogre::SceneNode *nodo_ent = sm->getRootSceneNode()->createChildSceneNode();
        nodo_ent->attachObject(l);
        nodo_ent->showBoundingBox(true);
    }
}
void lgXML::guardarXMLE(Ogre::Entity *ent)
{
    lgDoc.LoadFile(ruta_xml_file.c_str());
    XMLElement *rootxml = lgDoc.FirstChildElement("escena");

    string nombre_mesh = ent->getName();
    string nombre_xml;

    int p_x = ent->getParentSceneNode()->getPosition().x;
    int p_y = ent->getParentSceneNode()->getPosition().y;
    int p_z = ent->getParentSceneNode()->getPosition().z;

    float e_x = ent->getParentSceneNode()->getScale().x;
    float e_y = ent->getParentSceneNode()->getScale().y;
    float e_z = ent->getParentSceneNode()->getScale().z;

    Ogre::Quaternion s = ent->getParentNode()->getOrientation();
    float pitch = s.getPitch().valueDegrees();
    float yaw = s.getYaw().valueDegrees();
    float roll = s.getRoll().valueDegrees();

    int id;
    bool salir = false;
    while (salir == false)
    {
        for (XMLElement *rootEnt = rootxml->FirstChildElement("entidad"); rootEnt != NULL; rootEnt = rootEnt->NextSiblingElement("entidad"))
        {
            nombre_xml = rootEnt->Attribute("nombre");

            if (nombre_mesh == nombre_xml)
            {
                XMLElement *posicion_xml = rootEnt->FirstChildElement("posicion");
                XMLElement *x_xml = posicion_xml->FirstChildElement("x");
                XMLElement *y_xml = posicion_xml->FirstChildElement("y");
                XMLElement *z_xml = posicion_xml->FirstChildElement("z");
                x_xml->SetText(p_x);
                y_xml->SetText(p_y);
                z_xml->SetText(p_z);

                XMLElement *escala_xml = rootEnt->FirstChildElement("escala");
                XMLElement *e_x_xml = escala_xml->FirstChildElement("x");
                XMLElement *e_y_xml = escala_xml->FirstChildElement("y");
                XMLElement *e_z_xml = escala_xml->FirstChildElement("z");
                e_x_xml->SetText(e_x);
                e_y_xml->SetText(e_y);
                e_z_xml->SetText(e_z);

                XMLElement *rotacion_xml = rootEnt->FirstChildElement("rotacion");
                XMLElement *pitch_xml = rotacion_xml->FirstChildElement("x");
                XMLElement *yaw_xml = rotacion_xml->FirstChildElement("y");
                XMLElement *roll_xml = rotacion_xml->FirstChildElement("z");
                pitch_xml->SetText(pitch);
                yaw_xml->SetText(yaw);
                roll_xml->SetText(roll);

                lgDoc.SaveFile(ruta_xml_file.c_str());
                salir = true;
            }
        }
    }
}
void lgXML::guardarXMLL(Ogre::Light *l)
{
    lgDoc.LoadFile(ruta_xml_file.c_str());
    XMLElement *rootxml = lgDoc.FirstChildElement("escena");

    string nombre_luz = l->getName();

    int p_x = l->getParentSceneNode()->getPosition().x;
    int p_y = l->getParentSceneNode()->getPosition().y;
    int p_z = l->getParentSceneNode()->getPosition().z;

    Ogre::Quaternion s = l->getParentNode()->getOrientation();
    float pitch = s.getPitch().valueDegrees();
    float yaw = s.getYaw().valueDegrees();
    float roll = s.getRoll().valueDegrees();

    float color_r = l->getDiffuseColour().r;
    float color_g = l->getDiffuseColour().g;
    float color_b = l->getDiffuseColour().b;
    float color_a = l->getDiffuseColour().a;

    float color_esp_r = l->getSpecularColour().r;
    float color_esp_g = l->getSpecularColour().g;
    float color_esp_b = l->getSpecularColour().b;
    float color_esp_a = l->getSpecularColour().a;

    string tipo;

    switch (l->getType())
    {
    case 0:
        tipo = "Punto";
        break;
    case 1:
        tipo = "Direccional";
        break;
    case 2:
        tipo = "Foco";
        break;
    }

    int id;
    bool salir = false;
    while (salir == false)
    {
        for (XMLElement *rootEnt = rootxml->FirstChildElement("luz"); rootEnt != NULL; rootEnt = rootEnt->NextSiblingElement("luz"))
        {
            string nombre_xml = rootEnt->Attribute("nombre");

            if (nombre_luz == nombre_xml)
            {
                XMLElement *posicion_xml = rootEnt->FirstChildElement("posicion");
                XMLElement *x_xml = posicion_xml->FirstChildElement("x");
                XMLElement *y_xml = posicion_xml->FirstChildElement("y");
                XMLElement *z_xml = posicion_xml->FirstChildElement("z");
                x_xml->SetText(p_x);
                y_xml->SetText(p_y);
                z_xml->SetText(p_z);

                XMLElement *rotacion_xml = rootEnt->FirstChildElement("rotacion");
                XMLElement *pitch_xml = rotacion_xml->FirstChildElement("x");
                XMLElement *yaw_xml = rotacion_xml->FirstChildElement("y");
                XMLElement *roll_xml = rotacion_xml->FirstChildElement("z");
                pitch_xml->SetText(pitch);
                yaw_xml->SetText(yaw);
                roll_xml->SetText(roll);

                XMLElement *color_difuso_xml = rootEnt->FirstChildElement("difuso");
                XMLElement *d_r = color_difuso_xml->FirstChildElement("r");
                XMLElement *d_g = color_difuso_xml->FirstChildElement("g");
                XMLElement *d_b = color_difuso_xml->FirstChildElement("b");
                XMLElement *d_a = color_difuso_xml->FirstChildElement("a");
                d_r->SetText(color_r);
                d_g->SetText(color_g);
                d_b->SetText(color_b);
                d_a->SetText(color_a);

                XMLElement *color_especular_xml = rootEnt->FirstChildElement("especular");
                XMLElement *e_r = color_especular_xml->FirstChildElement("r");
                XMLElement *e_g = color_especular_xml->FirstChildElement("g");
                XMLElement *e_b = color_especular_xml->FirstChildElement("b");
                XMLElement *e_a = color_especular_xml->FirstChildElement("a");
                e_r->SetText(color_esp_r);
                e_g->SetText(color_esp_g);
                e_b->SetText(color_esp_b);
                e_a->SetText(color_esp_a);

                rootEnt->SetAttribute("tipo", tipo.c_str());

                lgDoc.SaveFile(ruta_xml_file.c_str());
                salir = true;
            }
        }
    }
}
