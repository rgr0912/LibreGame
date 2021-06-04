#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

#include "DebugDrawOgre.h"
#include "MotionStateOgre.h"
#include "BtOgreGP.h"

#include "Ogre.h"

#include <iostream>

class lgEntidad2{
    public:
    lgEntidad2(Ogre::String nombre, Ogre::String mesh){
        setNombre(nombre);
        setMesh(mesh);
        setPosicion(0,1,0);
        setDimensionesForma(1,1,1);
    };
    btRigidBody* lgElemento;
    btRigidBody* lgBody(btDiscreteDynamicsWorld *Dw);
    btRigidBody* lgPlayer(btDiscreteDynamicsWorld *Dw);
    btRigidBody* body;
    Ogre::SceneManager* lgSm;
    Ogre::AnimationState *lgAnimacion2;
    Ogre::Entity *entidad;
    Ogre::String nombre_ent;
    Ogre::String mesh_ent;
    btVector3 posicion_ent;
    btVector3 dimension_forma_cualision;
    void mostrar(Ogre::SceneManager* sm,btDiscreteDynamicsWorld *Dw, int tipo);
    void setWord(btDiscreteDynamicsWorld *Dw);
    void setNombre(Ogre::String nombre);
    void setMesh(Ogre::String mesh);
    void setPosicion(int x, int y, int z);
    void setDimensionesForma(int x, int y, int z);
    void setTiempo(Ogre::Real t);
    void setAnimacion(Ogre::String animacion);
    void setAnimacionAlto(Ogre::String anim1, Ogre::String anim2);
    Ogre::Entity* getEntidad(){
        return entidad;
    }
    btRigidBody* getBody(){
        return body;
    };
};

class lgBullet 
{
public:
    lgBullet(Ogre::SceneManager *sm);
    btDynamicsWorld *lgMundoDinamico(Ogre::SceneManager *sm);
    btDynamicsWorld *lgMundo;
    btDefaultCollisionConfiguration *m_collisionConfiguration;
    btCollisionDispatcher *m_dispatcher;
    btBroadphaseInterface *m_overlappingPairCache;
    btConstraintSolver *m_constraintSolver;
    btDiscreteDynamicsWorld *m_dynamicsWorld;
    btAlignedObjectArray<btCollisionShape *> m_collisionShapes;
    brPhysicsDebugDrawer *m_pPhysicDebugDraw;
    BtOgre::StaticMeshToShapeConverter *staconv;
    btRigidBody *bplayer;
    int lgElementos = 0;
    void setlgElementos(int e);
    int getlgElementos();
    BtOgre::DebugDrawer *dbdraw;
    Ogre::SceneManager *lgSm;
    //btRigidBody *localCreateRigidBody(float mass, const btTransform &startTransform, btCollisionShape *shape);
    btScalar m_defaultContactProcessingThreshold;
    Ogre::SceneNode *lnode;
    //void updateBody();
    //btBvhTriangleMeshShape *MeshC(Ogre::Entity *ent);
    //int getCollisionObjectCount();
    Ogre::String idNombre;
    Ogre::SceneNode *bNode;
    Ogre::SceneNode *colisionNode;
    virtual void draw(Ogre::SceneManager *sceneMgr);
    virtual void physicsDebugDraw(int debugFlags);
    virtual void caminar(btRigidBody *body, int velocidad);
    virtual void rotar(btRigidBody* body, btScalar velocidad);
    virtual void saltar(btRigidBody* body, int velocidad);
    btRigidBody *plano(Ogre::SceneNode *node, int x, int y, int z, double dimension);
    btRigidBody *entidad2(Ogre::Entity *ent, Ogre::SceneNode *node, int x, int y, int z);
    btRigidBody *lgEntidad(Ogre::String nombre, Ogre::String mesh, int x, int y, int z);
    Ogre::Vector3 setEscala;
    Ogre::Vector3 getEscala()
    {
        return setEscala;
    }
    void updateBody();
    Ogre::String  getColision;
};