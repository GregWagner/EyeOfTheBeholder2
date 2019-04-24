#define _CRT_SECURE_NO_DEPRECATE

#include "3DEngine.h"
#include <iostream>

int main(int argc, char* argv[])
{
    C3DEngine game;
    game.initViewArea();
    game.test();
    //if(spectalum.playIntro())
    //{
    game.loadMap(game.configuration.startMapID);
    game.run();
    //}
    return 0;
}
