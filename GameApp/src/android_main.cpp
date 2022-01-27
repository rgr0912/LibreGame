#include <android_native_app_glue.h>
#include <android/log.h>
#include <android/permission_manager.h>

#include "GameApp.h"
#include "gestureDetector.h"

#include <iostream>

#define APPNAME "MyApp2"

/*=============================================================================
 | Ogre Android bridge
 =============================================================================*/
bool *p_open2;
Ogre::Vector3 lastpos;
int cx = 0, cy = 0;
float x = 0;
float y = 0;
struct OgreAndroidBridge
{
    static int32_t check_android_permissions(struct android_app *app);

    static int32_t handleInput(struct android_app *app, AInputEvent *event)
    {

        if (!AndroidApp.getRenderWindow())
            return 0;

        static float len = 0;

        int wheel = 0; // overrides other events if mPinchGesture triggers

        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
        {
            //__android_log_print(ANDROID_LOG_VERBOSE, APPNAME, "fffff");

            ndk_helper::GESTURE_STATE s = mPinchGesture.Detect(event);

            if (s & ndk_helper::GESTURE_STATE_START)
            {
                ndk_helper::Vec2 p1, p2;
                mPinchGesture.GetPointers(p1, p2);
                len = (p1 - p2).length();
            }
            else if (s & ndk_helper::GESTURE_STATE_MOVE)
            {
                ndk_helper::Vec2 p1, p2;
                mPinchGesture.GetPointers(p1, p2);
                float curr = (p1 - p2).length();

                if (fabs(curr - len) / AndroidApp.getRenderWindow()->getWidth() > 0.01)
                {
                    wheel = (curr - len) > 0 ? 1 : -1;
                    len = curr;
                }
            }
        }
        float dpx = AndroidApp.w_a / AndroidApp.getRenderWindow()->getWidth();
        float dpy = AndroidApp.h_a / AndroidApp.getRenderWindow()->getHeight();


        if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
        {
            x = AMotionEvent_getX(event, 0);
            y = AMotionEvent_getY(event, 0);

            __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","TouchScreen Event: X:%d and Y:%d", int(x), int(y));

            int xx = (int)x;

            ImGuiIO &io = ImGui::GetIO();
            io.MousePos = ImVec2(int(x),int(y));
            AndroidApp.setValor(int(x), int(y));
        }

        AndroidApp.setValorW(AndroidApp.w_a, AndroidApp.h_a);

        AndroidApp.sdlEventosAndroid(event, wheel);

        return 1;
    }
    static void handleCmd(struct android_app *app, int32_t cmd)
    {

        switch (cmd)
        {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO INICIO");
            if (app->window)
            {
                if (!AndroidApp.getRenderWindow())
                {
                    AndroidApp.initAppAndroid(app->activity->assetManager, app->window);
                    __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO NO VENTANA");
                }
                else
                {

                    //mAssetManager = app->activity->assetManager;
                    AConfiguration *config = AConfiguration_new();
                    AConfiguration_fromAssetManager(config, app->activity->assetManager);
                    AndroidApp.getRenderWindow()->_notifySurfaceCreated(app->window, config);
                    AConfiguration_delete(config);
                    __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO YA HAY UNA VENTANA");
                }
            }
            break;
        case APP_CMD_TERM_WINDOW:
            __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO TERMINAR");
            if (AndroidApp.getRenderWindow())
                AndroidApp.getRenderWindow()->_notifySurfaceDestroyed();
            break;
        case APP_CMD_RESUME:
            __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO RESUMEN");
            if (AndroidApp.getRenderWindow())
                //AndroidApp.getRenderWindow()->setVisible(true);
                AndroidApp.getRenderWindow()->_notifySurfaceDestroyed();

            break;
        case APP_CMD_PAUSE:
            __android_log_print(ANDROID_LOG_VERBOSE, "LibreGame","ESTADO PAUSA");
            if (AndroidApp.getRenderWindow())
                //AndroidApp.getRenderWindow()->setVisible(false);
                AndroidApp.getRenderWindow()->_notifySurfaceDestroyed();
            break;
        case APP_CMD_CONFIG_CHANGED:
            break;
        }
    }

    static void go(struct android_app *state)
    {
        int ident, events;
        struct android_poll_source *source;

        while (true)
        {
            while ((ident = ALooper_pollAll(0, NULL, &events, (void **)&source)) >= 0)
            {
                if (source != NULL)
                    source->process(state, source);

                if (state->destroyRequested != 0)
                    return;
            }

            if (AndroidApp.getRenderWindow() && AndroidApp.getRenderWindow()->isActive())
            {
                AndroidApp.getRoot()->renderOneFrame();
            }
        }
    }

private:
    static GameApp AndroidApp;
    static ndk_helper::PinchDetector mPinchGesture;
};

GameApp OgreAndroidBridge::AndroidApp;
ndk_helper::PinchDetector OgreAndroidBridge::mPinchGesture;

void android_main(struct android_app *state)
{
    state->onAppCmd = &OgreAndroidBridge::handleCmd;
    state->onInputEvent = &OgreAndroidBridge::handleInput;
    OgreAndroidBridge::go(state);
}