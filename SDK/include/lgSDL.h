/*
LibreGame es un proyecto que intenta crear un game engine a partir de librerias libres.
*/

#include "Ogre.h"
#include "OgreTrays.h"
#include "OgreInput.h"
#include "OgreImGuiOverlay.h"
#include <OgreImGuiInputListener.h>
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
extern "C" struct SDL_Window;
typedef SDL_Window NativeWindowType;
#include "SDL2/SDL.h"
#include "SDL2/SDL_syswm.h"
#include "SDLInputMapping.h"
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
class lgSDL : public OgreBites::InputListener, public OgreBites::TrayListener
{
public:
    lgSDL();
    typedef std::set<std::pair<uint32_t, InputListener *>> InputListenerList;
    InputListenerList mInputListeners;

    std::unique_ptr<OgreBites::ImGuiInputListener> sdlImguiListener;
    OgreBites::InputListenerChain sdlListenerChain;

//protected:
/// sdlEvento proporciona la entrada del teclado y raton
    void sdlEventos(Ogre::Root *root, Ogre::RenderWindow *win, Ogre::Camera *cam, Ogre::RaySceneQuery *mRay);
///sdl_seleccion crea un una seleccion de un objeto en pantalla.
    bool sdl_seleccion(Ogre::RenderWindow *win, Ogre::Camera *cam, bool m, Ogre::RaySceneQuery *mRay, int x, int y, int xrel);
    void windowResized(Ogre::RenderWindow *rw) {}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    virtual void sdlEventosAndroid(AInputEvent *event, int wheel = 0);
    void _fireInputEvent(const Event &event, uint32_t windowID) const;
    AAssetManager *lgManagerAndroid;
    AConfiguration *lgConfigAndroid;
    int w_a;
    int h_a;
#else

    void _fireInputEvent(const OgreBites::Event &event, uint32_t windowID) const;

#endif
    void addInputListener(NativeWindowType *win, InputListener *lis);

    void addInputListener(InputListener *lis)
    {
        OgreAssert(!ventana_lista.empty(), "cree una ventana primero");
        addInputListener(ventana_lista[0].ventana_nativa, lis);
    }
    Ogre::RenderWindow *ogreWindow;
    ventanaNativapar tipo_ventana = {NULL, NULL};
    virtual ventanaNativapar crearVentana(const Ogre::String &nombre, Ogre::Root *root, uint32_t w = 0, uint32_t h = 0, Ogre::NameValuePairList miscParams = Ogre::NameValuePairList());
    Ogre::RenderWindow *getRenderWindow() const
    {
        return ventana_lista.empty() ? NULL : ventana_lista[0].ventana_ogre;
    }
///sdlObjecto indica el nombre del objecto selecionado en tiempo real.
    Ogre::String sdlObjeto = "no";
    typedef std::vector<ventanaNativapar> WindowList;
    WindowList ventana_lista;
    Ogre::RaySceneQueryResult::iterator itr;
    Ogre::SceneNode *sdlNodo;
};
