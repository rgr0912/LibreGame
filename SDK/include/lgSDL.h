#include "Ogre.h"
#include "OgreInput.h"
#include "imgui_impl_sdl.h"
#include <iostream>
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/configuration.h>
#include <android/asset_manager.h>
#include <android/native_window.h>
#include <android/input.h>
typedef ANativeWindow NativeWindowType;
using namespace OgreBites;
#else
//#include "imgui_impl_sdl.h"
extern "C" struct SDL_Window;
typedef SDL_Window NativeWindowType;
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#endif

struct ventanaNativapar
{
    Ogre::RenderWindow *ventana_ogre;
    NativeWindowType *ventana_nativa;
};
/**
 * Implementacion de la libreria de SDL para la entrada de teclado y raton para el
 * game engine, no tiene otro uso mas que el mensionado hasta el momento
 **/
class lgSDL
{
public:
    lgSDL();
    //1 llamado de la ventana
    ventanaNativapar tipo_ventana = {NULL, NULL};
    virtual ventanaNativapar crearVentana(const Ogre::String &name, Ogre::Root *root,uint32_t w = 0, uint32_t h = 0, Ogre::NameValuePairList miscParams = Ogre::NameValuePairList());
    Ogre::RenderWindow *getRenderWindow() const
    {
        return ventana_lista.empty() ? NULL : ventana_lista[0].ventana_ogre;
    }
    void sdlEventos(Ogre::Root *root, Ogre::RenderWindow *win, Ogre::Camera *cam);
    Ogre::String objeto = "no";
    typedef std::vector<ventanaNativapar> WindowList;
    WindowList ventana_lista;
    Ogre::RaySceneQueryResult::iterator itr;
    Ogre::SceneNode *_selectnode;
    virtual void windowResized(Ogre::RenderWindow *rw) {}
    void rw(Ogre::Camera* cam);
    bool sdl_seleccion(Ogre::RenderWindow *win, Ogre::Camera *cam, bool m, Ogre::RaySceneQuery *mRay, int x, int y, int xrel);
    void mover_nodo(Ogre::String tipo, Ogre::SceneNode *node, int d);
    void mover_camara(Ogre::String tipo, Ogre::Camera *cam, int d);
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    virtual void sdlEventosAndroid(AInputEvent *event, int wheel = 0);
    void _fireInputEvent(const Event &event, uint32_t windowID) const;
    typedef std::set<std::pair<uint32_t, InputListener *>> InputListenerList;
    InputListenerList mInputListeners;
    //ventanaNativapar crearVentanaAndroid(const Ogre::String &name, Ogre::Root* root, uint32_t w = 0, uint32_t h = 0, Ogre::NameValuePairList miscParams = Ogre::NameValuePairList());
    AAssetManager *lgManagerAndroid;
    AConfiguration *lgConfigAndroid;
#endif
};
