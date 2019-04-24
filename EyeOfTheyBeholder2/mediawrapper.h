#pragma once

#include "SFont.h"
#include <SDL.h>

//
// class initialisation
//

class MEDIAWrapper {
public:
    //neues Fenster erzueugen
    void setupWindow(int posX, int posY, int width, int height, bool fullscreen);

    //leeres Bild erstellen
    void createImage(int imageID, int paletteSourceImageID, int width, int height);

    //Bild laden
    void loadImage(int imageID, char* path);

    //Surface als BMP File abspeichern
    void saveBMP(int imageID);

    //Bild zu Bild blitten
    void copyImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int transpR, int transpG, int transpB);

    //Bild zu Bild blitten mit Größenänderung
    bool copyScaledImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int toWidth, int toHeight, int transpR, int transpG, int transpB, bool mirror);

    //Original CPS Bild laden
    void loadCPS(int imageID, char* filename, char* palette, int posX, int posY, int width, int height, bool transp, bool sprite);

    //WORD Wert aus CPS File auslesen
    int getWord(unsigned char string[64000], int pos);

    //Format80 Decoder
    int format80decode(unsigned char image_in[], unsigned char image_out[]);

    //Sound laden
    void loadSound(int nr, char path[128]);

    //Sound freigeben
    void freeSound(int nr);

    //Sound abspielen
    void playSound(int nr);

    //Sound anhalten
    void stopSound(int nr);

    //Sound spielt?
    bool isPlaying(int nr);

    //Bild aus Speicher löschen
    void freeImage(int nr);

    //prüfen ob Bild scjon geladen wurde
    bool imageLoaded(int nr);

    //Bild zeichnen
    void drawImage(int imageID, int posX, int posY, int fromPosX = 0, int fromPosY = 0, int width = 0, int height = 0);

    //Text zeichnen
    void drawText(int fontID, int posX, int posY, int r, int g, int b, char* text, bool center = false);

    //Bildschirm Refresh
    void refresh();

    //"echten" screen zeichnen
    void updateVideo();

    //Viereck zeichnen
    void fillRect(int posX, int posY, int width, int height, int r, int g, int b, int imageID);

    //Palette laden für spätere Pixel Aktionen
    void loadPal(char* filename);

    //Pixel zeichnen mit aktueller Palette
    void drawPixel(int posX, int posY, int palOffset, int imageID);

    //Gammewert setzen
    void setGamma(int r, int g, int b);

    //Maus Koordinaten übergeben
    void getMouseState(int* x, int* y);

    //Maustaste links
    bool getMouseLeft();

    //Maustaste rechts
    bool getMouseRight();

    //Tastenevents
    void updateKeys();

    //SDL beenden
    void quit();

    //bestimmte Zeit warten
    void sleep(int ms);

    //Millisekunden auslesen
    int getMilliSeconds();

    //SDL screen
    SDL_Surface* screen_game;
    SDL_Surface* screen;

    //zu SDL Farben konvertierte 6Bit EOB-Palette
    SDL_Color current_colors[256];

    //Auflösung
    int screenWidth;
    int screenHeight;

    //Fonts
    SFont_Font* font[10];

    //Grafikspeicher
    SDL_Surface* images[512];
    int imageCounter;

    //Sounds
    Mix_Chunk* sound[512];
    int soundCounter;

    //Tasten
    bool keyEsc, keyLeft, keyRight, keyUp, keyDown;
};
