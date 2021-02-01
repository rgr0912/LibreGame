#include "Ogre.h"

extern "C" struct SDL_Window;

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
#include <android/native_window.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
typedef ANativeWindow NativeWindowType;
#else
typedef SDL_Window NativeWindowType;
#endif

struct NativeWindowPair
{
    Ogre::RenderWindow *render;
    NativeWindowType *native;
};

class lgOgre
{
public:
    lgOgre();
    virtual void crearRoot();
    Ogre::RenderWindow *getRenderWindow() const
    {
        return mWindows.empty() ? NULL : mWindows[0].render;
    }

    Ogre::Root *getRoot() const
    {
        return lgRoot;
    }

protected:
    Ogre::Root *lgRoot;
    typedef std::vector<NativeWindowPair> WindowList;
    WindowList mWindows;
};