#ifndef MYMOTIONSTATE_H_INCLUDED
#define MYMOTIONSTATE_H_INCLUDED

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

class MotionStateOgre : public btMotionState{
    protected:
        Ogre::SceneNode* _visibleobj;
        btTransform _pos;
    public:
        MotionStateOgre(const btTransform &initialpos, Ogre::SceneNode* node);
        void setNode(Ogre::SceneNode* node);
        virtual void getWorldTransform(btTransform &worldTr) const;
        virtual void setWorldTransform(const btTransform &worldTr);

};
#endif // MYMOTIONSTATE_H_INCLUDED
