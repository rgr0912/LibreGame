#include "lgSDL.h"

lgSDL::lgSDL()
{
}

//codigo extraido de ogre para implementar sdl
ventanaNativapar lgSDL::crearVentana(const Ogre::String &name, Ogre::Root *root, Ogre::uint32 w, Ogre::uint32 h, Ogre::NameValuePairList miscParams)
{

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    // Make sure Trays are not tiny -  we cannot easily scale the UI, therefore just reduce resolution
    float contentScaling = AConfiguration_getDensity(lgConfigAndroid) / float(ACONFIGURATION_DENSITY_HIGH);
    if (contentScaling > 1.0)
    {
        miscParams["contentScalingFactor"] = std::to_string(contentScaling);
        miscParams["FSAA"] = "2";
    }
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(ventana_lista[0].ventana_nativa));
    miscParams["androidConfig"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(lgConfigAndroid));
    miscParams["preserveContext"] = "true"; //Optionally preserve the gl context, prevents reloading all resources, this is false by default

    ventana_lista[0].ventana_ogre = Ogre::Root::getSingleton().createRenderWindow(name, 0, 0, false, &miscParams);

    return tipo_ventana;
#else
    if (!SDL_WasInit(SDL_INIT_VIDEO))
    {
        SDL_InitSubSystem(SDL_INIT_VIDEO);
    }

    auto p = root->getRenderSystem()->getRenderWindowDescription();
    miscParams.insert(p.miscParams.begin(), p.miscParams.end());
    p.miscParams = miscParams;
    p.name = name;

    if (w > 0 && h > 0)
    {
        p.width = w;
        p.height = h;
    }

    int flags = p.useFullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE;
    int d = Ogre::StringConverter::parseInt(miscParams["monitorIndex"], 1) - 1;
    tipo_ventana.ventana_nativa = SDL_CreateWindow(p.name.c_str(), SDL_WINDOWPOS_UNDEFINED_DISPLAY(d), SDL_WINDOWPOS_UNDEFINED_DISPLAY(d), p.width, p.height, flags);
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(tipo_ventana.ventana_nativa, &wmInfo);
    p.miscParams["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.x11.window));

    if (!ventana_lista.empty())
    {
        // additional windows should reuse the context
        p.miscParams["currentGLContext"] = "true";
    }

    tipo_ventana.ventana_ogre = root->createRenderWindow(p);
    ventana_lista.push_back(tipo_ventana);
    return tipo_ventana;
#endif
}

