#include "MotionStateOgre.h"

MotionStateOgre::MotionStateOgre(const btTransform &initialpos, Ogre::SceneNode* node){
    _visibleobj = node;
    _pos = initialpos;
}
void MotionStateOgre::setNode(Ogre::SceneNode* node){
    _visibleobj = node;
}
void MotionStateOgre::getWorldTransform(btTransform &worldTr) const{
    worldTr = _pos;
}
void MotionStateOgre::setWorldTransform(const btTransform &worldTr){
    if(NULL == _visibleobj) return;
    btQuaternion rot = worldTr.getRotation();
    _visibleobj->setOrientation(rot.w(), rot.y(), rot.y(), rot.z());
    btVector3 pos = worldTr.getOrigin();
    _visibleobj->setPosition(pos.x(),pos.y(),pos.z());
}
