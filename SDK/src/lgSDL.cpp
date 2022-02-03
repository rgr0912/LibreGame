#include "lgSDL.h"

lgSDL::lgSDL()
{
}
//codigo extraido de ogre para implementar sdl
ventanaNativapar lgSDL::crearVentana(const Ogre::String &name, Ogre::Root *root, Ogre::uint32 w, Ogre::uint32 h, Ogre::NameValuePairList miscParams)
{

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    // Make sure Trays are not tiny -  we cannot easily scale the UI, therefore just reduce resolution
    miscParams["externalWindowHandle"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(ventana_lista[0].ventana_nativa));
    miscParams["androidConfig"] = Ogre::StringConverter::toString(reinterpret_cast<size_t>(lgConfigAndroid));
    miscParams["preserveContext"] = "true"; //Optionally preserve the gl context, prevents reloading all resources, this is false by default

    auto p = root->getRenderSystem()->getRenderWindowDescription();
    miscParams.insert(p.miscParams.begin(), p.miscParams.end());
    p.miscParams = miscParams;
    p.name = name;

    ventana_lista[0].ventana_ogre = root->createRenderWindow(p);
    return ventana_lista[0];
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
#if OGRE_PLATFORM != OGRE_PLATFORM_EMSCRIPTEN
    SDL_SysWMinfo wmInfo;
    SDL_VERSION(&wmInfo.version);
    SDL_GetWindowWMInfo(tipo_ventana.ventana_nativa, &wmInfo);
#endif
    p.miscParams["sdlwin"] = Ogre::StringConverter::toString(size_t(tipo_ventana.ventana_nativa));
#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
    p.miscParams["parentWindowHandle"] = Ogre::StringConverter::toString(size_t(wmInfo.info.x11.window));
#endif
    if (!ventana_lista.empty())
    {
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
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    for (WindowList::iterator it = ventana_lista.begin(); it != ventana_lista.end(); ++it)
    {
        Ogre::RenderWindow *win = it->ventana_ogre;
        w_a = ANativeWindow_getWidth(it->ventana_nativa);
        h_a = ANativeWindow_getHeight(it->ventana_nativa);
        win->resize(w_a, h_a);
        cam->setAspectRatio(Ogre::Real(w_a) / Ogre::Real(h_a));
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
            break;
        case SDL_CONTROLLERDEVICEADDED:
            if (auto c = SDL_GameControllerOpen(event.cdevice.which))
            {
                const char *name = SDL_GameControllerName(c);
                Ogre::LogManager::getSingleton().stream() << "Opened Gamepad: " << (name ? name : "unnamed");
            }
            break;
        default:
            _fireInputEvent(convert(event), event.window.windowID);
            break;
        }
    }
#endif
}
void lgSDL::addInputListener(NativeWindowType *win, InputListener *lis)
{
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    mInputListeners.insert(std::make_pair(SDL_GetWindowID(win), lis));
#else
    mInputListeners.insert(std::make_pair(0, lis));
#endif
}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
using namespace OgreBites;
int dx = 0;
int dy = 0;

void lgSDL::sdlEventosAndroid(AInputEvent *event, int wheel)
{
    Event evt = {0};

    static TouchFingerEvent lastTouch = {0};

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
            break;
        case AMOTION_EVENT_ACTION_UP:
            evt.type = FINGERUP;
            break;
        case AMOTION_EVENT_ACTION_MOVE:
            evt.type = FINGERMOTION;
            break;
        default:
            return;
        }

        Ogre::RenderWindow *win = getRenderWindow();

        evt.tfinger.fingerId = AMotionEvent_getPointerId(event, 0);
        evt.tfinger.x = AMotionEvent_getRawX(event, 0) / win->getWidth() * win->getViewPointToPixelScale();
        evt.tfinger.y = AMotionEvent_getRawY(event, 0) / win->getHeight() * win->getViewPointToPixelScale();

        if (evt.type == FINGERMOTION)
        {
            if (evt.tfinger.fingerId != lastTouch.fingerId)
                return; // wrong finger

            evt.tfinger.dx = evt.tfinger.x - lastTouch.x;
            evt.tfinger.dy = evt.tfinger.y - lastTouch.y;
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
#else
void lgSDL::_fireInputEvent(const OgreBites::Event &event, uint32_t windowID) const
{
    OgreBites::Event scaled = event;
    if (OGRE_PLATFORM == OGRE_PLATFORM_APPLE && event.type == OgreBites::MOUSEMOTION)
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
        if (it->first != windowID && event.type <= OgreBites::TEXTINPUT)
            continue;

        InputListener &l = *it->second;

        switch (event.type)
        {
        case OgreBites::KEYDOWN:
            l.keyPressed(event.key);
            break;
        case OgreBites::KEYUP:
            l.keyReleased(event.key);
            break;
        case OgreBites::MOUSEBUTTONDOWN:
            l.mousePressed(event.button);
            break;
        case OgreBites::MOUSEBUTTONUP:
            l.mouseReleased(event.button);
            break;
        case OgreBites::MOUSEWHEEL:
            l.mouseWheelRolled(event.wheel);
            break;
        case OgreBites::MOUSEMOTION:
            l.mouseMoved(scaled.motion);
            break;
        case OgreBites::FINGERDOWN:
            // for finger down we have to move the pointer first
            l.touchMoved(event.tfinger);
            l.touchPressed(event.tfinger);
            break;
        case OgreBites::FINGERUP:
            l.touchReleased(event.tfinger);
            break;
        case OgreBites::FINGERMOTION:
            l.touchMoved(event.tfinger);
            break;
        case OgreBites::TEXTINPUT:
            l.textInput(event.text);
            break;
        case OgreBites::CONTROLLERAXISMOTION:
            l.axisMoved(event.axis);
            break;
        case OgreBites::CONTROLLERBUTTONDOWN:
            l.buttonPressed(event.cbutton);
            break;
        case OgreBites::CONTROLLERBUTTONUP:
            l.buttonReleased(event.cbutton);
            break;
        }
    }
}
#endif
bool lgSDL::sdl_seleccion(Ogre::RenderWindow *win, Ogre::Camera *cam, bool m, Ogre::RaySceneQuery *mRay, int x, int y, int xrel)
{
    Ogre::Vector3 vt(0, 0, 0);
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
            sdlNodo = itr->movable->getParentSceneNode();
            if (!sdlNodo->getShowBoundingBox())
            {
                if (sdlNodo->getShowBoundingBox())
                {
                    sdlNodo->showBoundingBox(false);
                }
                sdlNodo->showBoundingBox(true);
                seleccion_objecto = true;
                sdlObjeto = itr->movable->getName();
                const char *c = sdlObjeto.c_str();
            }
            else
            {
                sdlNodo->showBoundingBox(false);
                seleccion_objecto = false;
                sdlObjeto = "no";
            }
        }
    }
    return true;
}