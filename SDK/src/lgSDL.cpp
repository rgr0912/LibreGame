#include "lgSDL.h"

lgSDL::lgSDL()
{
}
void lgSDL::rw(Ogre::Camera* cam){
    cam->setNearClipDistance(0.5);
    cam->setFarClipDistance(500);
cam->setAspectRatio(Ogre::Real(getRenderWindow()->getWidth()/getRenderWindow()->getHeight()));
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
void lgSDL::sdlEventos(Ogre::Root *root, Ogre::RenderWindow *win, Ogre::Camera *cam)
{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    for (WindowList::iterator it = ventana_lista.begin(); it != ventana_lista.end(); ++it)
    {
        Ogre::RenderWindow *win = it->ventana_ogre;
        int w = ANativeWindow_getWidth(it->ventana_nativa);
        int h = ANativeWindow_getHeight(it->ventana_nativa);
        win->resize(w, h);
        cam->setAspectRatio(Ogre::Real(w) / Ogre::Real(h));
        windowResized(win);
    }
#else
    ImGui_ImplSDL2_InitForVulkan(ventana_lista[0].ventana_nativa);
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

                if (event.window.windowID != SDL_GetWindowID(it->ventana_nativa))
                    continue;

                Ogre::RenderWindow *win = it->ventana_ogre;
                win->resize(event.window.data1, event.window.data2);
                win->windowMovedOrResized();
                cam->setAspectRatio(Ogre::Real(event.window.data1) / Ogre::Real(event.window.data2));
                windowResized(win);
            }
            break; /**
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
            sdl_seleccion(win, cam, seleccion_objecto, mRay, event.motion.x, event.motion.y, event.motion.xrel);**/
            //}
            //break;
        }
    }
#endif
}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
using namespace OgreBites;
void lgSDL::sdlEventosAndroid(AInputEvent *event, int wheel)
{
    Event evt = {0};

    static TouchFingerEvent lastTouch = {0};
    ImGuiIO &io = ImGui::GetIO();
    io.MouseDrawCursor = false;

    if (wheel)
    {
        evt.type = MOUSEWHEEL;
        evt.wheel.y = wheel;
        //_fireInputEvent(evt, 0);
        lastTouch.fingerId = -1; // prevent move-jump after pinch is over
        return;
    }

    if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
    {
        int32_t action = AMOTION_EVENT_ACTION_MASK & AMotionEvent_getAction(event);

        switch (action)
        {
        case AMOTION_EVENT_ACTION_DOWN:
            evt.type = FINGERDOWN;
            Ogre::LogManager::getSingletonPtr()->logMessage("*** pulsando sobre la pantalla ***");
            io.MouseDown[wheel] = true;
            break;
        case AMOTION_EVENT_ACTION_UP:
            evt.type = FINGERUP;
            Ogre::LogManager::getSingletonPtr()->logMessage("*** sin pulsando sobre la pantalla ***");
            io.MouseDown[wheel] = false;
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            evt.type = FINGERMOTION;
            Ogre::LogManager::getSingletonPtr()->logMessage("*** deslisando sobre la pantalla ***");
            break;
        default:
            return;
        }

        Ogre::RenderWindow *win = getRenderWindow();
        windowResized(win);

        evt.tfinger.fingerId = AMotionEvent_getPointerId(event, 0);
        evt.tfinger.x = AMotionEvent_getRawX(event, 0) / win->getWidth() * win->getViewPointToPixelScale();
        evt.tfinger.y = AMotionEvent_getRawY(event, 0) / win->getHeight() * win->getViewPointToPixelScale();

        if (evt.type == FINGERMOTION)
        {
            if (evt.tfinger.fingerId != lastTouch.fingerId)
                return; // wrong finger

            evt.tfinger.dx = evt.tfinger.x - lastTouch.x;
            evt.tfinger.dy = evt.tfinger.y - lastTouch.y;

            std::string mv = std::to_string(evt.tfinger.dx);
            Ogre::LogManager::getSingletonPtr()->logMessage(mv);
            
        }

        lastTouch = evt.tfinger;
    }
    else
    {
        if (AKeyEvent_getKeyCode(event) != AKEYCODE_BACK)
            return;

        evt.type = AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN ? KEYDOWN : KEYUP;
        evt.key.keysym.sym = SDLK_ESCAPE;
    }

    _fireInputEvent(evt, 0);
}
void lgSDL::_fireInputEvent(const Event &event, uint32_t windowID) const
{
    Event scaled = event;
    if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE && event.type == MOUSEMOTION)
    {
        // assumes all windows have the same scale
        float viewScale = getRenderWindow()->getViewPointToPixelScale();
        scaled.motion.x *= viewScale;
        scaled.motion.y *= viewScale;
    }

    for (InputListenerList::iterator it = mInputListeners.begin();
         it != mInputListeners.end(); ++it)
    {
        // gamepad events are not window specific
        if (it->first != windowID && event.type <= TEXTINPUT)
            continue;

        InputListener &l = *it->second;

        switch (event.type)
        {
        case KEYDOWN:
            l.keyPressed(event.key);
            break;
        case KEYUP:
            l.keyReleased(event.key);
            break;
        case MOUSEBUTTONDOWN:
            l.mousePressed(event.button);
            break;
        case MOUSEBUTTONUP:
            l.mouseReleased(event.button);
            break;
        case MOUSEWHEEL:
            l.mouseWheelRolled(event.wheel);
            break;
        case MOUSEMOTION:
            l.mouseMoved(scaled.motion);
            break;
        case FINGERDOWN:
            // for finger down we have to move the pointer first
            l.touchMoved(event.tfinger);
            l.touchPressed(event.tfinger);
            break;
        case FINGERUP:
            l.touchReleased(event.tfinger);
            break;
        case FINGERMOTION:
            l.touchMoved(event.tfinger);
            break;
        case TEXTINPUT:
            l.textInput(event.text);
            break;
        case CONTROLLERAXISMOTION:
            l.axisMoved(event.axis);
            break;
        case CONTROLLERBUTTONDOWN:
            l.buttonPressed(event.cbutton);
            break;
        case CONTROLLERBUTTONUP:
            l.buttonReleased(event.cbutton);
            break;
        }
    }
}
#endif
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