#include "LibreGame.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/configuration.h>
#include <android/asset_manager.h>
#include <android/input.h>
#include <android/log.h>
#endif

const char *filexml = "mnt/sdcard/superm.xml";

void LibreGame::cargarEscena()
{
  XMLDocument doc_proyecto;
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
  tinyxml2::XMLDocument doc;
  doc.LoadFile(xmlfile);
  XMLElement *rootxml = doc.FirstChildElement("sistema");
  ruta_xml_file = rootxml->Attribute("ruta");
  doc_proyecto.LoadFile(ruta_xml_file.c_str());
#else

  AAssetDir *assetDir = AAssetManager_openDir(lgManagerAndroid, "");
  const char *filename;
  while ((filename = AAssetDir_getNextFileName(assetDir)) != NULL)
  {
    //__android_log_print(ANDROID_LOG_DEBUG, "Archivo ->", "%s", filename);
  }
  if (doc_proyecto.LoadFile(filexml) != 0)
  {
    __android_log_print(ANDROID_LOG_DEBUG, "Funcioando XML", "%s", filexml);
  }

  AAssetDir_close(assetDir);
#endif
  XMLElement *xml_proyecto = doc_proyecto.FirstChildElement("escena");
  int entidades = 1;
  int luces = 1;
  xml_proyecto->QueryIntAttribute("entidades", &entidades); 
  total_body = entidades;

  xml_proyecto->QueryIntAttribute("luces", &luces);
  int no = 0;

  if (entidades > 0)
  {
    for (tinyxml2::XMLElement *e = xml_proyecto->FirstChildElement("entidad"); e != NULL; e = e->NextSiblingElement("entidad"))
    {
      std::string mesh = e->Attribute("ruta");
      std::string nombre = e->Attribute("nombre");
      std::string tipo = e->Attribute("tipo");
      int t;
      if(tipo == "player"){
        t = 0;
      }else if(tipo == "estatico"){
        t = 1;
      }
      int x, y, z;
      float e_x, e_y, e_z;
      float r_p, r_y, r_r;
      float f_x, f_y, f_z;

      bool sombre_act = false;

      tinyxml2::XMLElement *pos_xml = e->FirstChildElement("posicion");
      XMLElement *p_x = pos_xml->FirstChildElement("x");
      XMLElement *p_y = pos_xml->FirstChildElement("y");
      XMLElement *p_z = pos_xml->FirstChildElement("z");
      p_x->QueryIntText(&x);
      p_y->QueryIntText(&y);
      p_z->QueryIntText(&z);

      tinyxml2::XMLElement *esc_xml = e->FirstChildElement("escala");
      XMLElement *e_x_xml = esc_xml->FirstChildElement("x");
      XMLElement *e_y_xml = esc_xml->FirstChildElement("y");
      XMLElement *e_z_xml = esc_xml->FirstChildElement("z");
      e_x_xml->QueryFloatText(&e_x);
      e_y_xml->QueryFloatText(&e_y);
      e_z_xml->QueryFloatText(&e_z);

      tinyxml2::XMLElement *rotacion = e->FirstChildElement("rotacion");
      XMLElement *pitch = rotacion->FirstChildElement("x");
      XMLElement *yaw = rotacion->FirstChildElement("y");
      XMLElement *roll = rotacion->FirstChildElement("z");
      pitch->QueryFloatText(&r_p);
      yaw->QueryFloatText(&r_y);
      roll->QueryFloatText(&r_r);

      tinyxml2::XMLElement *forma_xml = e->FirstChildElement("forma");
      XMLElement *f_x_xml = forma_xml->FirstChildElement("x");
      XMLElement *f_y_xml = forma_xml->FirstChildElement("y");
      XMLElement *f_z_xml = forma_xml->FirstChildElement("z");
      f_x_xml->QueryFloatText(&f_x);
      f_y_xml->QueryFloatText(&f_y);
      f_z_xml->QueryFloatText(&f_z);

      if (nombre != "")
      {
        lgEntidadv.push_back(entidad());
        lgEntidadv[no].ent = new lgEntidad2(nombre, mesh);
        lgEntidadv[no].ent->setPosicion(x, y, z);
        lgEntidadv[no].ent->setDimensiones(e_x, e_y, e_z);
        lgEntidadv[no].ent->setDimensionesForma(f_x, f_y, f_z);
        lgEntidadv[no].ent->setRotacion(r_p, r_y, r_r);
        lgEntidadv[no].ent->mostrar(lgManager, m_dynamicsWorld, t);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
          __android_log_print(ANDROID_LOG_DEBUG, "Mesh ", "%s", nombre.c_str());
#endif
      }
      no++;
    }
    int nol = 0;
    if (luces > 0)
    {
      for (tinyxml2::XMLElement *e = xml_proyecto->FirstChildElement("luz"); e != NULL; e = e->NextSiblingElement("luz"))
      {
        std::string nombre = e->Attribute("nombre");
        std::string tipo = e->Attribute("tipo");

        int x, y, z;
        float r_p, r_y, r_r;
        static ImVec4 color_difuso = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);
        static ImVec4 color_especular = ImVec4(114.0f / 255.0f, 144.0f / 255.0f, 154.0f / 255.0f, 200.0f / 255.0f);

        tinyxml2::XMLElement *pos_xml = e->FirstChildElement("posicion");
        tinyxml2::XMLElement *difuso = e->FirstChildElement("difuso");
        tinyxml2::XMLElement *especular = e->FirstChildElement("especular");
        tinyxml2::XMLElement *rotacion = e->FirstChildElement("rotacion");

        XMLElement *p_x = pos_xml->FirstChildElement("x");
        XMLElement *p_y = pos_xml->FirstChildElement("y");
        XMLElement *p_z = pos_xml->FirstChildElement("z");
        p_x->QueryIntText(&x);
        p_y->QueryIntText(&y);
        p_z->QueryIntText(&z);

        XMLElement *pitch = rotacion->FirstChildElement("x");
        XMLElement *yaw = rotacion->FirstChildElement("y");
        XMLElement *roll = rotacion->FirstChildElement("z");
        pitch->QueryFloatText(&r_p);
        yaw->QueryFloatText(&r_y);
        roll->QueryFloatText(&r_r);

        XMLElement *dis_r = difuso->FirstChildElement("r");
        XMLElement *dis_g = difuso->FirstChildElement("g");
        XMLElement *dis_b = difuso->FirstChildElement("b");
        XMLElement *dis_a = difuso->FirstChildElement("a");
        dis_r->QueryFloatText(&color_difuso.x);
        dis_g->QueryFloatText(&color_difuso.y);
        dis_b->QueryFloatText(&color_difuso.z);
        dis_a->QueryFloatText(&color_difuso.w);

        XMLElement *esp_r = especular->FirstChildElement("r");
        XMLElement *esp_g = especular->FirstChildElement("g");
        XMLElement *esp_b = especular->FirstChildElement("b");
        XMLElement *esp_a = especular->FirstChildElement("a");
        esp_r->QueryFloatText(&color_especular.x);
        esp_g->QueryFloatText(&color_especular.y);
        esp_b->QueryFloatText(&color_especular.z);
        esp_a->QueryFloatText(&color_especular.w);

        if (nombre != "")
        {
          lgLucesv.push_back(luz());
          lgLucesv[nol].l = new lgLuces(nombre, tipo);
          lgLucesv[nol].l->setPosicion(x, y, z);
          lgLucesv[nol].l->setColorDifuso(color_difuso);
          lgLucesv[nol].l->setColorEspecular(color_especular);
          lgLucesv[nol].l->setRotacion(r_p, r_y, r_r);
          lgLucesv[nol].l->mostrar(lgManager);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
          __android_log_print(ANDROID_LOG_DEBUG, "Luces ", "%s", nombre.c_str());
#endif
        }

        nol++;
      }
    }
  }
}