/*
LibreGame es un proyecto que intenta crear un game engine a partir de librerias libres.
*/
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

#include "DebugDrawOgre.h"
#include "MotionStateOgre.h"
#include "BtOgreGP.h"

#include "Ogre.h"

#include <iostream>

///LibreGame utiliza Bullet como motor de fisica para su simulacion,

class lgBullet
{
public:
    lgBullet(Ogre::SceneManager *sm);

protected:
//Configuracion de bullet desde aquí
    btDynamicsWorld *lgMundoDinamico(Ogre::SceneManager *sm);
    btDynamicsWorld *lgMundo;
    btDefaultCollisionConfiguration *m_collisionConfiguration;
    btCollisionDispatcher *m_dispatcher;
    btBroadphaseInterface *m_overlappingPairCache;
    btConstraintSolver *m_constraintSolver;
    btDiscreteDynamicsWorld *m_dynamicsWorld;
    btAlignedObjectArray<btCollisionShape *> m_collisionShapes;
//hasta aqui
///Debug draw para ogre, se muestran las lineas de dibujo de las formas.
    brPhysicsDebugDrawer *m_pPhysicDebugDraw;
    BtOgre::StaticMeshToShapeConverter *staconv;
///Esto crea un cuerpo dinamico en bullet, que se combina con un mesh de ogre.
    btRigidBody *bplayer;
    int lgElementos = 0;
    void setlgElementos(int e);
    int getlgElementos();
    BtOgre::DebugDrawer *dbdraw;
    Ogre::SceneManager *lgSm;
    btScalar m_defaultContactProcessingThreshold;
    Ogre::SceneNode *lnode;
    Ogre::String idNombre;
    Ogre::SceneNode *bNode;
    Ogre::SceneNode *colisionNode;
    virtual void draw(Ogre::SceneManager *sceneMgr);
    virtual void physicsDebugDraw(int debugFlags);
    virtual void caminar(btRigidBody *body, int velocidad);
    virtual void rotar(btRigidBody *body, btScalar velocidad);
    virtual void saltar(btRigidBody *body, int velocidad);
    btRigidBody *plano(Ogre::SceneNode *node, int x, int y, int z, double dimension);
    Ogre::Vector3 setEscala;
    Ogre::Vector3 getEscala()
    {
        return setEscala;
    }
    void updateBody(int total_body);
    Ogre::String getColision;
};