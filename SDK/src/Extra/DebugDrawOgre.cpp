#include "DebugDrawOgre.h"
#include <iostream>

//#include "OgreBulletUtils.h"

using namespace Ogre;

brPhysicsDebugDrawer::brPhysicsDebugDrawer( Ogre::SceneManager *scm ){
    mContactPoints = &mContactPoints1;

    mLines = new ManualObject("LineasDepuracion");
    //mLines = new DynamicLines(Ogre::RenderOperation::OT_LINE_LIST);
    mTriangles = new ManualObject("TriangulacionDepuracion");

    assert(mLines); 
    assert(mTriangles);

    mLines->setDynamic(true);
    mTriangles->setDynamic(true);

    Ogre::SceneNode*  node =scm->getRootSceneNode()->createChildSceneNode();
    node->attachObject( mLines );
    node->attachObject( mTriangles);

    //mLines->estimateVertexCount(100000 );
    //mLines->estimateIndexCount(0 );
    node->setVisible(true);

    matName = "BulletDebugMode";
    MaterialPtr mtl = MaterialManager::getSingleton().getDefaultSettings()->clone(matName);
    mtl->setReceiveShadows(false);
    mtl->setSceneBlending( SBT_TRANSPARENT_ALPHA);
    mtl->setDepthBias( 0.1, 0 );
    TextureUnitState * tu = mtl->getTechnique(0)->getPass(0)->createTextureUnitState();
    assert(tu);
    tu->setColourOperationEx( LBX_SOURCE1,LBS_DIFFUSE );
    mtl->getTechnique(0)->setLightingEnabled(true);

    mtl->getTechnique(0)->setSelfIllumination(ColourValue::Red);

    mDebugModes = (DebugDrawModes)DBG_DrawWireframe;
    Root::getSingleton().addFrameListener(this);
}
brPhysicsDebugDrawer::~brPhysicsDebugDrawer(){
    Root::getSingleton().removeFrameListener(this);
    delete mLines;
    delete mTriangles;
}
void brPhysicsDebugDrawer::drawLine( const btVector3 &from, const btVector3 &to, const btVector3 &color ){
    ColourValue c( color.getX(), color.getY(),color.getZ() );
    c.saturate();

    mLines->position( cvt(from) );
    mLines->colour( c );
    mLines->position( cvt(to) );
    mLines->colour( c );
}

void brPhysicsDebugDrawer::drawTriangle( const btVector3 &v0, const btVector3 &v1, const btVector3 &v2, const btVector3 &color, btScalar alpha ){
    ColourValue c( color.getX(), color.getY(),color.getZ(), alpha );

    c.saturate();
    mTriangles->position( cvt(v0) );
    mTriangles->colour( c );
    mTriangles->position( cvt(v1) );
    mTriangles->colour( c );
    mTriangles->position( cvt(v2) );
    mTriangles->colour( c );
}

void brPhysicsDebugDrawer::drawContactPoint( const btVector3 &PointOnB, const btVector3 &normalOnB, btScalar distance,int lifeTime, const btVector3 &color ){
    mContactPoints->resize(mContactPoints->size() + 1 );
    ContactPoint p = mContactPoints->back();
    p.from = cvt( PointOnB );

    p.to = p.from + cvt( normalOnB ) * distance;
    p.dieTime =Root::getSingleton().getTimer()->getMilliseconds() + lifeTime;
    p.color.r = color.x();
    p.color.g = color.y();
    p.color.b = color.z();
}
bool brPhysicsDebugDrawer::frameStarted( const Ogre::FrameEvent& evt ){
    mLines->begin( matName,RenderOperation::OT_LINE_LIST );
    mTriangles->begin( matName,RenderOperation::OT_TRIANGLE_LIST );

    size_t now =Root::getSingleton().getTimer()->getMilliseconds();
    std::vector< ContactPoint > *newCP =mContactPoints == &mContactPoints1 ? &mContactPoints2 : &mContactPoints1;

    for ( std::vector< ContactPoint >::iterator i =mContactPoints->begin(); i < mContactPoints->end(); i++ ){
        ContactPoint &cp = *i;
        mLines->position( cp.from );
        mLines->colour( cp.color );
        mLines->position( cp.to );
        if(now <= cp.dieTime )
            newCP->push_back( cp );
    }   

    //mContactPoints->clear();
    //mContactPoints = newCP;

   
    return true;
}
bool brPhysicsDebugDrawer::frameEnded( const Ogre::FrameEvent& evt ){
    mLines->end();
    mTriangles->end();

    
    return true;
}
void brPhysicsDebugDrawer::reportErrorWarning( const char *warningString ){
    LogManager::getSingleton().getDefaultLog()->logMessage( warningString);
}
void brPhysicsDebugDrawer::draw3dText( const btVector3 &location, const char *textString ){

}
void brPhysicsDebugDrawer::setDebugMode( int debugMode ){
    mDebugModes = (DebugDrawModes) debugMode;
}
int brPhysicsDebugDrawer::getDebugMode() const{
    return mDebugModes;
}
void brPhysicsDebugDrawer::clear(){
    mLines->clear();
    mTriangles->clear();
}