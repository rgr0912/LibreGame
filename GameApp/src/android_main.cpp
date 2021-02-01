#include <android_native_app_glue.h>
#include <android/log.h>

#include "GameApp.h"
//#include "gestureDetector.h"

/*=============================================================================
 | Ogre Android bridge
 =============================================================================*/
struct OgreAndroidBridge
{
    static void handleCmd(struct android_app *app, int32_t cmd)
    {
        switch (cmd)
        {
        case APP_CMD_SAVE_STATE:
            break;
        case APP_CMD_INIT_WINDOW:
            if (app->window)
            {
                if (!AndroidApp.getRenderWindow())
                {
                    AndroidApp.initAppAndroid(app->activity->assetManager, app->window);
                }
                else
                {
                    AConfiguration *config = AConfiguration_new();
                    AConfiguration_fromAssetManager(config, app->activity->assetManager);
                    AndroidApp.getRenderWindow()->_notifySurfaceCreated(app->window, config);
                    AConfiguration_delete(config);
                }
            }
            break;
            case APP_CMD_TERM_WINDOW:
                if(AndroidApp.getRenderWindow())
                    AndroidApp.getRenderWindow()->_notifySurfaceDestroyed();
                break;
            case APP_CMD_RESUME:
                if(AndroidApp.getRenderWindow())
                    AndroidApp.getRenderWindow()->setVisible(true);
                break;
            case APP_CMD_PAUSE:
                if(AndroidApp.getRenderWindow())
                    AndroidApp.getRenderWindow()->setVisible(false);
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
};

GameApp OgreAndroidBridge::AndroidApp;

void android_main(struct android_app *state)
{
    state->onAppCmd = &OgreAndroidBridge::handleCmd;
    OgreAndroidBridge::go(state);
}