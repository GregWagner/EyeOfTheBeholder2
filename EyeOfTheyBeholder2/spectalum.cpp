#define _CRT_SECURE_NO_DEPRECATE

#include "3DEngine.h"
#include <iostream>

int main(int argc, char* argv[])
{
    Engine spectalum;

    //  spectalum.playIntro())
    spectalum.loadMap(spectalum.mConfiguration.mStartMapID);
    spectalum.run();
    return 0;
}
