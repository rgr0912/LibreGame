#include "lgOgre.h"

lgOgre::lgOgre()
{
}
void lgOgre::crearRoot()
{
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
    lgRoot = OGRE_NEW Ogre::Root("");
#else
    lgRoot = new Ogre::Root();
#endif
}