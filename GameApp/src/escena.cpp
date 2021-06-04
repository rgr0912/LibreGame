#include "GameApp.h"
GameApp::GameApp(){

}
void GameApp::escena(){
#if OGRE_PLATFORM != OGRE_PLATFORM_ANDROID
    initApp();
#endif
    
    ent3 = new lgEntidad2("Has encontrado la puerta, ¿tienes las llaves?, si no buscalas","testing.mesh");
    ent3->setPosicion(10,6,-10);
    ent3->setDimensionesForma(0.5f,6,3);
    ent3->mostrar(sm,m_dynamicsWorld,2);
}
#if OGRE_PLATFORM == OGRE_PLATFORM_ANDROID
void GameApp::initAppAndroid(AAssetManager *assetMgr, ANativeWindow *window)
{
    lgConfigAndroid = AConfiguration_new();
    AConfiguration_fromAssetManager(lgConfigAndroid, assetMgr);
    lgManagerAndroid = assetMgr;

    ventana_lista.resize(1);
    ventana_lista[0].ventana_nativa = window;
    initApp();
    escena();
}
#endif