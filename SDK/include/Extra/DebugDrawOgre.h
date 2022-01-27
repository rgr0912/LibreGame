#ifndef DebugDrawer_h__
#define DebugDrawer_h__

//#include "../common.h"

#include "Ogre.h"
#include "btBulletDynamicsCommon.h"

inline btVector3 cvt(const Ogre::Vector3 &V){
    return btVector3(V.x, V.y, V.z);
}

inline Ogre::Vector3 cvt(const btVector3&V){
    return Ogre::Vector3(V.x(), V.y(), V.z());
}

inline btQuaternion cvt(const Ogre::Quaternion &Q)
{
    return btQuaternion(Q.x, Q.y, Q.z, Q.w);
};

inline Ogre::Quaternion cvt(const btQuaternion &Q)
{
    return Ogre::Quaternion(Q.w(), Q.x(), Q.y(), Q.z());
};

class brPhysicsDebugDrawer: public btIDebugDraw,public Ogre::FrameListener{
    public:
        brPhysicsDebugDrawer( Ogre::SceneManager *scm);
        ~brPhysicsDebugDrawer();
        virtual void     drawLine (const btVector3 &from, const btVector3 &to, const btVector3 &color);
        virtual void     drawTriangle (const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar);
        virtual void     drawContactPoint (const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,int lifeTime, const btVector3 &color);
        virtual void     reportErrorWarning (const char *warningString);
        virtual void     draw3dText (const btVector3 &location, const char *textString);
        virtual void     setDebugMode (int debugMode);
        virtual int     getDebugMode () const;
        virtual void clear();
    protected:
        bool frameStarted(const Ogre::FrameEvent& evt);
        bool frameEnded(const Ogre::FrameEvent& evt);
    private:
        struct ContactPoint{
            Ogre::Vector3 from;
            Ogre::Vector3 to;
            Ogre::ColourValue   color;
            size_t        dieTime;

        };
        DebugDrawModes               mDebugModes;
        Ogre::ManualObject          *mLines;
        Ogre::ManualObject          *mTriangles;
        std::vector< ContactPoint >* mContactPoints;
        std::vector< ContactPoint >  mContactPoints1;
        std::vector< ContactPoint >  mContactPoints2;
        Ogre::String matName;
};
#endif // DebugDrawer_h__