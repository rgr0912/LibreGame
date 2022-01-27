#include "lgBullet.h"

bool useMCLPSolver = true;
void* GROUND_ID = (void*)1;
bool act1 = false;
bool act2 = true;
Ogre::SceneNode* colisionNode;
Ogre::Entity* colisionNode2;
Ogre::String getNombre = "-------------";
int totalBodies=0;
bool legContactProcessedCallback(btManifoldPoint &cp, void *body0, void *body1)
{
    btCollisionObject *o1 = static_cast<btCollisionObject *>(body0);
    btCollisionObject *o2 = static_cast<btCollisionObject *>(body1);

    void *ID1 = o1->getUserPointer();
    void *ID2 = o2->getUserPointer();

    if (ID1 != GROUND_ID || ID2 != GROUND_ID)
    {

        if (ID1 != GROUND_ID && ID1)
        {
            colisionNode = (Ogre::SceneNode *)o1->getUserPointer();
            getNombre = "";
        }

        if (ID2 != GROUND_ID && ID2)
        {
            colisionNode = (Ogre::SceneNode *)o2->getUserPointer();
            getNombre = colisionNode->getName();
        }
    }
    return false;
}

lgBullet::lgBullet(Ogre::SceneManager* sm)
{
    lgSm = sm;
}
btDynamicsWorld *lgBullet::lgMundoDinamico(Ogre::SceneManager *esceneManager)
{
    lgSm = esceneManager;

    m_collisionConfiguration = new btDefaultCollisionConfiguration();
    m_dispatcher = new btCollisionDispatcher(m_collisionConfiguration);
    btVector3 worldMin(-1000, -1000, -1000);
    btVector3 worldMax(1000, 1000, 1000);
    m_overlappingPairCache = new btAxisSweep3(worldMin, worldMax);
    if (useMCLPSolver)
    {
        btDantzigSolver *mlcp = new btDantzigSolver();
        btMLCPSolver *sol = new btMLCPSolver(mlcp);
        m_constraintSolver = sol;
    }
    else
    {
        m_constraintSolver = new btSequentialImpulseConstraintSolver();
    }
    m_dynamicsWorld = new btDiscreteDynamicsWorld(m_dispatcher, m_overlappingPairCache, m_constraintSolver, m_collisionConfiguration);
    if (useMCLPSolver)
    {
        m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 1; 
    }
    else
    {
        m_dynamicsWorld->getSolverInfo().m_minimumSolverBatchSize = 128;
    }
    m_dynamicsWorld->getSolverInfo().m_globalCfm = 0.00001;

    draw(lgSm);

    dbdraw = new BtOgre::DebugDrawer(lgSm->getRootSceneNode(), m_dynamicsWorld);
    m_dynamicsWorld->setDebugDrawer(dbdraw);
    //dbdraw->setDebugMode(true);

    gContactProcessedCallback = legContactProcessedCallback;

    m_dynamicsWorld->setGravity(btVector3(0, -9.8f, 0));
    return m_dynamicsWorld;
}
void lgBullet::physicsDebugDraw(int debugFlags){
	if(m_dynamicsWorld && m_dynamicsWorld->getDebugDrawer()){
		m_dynamicsWorld->getDebugDrawer()->setDebugMode(debugFlags);
		m_dynamicsWorld->debugDrawWorld();
	}
}
btRigidBody* lgBullet::plano(Ogre::SceneNode* node, int x, int y, int z, double tamano){
	int upAxis = 1;

	btVector3 groundExtents(tamano, tamano, tamano);
	groundExtents[upAxis] = 0.5f;
	btCollisionShape* groundShape = new btBoxShape(groundExtents);
	m_collisionShapes.push_back(groundShape);
	
	MotionStateOgre* fallMotionState = new MotionStateOgre(btTransform(btQuaternion(0,0,0,1),btVector3(x,y,z)), node);
	btScalar mass = 0;
	btVector3 fallInertia(0,0,0);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,groundShape,fallInertia);
	btRigidBody* body = new btRigidBody(fallRigidBodyCI);

	return body;
}
void lgBullet::draw(Ogre::SceneManager *esceneManager)
{
    lgSm = esceneManager;
    m_pPhysicDebugDraw = new brPhysicsDebugDrawer(lgSm);
    m_dynamicsWorld->setDebugDrawer(m_pPhysicDebugDraw);
}
void lgBullet::updateBody(int total_body){
    btRigidBody *TObject;

    for(int i=0; i < total_body; i=i+1){
        btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
        btRigidBody* body = btRigidBody::upcast(obj);
        if(body && body->getMotionState()){
            btTransform trans;
            body->getMotionState()->getWorldTransform(trans);

            void *userPointer = body->getUserPointer();
            if(userPointer){
                btQuaternion orientation = trans.getRotation();
                Ogre::SceneNode *scenenode = static_cast<Ogre::SceneNode *>(userPointer);
                scenenode->setPosition(Ogre::Vector3(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ()));
                scenenode->setOrientation(Ogre::Quaternion(orientation.getX(),orientation.getY(),orientation.getZ(),orientation.getW()));
			}
		}
    }
    getColision = getNombre;   

}
void lgBullet::caminar(btRigidBody* body, int velocidad){
	btVector3 vel;

	btTransform xform = body->getWorldTransform();
	btVector3 cur = body->getLinearVelocity();
	btVector3 basic = xform.getBasis()[0];
       
	vel = 0.5f*velocidad*basic;

	body->setLinearVelocity(btVector3(vel[2],cur[1],vel[0]));
}
void lgBullet::rotar(btRigidBody* body, btScalar velocidad){
	btTransform xformr = body->getWorldTransform();
    btMatrix3x3 R = xformr.getBasis();
        
    R = R * btMatrix3x3(btQuaternion(btVector3(0,1,0),velocidad));
        
    xformr.setBasis(R);
    body->setWorldTransform(xformr);
}
void lgBullet::saltar(btRigidBody* body, int velocidad){
	body->applyCentralForce(btVector3(btVector3(0,150,0)));
}