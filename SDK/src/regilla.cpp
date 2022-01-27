#include "LibreGame.h"

void LibreGame::regilla()
{
    Ogre::SceneNode *node;
    node = lgManager->getRootSceneNode()->createChildSceneNode();
    Ogre::ManualObject *rejilla = lgManager->createManualObject("Grid");

    for (float linea = -200; linea <= 200; linea += 5)
    {
        rejilla->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
        rejilla->position(200, 0, linea);
        rejilla->position(-200, 0, linea);
        rejilla->index(0);
        rejilla->index(1);
        rejilla->end();
    }

    for (float linea = -200; linea <= 200; linea += 5)
    {
        rejilla->begin("BaseWhiteNoLighting", Ogre::RenderOperation::OT_LINE_STRIP);
        rejilla->position(linea, 0, -200);
        rejilla->position(linea, 0, 200);
        rejilla->index(1);
        rejilla->index(0);
        rejilla->end();
    }

    node->attachObject(rejilla);
    node->setPosition(0, 0, 0);
}