bool seleccion_objecto = false;
bool mx = false;
bool my = false;
bool mz = false;
bool rc = false;
bool ic = false;
bool me = false;
bool nx = false;
bool ny = false;
bool nz = false;
void lgSDL::sdlEventos(Ogre::Root *root, Ogre::RenderWindow *win, Ogre::Camera *cam, Ogre::RaySceneQuery *mRay)
{
    /** ImGui_ImplSDL2_InitForVulkan(tipo_ventana.ventana_sdl);
    if (ventana_lista.empty())
    {
        // SDL events no es inicializado
        return;
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        //implementando eventos imgui con sdl
        ImGui_ImplSDL2_ProcessEvent(&event);

        switch (event.type)
        {
        case SDL_QUIT:
            root->queueEndRendering();
            break;
        case SDL_WINDOWEVENT:
            if (event.window.event != SDL_WINDOWEVENT_RESIZED)
                continue;

            for (WindowList::iterator it = ventana_lista.begin(); it != ventana_lista.end(); ++it)
            {
                if (event.window.windowID != SDL_GetWindowID(it->ventana_sdl))
                    continue;

                Ogre::RenderWindow *win = it->venta_ogre;
                win->windowMovedOrResized();
                windowResized(win);
            }
            break;
        case SDL_KEYDOWN:
        {
            if (event.key.keysym.sym == SDLK_x)
            {
                mx = true;
            }
            if (event.key.keysym.sym == SDLK_y)
            {
                my = true;
            }
            if (event.key.keysym.sym == SDLK_z)
            {
                mz = true;
            }
            if (event.key.keysym.sym == SDLK_r)
            {
                rc = true;
            }
        }
        break;
        case SDL_KEYUP:
            break;
        case SDL_MOUSEMOTION:
        {
            if (mx == true)
                mover_camara("mover_en_x", cam, event.motion.xrel);
            if (my == true)
                mover_camara("mover_en_y", cam, event.motion.yrel);
            if (mz == true)
                mover_camara("mover_en_z", cam, event.motion.xrel);
            if (rc == true)
                mover_camara("rotar_camara", cam, event.motion.xrel);
        }
        break;
        case SDL_MOUSEBUTTONDOWN:
        {
            if (mx == true)
            {
                mx = false;
            }
            if (my == true)
            {
                my = false;
            }
            if (mz == true)
            {
                mz = false;
            }
            if (rc == true)
            {
                rc = false;
            }
            sdl_seleccion(win, cam, seleccion_objecto, mRay, event.motion.x, event.motion.y, event.motion.xrel);
        }
        break;
        }
    }**/
}
bool lgSDL::sdl_seleccion(Ogre::RenderWindow *win, Ogre::Camera *cam, bool m, Ogre::RaySceneQuery *mRay, int x, int y, int xrel)
{
    /**Ogre::Vector3 vt(0, 0, 0);
    Ogre::Real tVelocidad = 0.05;
    Ogre::Real r = 0;
    Ogre::Real deltaT = (0.016 + 0.017) / 2;
    int fps = 1.0 / deltaT;

    Ogre::Vector3 transVectorX = Ogre::Vector3::ZERO;

    Ogre::Real screenWidth = win->getWidth();
    Ogre::Real screenHeight = win->getHeight();

    Ogre::Real offsetX = x / screenWidth;
    Ogre::Real offsetY = y / screenHeight;

    Ogre::Ray ray = cam->getCameraToViewportRay(offsetX, offsetY);
    mRay->setRay(ray);
    Ogre::RaySceneQueryResult &result = mRay->execute();
    for (itr = result.begin(); itr != result.end(); itr++)
    {
        if (itr->movable && itr->movable->getMovableType() != "ManualObject")
        {
            _selectnode = itr->movable->getParentSceneNode();
            if (!_selectnode->getShowBoundingBox())
            {
                _selectnode->showBoundingBox(true);
                seleccion_objecto = true;
                objeto = itr->movable->getName();
                const char *c = objeto.c_str();
            }
            else
            {
                _selectnode->showBoundingBox(false);
                seleccion_objecto = false;
                objeto = "no";
            }
        }
    }
    return true;**/
}
void lgSDL::mover_camara(Ogre::String tipo, Ogre::Camera *cam, int d)
{
    /** Ogre::Vector3 vt(0, 0, 0);
    Ogre::Real tVelocidad = 0.05;
    Ogre::Real r = 0;
    Ogre::Real deltaT = (0.016 + 0.017) / 2;
    int fps = 1.0 / deltaT;

    float rotx = d * deltaT * -.08;
    float roty = d * deltaT * -.08;

    Ogre::Vector3 transVectorX = Ogre::Vector3::ZERO;

    Ogre::Vector3 transVector2 = Ogre::Vector3::ZERO;
    if (tipo == "rotar_camara")
    {
        if (d != 0)
        {
            cam->getParentNode()->yaw(Ogre::Radian(rotx * 10));
        }
    }
    if (tipo == "inclinar_camara")
    {
        if (d != 0)
        {
            cam->getParentNode()->pitch(Ogre::Radian(rotx * 10));
        }
    }
    if (tipo == "mover_en_x")
    {
        if (d != 0)
        {
            transVectorX.x = -d * 50;
        }
        cam->getParentSceneNode()->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }
    else if (tipo == "mover_en_y")
    {
        if (d != 0)
        {
            transVectorX.y = -d * 50;
        }
        cam->getParentSceneNode()->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }
    else if (tipo == "mover_en_z")
    {
        if (d != 0)
        {
            transVectorX.z = -d * 50;
        }
        cam->getParentSceneNode()->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }**/
}
void lgSDL::mover_nodo(Ogre::String tipo, Ogre::SceneNode *node, int d)
{
    /**Ogre::Vector3 vt(0, 0, 0);
    Ogre::Real tVelocidad = 0.05;
    Ogre::Real r = 0;
    Ogre::Real deltaT = (0.016 + 0.017) / 2;
    int fps = 1.0 / deltaT;

    float roty = d * deltaT * -.08;

    Ogre::Vector3 transVectorX = Ogre::Vector3::ZERO;

    if (tipo == "mover_en_x")
    {
        if (d != 0)
        {
            transVectorX.x = d * 50;
        }
        node->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }
    else if (tipo == "mover_en_y")
    {
        if (d != 0)
        {
            transVectorX.y = -d * 50;
        }
        node->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }
    else if (tipo == "mover_en_z")
    {
        if (d != 0)
        {
            transVectorX.z = d * 50;
        }
        node->translate(transVectorX * deltaT, Ogre::Node::TS_LOCAL);
    }**/
}