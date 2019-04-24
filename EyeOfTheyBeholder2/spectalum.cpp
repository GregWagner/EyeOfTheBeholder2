#define _CRT_SECURE_NO_DEPRECATE

#include "3DEngine.h"
#include <iostream>

int main(int argc, char* argv[])
{
    C3DEngine spectalum;
    spectalum.initViewArea();
    spectalum.test();
    //if(spectalum.playIntro())
    //{
    spectalum.loadMap(spectalum.configuration.startMapID);
    spectalum.run();
    //}
    return 0;
}
