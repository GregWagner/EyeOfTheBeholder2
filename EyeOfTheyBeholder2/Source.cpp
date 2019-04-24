#include <SDL.h>
#include <iostream>

int main(int argc, char* argv[])
{
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "SDL initialization failed. SDL Error: " << SDL_GetError();
    } else {
        std::cout << "SDL initialization succeeded!";
    }
    return 0;
}
