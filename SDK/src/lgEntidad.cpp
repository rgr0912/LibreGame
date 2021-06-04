#include "lgBullet.h"

void lgEntidad2::mostrar(Ogre::SceneManager *sm, btDiscreteDynamicsWorld *Dw, int tipo)
{
	lgSm = sm;
	if (tipo == 1)
	{
		lgElemento = lgPlayer(Dw);
	}
	else
	{
		lgElemento = lgBody(Dw);
	}
	//setWord(Dw);
}
int i3 = 0;
btRigidBody *lgEntidad2::lgBody(btDiscreteDynamicsWorld *Dw)
{
	Ogre::Entity *entidad = lgSm->createEntity(nombre_ent, mesh_ent);
	Ogre::SceneNode *nodo = lgSm->getRootSceneNode()->createChildSceneNode(nombre_ent);
	nodo->scale(2, 2, 2);
	nodo->attachObject(entidad);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1, 0, 0, 0));

	//btVector3 posicion_ent;
	btVector3 inercia(0, 0.98f, 0);

	startTransform.setOrigin(posicion_ent);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));
	//MyMotionState* state = new MyMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)), player);

	BtOgre::StaticMeshToShapeConverter *staconv = new BtOgre::StaticMeshToShapeConverter(entidad);

	btCollisionShape *mshape = new btBoxShape(dimension_forma_cualision);
	//btCollisionShape *mshape = new btCapsuleShape(btScalar(3.5f), btScalar(4.0f));

	btScalar mass = 0;
	btVector3 fallInertia(0, 90.8, 0);
	mshape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mshape, fallInertia);
	body = new btRigidBody(fallRigidBodyCI);

	body->setUserPointer(nodo);

	Dw->addRigidBody(body);

	i3++;

	std::cout << "total elementosx : " << i3 << std::endl;
	return body;
}
btRigidBody *lgEntidad2::lgPlayer(btDiscreteDynamicsWorld *Dw)
{
	entidad = lgSm->createEntity(nombre_ent, mesh_ent);
	Ogre::SceneNode *nodo = lgSm->getRootSceneNode()->createChildSceneNode();
	//nodo->scale(0.1,0.1,0.1);
	nodo->attachObject(entidad);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 1, 0));

	btVector3 pos(posicion_ent);
	//btVector3 inercia(0, 100, 0);

	startTransform.setOrigin(pos);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));
	//MyMotionState* state = new MyMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)), player);

	btCollisionShape *mshape = new btCapsuleShape(btScalar(3.5f), btScalar(4.0f));
	btScalar mass = 2;
	btVector3 fallInertia(0, 100, 0);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mshape, fallInertia);
	body = new btRigidBody(fallRigidBodyCI);

	//bplayer = new btRigidBody(btScalar(2),state,mshape,inercia);
	body->setActivationState(DISABLE_DEACTIVATION);
	body->setAngularFactor(0.0f);
	body->setFriction(btScalar(3));
	body->setUserPointer(nodo);

	Dw->addRigidBody(body);
	return body;
}
void lgEntidad2::setWord(btDiscreteDynamicsWorld *dw)
{
	dw->addRigidBody(lgElemento);
}
void lgEntidad2::setNombre(Ogre::String nombre)
{
	nombre_ent = nombre;
}
void lgEntidad2::setPosicion(int x, int y, int z)
{
	posicion_ent = btVector3(x, y, z);
}
void lgEntidad2::setDimensionesForma(int x, int y, int z)
{
	dimension_forma_cualision = btVector3(x, y, z);
}
void lgEntidad2::setMesh(Ogre::String mesh)
{
	mesh_ent = mesh;
}
void lgEntidad2::setTiempo(Ogre::Real tiempo)
{
	lgAnimacion2->addTime(tiempo);
}
void lgEntidad2::setAnimacion(Ogre::String animacion)
{
	lgAnimacion2 = entidad->getAnimationState(animacion);
	lgAnimacion2->setLoop(true);
	lgAnimacion2->setEnabled(true);
}
void lgEntidad2::setAnimacionAlto(Ogre::String anim1, Ogre::String anim2)
{
	lgAnimacion2 = entidad->getAnimationState(anim1);
	lgAnimacion2->setLoop(false);
	lgAnimacion2->setEnabled(false);
	lgAnimacion2 = entidad->getAnimationState(anim2);
	lgAnimacion2->setLoop(true);
	lgAnimacion2->setEnabled(true);
}
btRigidBody *lgBullet::lgEntidad(Ogre::String nombre, Ogre::String mesh, int x, int y, int z)
{
	Ogre::Entity *ent = lgSm->createEntity(nombre, mesh);
	Ogre::SceneNode *nodo = lgSm->getRootSceneNode()->createChildSceneNode();
	nodo->scale(getEscala());
	nodo->attachObject(ent);

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1, 0, 0, 0));

	btVector3 pos(x, y, z);
	btVector3 inercia(0, 0.98f, 0);

	startTransform.setOrigin(pos);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));
	//MyMotionState* state = new MyMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)), player);

	staconv = new BtOgre::StaticMeshToShapeConverter(ent);

	btCollisionShape *mShape = new btBoxShape(btVector3(1, 1, 1));

	btScalar mass = 5;
	btVector3 fallInertia(0, 90.8, 0);
	mShape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mShape, fallInertia);
	btRigidBody *body = new btRigidBody(fallRigidBodyCI);

	body->setUserPointer(nodo);

	m_dynamicsWorld->addRigidBody(body);

	setlgElementos(1);

	std::cout << "total elementos : " << getlgElementos() << std::endl;

	return bplayer;
}
void lgBullet::setlgElementos(int e)
{
	lgElementos = getlgElementos() + e;
}
int lgBullet::getlgElementos()
{
	return lgElementos;
}