#include "GameApp.h"

GameApp::GameApp(){

}
void GameApp::escena(){
    initApp();
}
int main(){
    GameApp app;
    app.escena();
    return 0;
}