#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/MLCPSolvers/btDantzigSolver.h"
#include "BulletDynamics/MLCPSolvers/btMLCPSolver.h"

#include "DebugDrawOgre.h"
#include "MotionStateOgre.h"
#include "BtOgreGP.h"

#include "Ogre.h"

#include "imgui.h"

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
    btRigidBody* cuerpoEstatico(btDiscreteDynamicsWorld *Dw);
    btRigidBody* body;
    Ogre::SceneManager* lgSm;
    Ogre::AnimationState *lgAnimacion2;
    Ogre::Entity *entidad;
    Ogre::SceneNode* nodo;
    Ogre::String nombre_ent;
    Ogre::String mesh_ent;
    btVector3 posicion_ent;
    btVector3 dimension_forma_cualision;
    int p_x=0, p_y=0, p_z=0;
    int e_x=0, e_y=0, e_z=0;
    float r_p = 0, r_y = 0, r_r = 0;
    void mostrar(Ogre::SceneManager* sm,btDiscreteDynamicsWorld *Dw, int tipo);
    void setWord(btDiscreteDynamicsWorld *Dw);
    void setNombre(Ogre::String nombre);
    void setMesh(Ogre::String mesh);
    void setPosicion(int x, int y, int z);
    void setDimensiones(int x, int y, int z);
    void setDimensionesForma(int x, int y, int z);
    void setRotacion(float pitch, float yaw, float roll);
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