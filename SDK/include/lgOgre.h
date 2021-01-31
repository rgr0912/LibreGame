#include "Ogre.h"

class lgOgre{
    public:
    lgOgre();
    virtual void crearRoot();
    protected:
    Ogre::Root* lgRoot;
};