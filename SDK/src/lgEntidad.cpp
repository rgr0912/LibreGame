#include "lgEntidad.h"
#include "lgBullet.h"

void lgEntidad2::mostrar(Ogre::SceneManager *sm, btDiscreteDynamicsWorld *Dw, int tipo)
{
	lgSm = sm;
	switch(tipo){
		case 0 : lgElemento = lgPlayer(Dw); break;
		case 1 : lgElemento = lgBody(Dw); break;
		case 2 : lgElemento = cuerpoEstatico(Dw); break;
	}
}
btRigidBody *lgEntidad2::lgBody(btDiscreteDynamicsWorld *Dw)
{
	//construccion de vector sombras mesh
	Ogre::MeshPtr vectorMesh = Ogre::MeshManager::getSingleton().load(mesh_ent, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	unsigned short src, dest;
	if (!vectorMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		vectorMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);

	entidad = lgSm->createEntity(nombre_ent, mesh_ent);
	//entidad->setMaterialName("RTSS/OffsetMapping");
	nodo = lgSm->getRootSceneNode()->createChildSceneNode();
	nodo->attachObject(entidad);
	nodo->setPosition(p_x, p_y, p_z);
	nodo->setScale(e_x, e_y, e_z);
	nodo->pitch(Ogre::Degree(r_p));
	nodo->yaw(Ogre::Degree(r_y));
	nodo->roll(Ogre::Degree(r_r));

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1, 0, 0, 0));

	btVector3 inercia(0, 0.98f, 0);

	startTransform.setOrigin(posicion_ent);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));

	BtOgre::StaticMeshToShapeConverter *staconv = new BtOgre::StaticMeshToShapeConverter(entidad);
	btCollisionShape *mshape = new btBoxShape(dimension_forma_cualision);

	btScalar mass = 0;
	btVector3 fallInertia(0, 90.8, 0);
	mshape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mshape, fallInertia);
	body = new btRigidBody(fallRigidBodyCI);

	body->setUserPointer(nodo);

	Dw->addRigidBody(body);
	return body;
}
btRigidBody *lgEntidad2::cuerpoEstatico(btDiscreteDynamicsWorld *Dw)
{
	//construccion de vector sombras mesh
	Ogre::MeshPtr vectorMesh = Ogre::MeshManager::getSingleton().load(mesh_ent, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	unsigned short src, dest;
	if (!vectorMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		vectorMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);

	entidad = lgSm->createEntity(nombre_ent, mesh_ent);
	//entidad->setMaterialName("RTSS/OffsetMapping");
	nodo = lgSm->getRootSceneNode()->createChildSceneNode();
	nodo->attachObject(entidad);
	nodo->setPosition(p_x, p_y, p_z);
	nodo->setScale(e_x, e_y, e_z);
	nodo->pitch(Ogre::Degree(r_p));
	nodo->yaw(Ogre::Degree(r_y));
	nodo->roll(Ogre::Degree(r_r));

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(1, 0, 0, 0));

	btVector3 inercia(0, 0, 0);

	startTransform.setOrigin(posicion_ent);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));

	BtOgre::StaticMeshToShapeConverter *staconv = new BtOgre::StaticMeshToShapeConverter(entidad);

	btCollisionShape *mshape = new btBoxShape(dimension_forma_cualision);

	btScalar mass = 0;
	btVector3 fallInertia(0, 0, 0);
	mshape->calculateLocalInertia(mass, fallInertia);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mshape, fallInertia);
	body = new btRigidBody(fallRigidBodyCI);

	body->setUserPointer(nodo);

	Dw->addRigidBody(body);
	return body;
}
btRigidBody *lgEntidad2::lgPlayer(btDiscreteDynamicsWorld *Dw)
{
	//construccion de vector sombras mesh
	Ogre::MeshPtr vectorMesh = Ogre::MeshManager::getSingleton().load(mesh_ent, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	unsigned short src, dest;
	if (!vectorMesh->suggestTangentVectorBuildParams(Ogre::VES_TANGENT, src, dest))
		vectorMesh->buildTangentVectors(Ogre::VES_TANGENT, src, dest);

	Ogre::MeshPtr bMesh = Ogre::MeshManager::getSingleton().load(mesh_ent, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
	bMesh->buildEdgeList();
	
	entidad = lgSm->createEntity(nombre_ent, mesh_ent);
	entidad->setMaterialName("RTSS/OffsetMapping");
	nodo = lgSm->getRootSceneNode()->createChildSceneNode();
	nodo->attachObject(entidad);
	nodo->setPosition(p_x, p_y, p_z);
	nodo->setScale(e_x, e_y, e_z);
	nodo->pitch(Ogre::Degree(r_p));
	nodo->yaw(Ogre::Degree(r_y));
	nodo->roll(Ogre::Degree(r_r));

	btTransform startTransform;
	startTransform.setIdentity();
	startTransform.setRotation(btQuaternion(0, 0, 1, 0));

	btVector3 pos(posicion_ent);

	startTransform.setOrigin(pos);

	btDefaultMotionState *state = new btDefaultMotionState(btTransform(startTransform));

	btCollisionShape *mshape = new btCapsuleShape(btScalar(dimension_forma_cualision.x()), btScalar(dimension_forma_cualision.y()));
	btScalar mass = 2;
	btVector3 fallInertia(0, 100, 0);
	btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass, state, mshape, fallInertia);
	body = new btRigidBody(fallRigidBodyCI);

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
	p_x = x, p_y = y, p_z = z;
}
void lgEntidad2::setDimensiones(int x, int y, int z)
{
	e_x = x, e_y = y, e_z = z;
}
void lgEntidad2::setDimensionesForma(int x, int y, int z)
{
	dimension_forma_cualision = btVector3(x, y, z);
}
void lgEntidad2::setRotacion(float pitch, float yaw, float roll){
    r_p = pitch, r_y = yaw, r_r = roll;
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
void lgBullet::setlgElementos(int e)
{
	lgElementos = getlgElementos() + e;
}
int lgBullet::getlgElementos()
{
	return lgElementos;
}