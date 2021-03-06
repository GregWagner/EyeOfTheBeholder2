#pragma warning(disable : 4996)

#include "mediawrapper.h"
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <sys/stat.h>

// Fenster erzeugen
void MediaWrapper::setupWindow(int posX, int posY, int width, int height, bool mFullscreen)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return;
    }

    // create the window where we will draw
    if (mFullscreen) {
        mWindow = SDL_CreateWindow("Spectalum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);
    } else {
        mWindow = SDL_CreateWindow("Spectalum", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            width, height, SDL_WINDOW_OPENGL);
    }

    // call SDL_CreateRenderer in order for draw calls to affect this window
    mRenderer = SDL_CreateRenderer(mWindow, -1, 0);
    /*
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear"); // make the scaled rendering look smoother.
    SDL_RenderSetLogicalSize(mRenderer, 640, 480);
    */

    // SDL screen_game erzeugen
    mScreen = SDL_CreateRGBSurface(0, 640, 480, 32, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    mTexture = SDL_CreateTexture(mRenderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, 640, 480);

    // SDL screen erzeugen
    this->mScreenWidth = width;
    this->mScreenHeight = height;

    mFont[0] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 0, 0, 0);
    mFont[1] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 0, 0, 255);
    mFont[2] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 254, 254, 254);
    mFont[3] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 116, 232, 252);
    mFont[4] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 254, 254, 254);
    mFont[5] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 252, 84, 84);
    mFont[6] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 84, 252, 252);
    mFont[7] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 0, 0, 0);

    // Mauszeiger ausschalten
    SDL_ShowCursor(SDL_DISABLE);

#ifdef SDL_V1
    //Audio Init
    Mix_OpenAudio(24000, AUDIO_S16SYS, 2, 1024);
#endif

    // Surfaces initialisieren
    for (int s = 0; s < 512; s++) {
        mSurfaceImages[s] = nullptr;
    }
    std::cout << "Leaving " << __FUNCTION__ << '\n';
}

// Bild laden
void MediaWrapper::loadImage(int imageID, char* path)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    char realpath[128];

    this->freeImage(imageID);

    sprintf_s(realpath, "%s.bmp", path);
    mSurfaceImages[imageID] = SDL_LoadBMP(realpath);
    std::cout << "Leaving " << __FUNCTION__ << '\n';
}

// Surface als BMP File abspeichern
void MediaWrapper::saveBMP(int imageID)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    if (mSurfaceImages[imageID] != NULL)
        SDL_SaveBMP(mSurfaceImages[imageID], "out.bmp");
    else
        printf("error saving image: %d\n", imageID);
}

// Bild aus Speicher l�schen
void MediaWrapper::freeImage(int i)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    if (mSurfaceImages[i] != NULL) {
        SDL_FreeSurface(mSurfaceImages[i]);
    }

    delete (mSurfaceImages[i]);
    mSurfaceImages[i] = NULL;
}

// pr�fen ob Bild bereits erstellt wurde
bool MediaWrapper::imageLoaded(int nr)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    if (mSurfaceImages[nr] != NULL)
        return true;
    return false;
}

// CPS Bild laden
void MediaWrapper::loadCPS(int imageID, std::string filename, std::string palette, int posX, int posY, int width, int height, bool transp, bool sprite)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    FILE *palfile, *source;
    unsigned char pal[768];
    unsigned char src[64000];
    unsigned char srcdata[64000];
    unsigned char dest[64000];
    long pos = 0;
    int filesize;

    this->freeImage(imageID);

    // Palette laden
    palfile = NULL;
    palfile = fopen(palette.c_str(), "rb");
    std::cout << "Opening palette file: " << palette << '\n';
    if (palfile == NULL) {
        printf("palette not found: %s\n", palette.c_str());
        this->sleep(2000);
        this->quit();
    }
    printf("palette found: %s\n", palette.c_str());
    fseek(palfile, 0, SEEK_END);
    filesize = ftell(palfile);
    fseek(palfile, 0, SEEK_SET);
    if (filesize != 768) {
        printf("no valid PAL file: %s\n", palette.c_str());
        this->sleep(2000);
        this->quit();
    }
    fread(pal, 1, 768, palfile);
    fclose(palfile);

    // CPS File laden
    source = NULL;
    source = fopen(filename.c_str(), "rb");
    std::cout << "Opening CPS file: " << filename << '\n';
    if (source == NULL) {
        printf("file not found: %s\n", filename.c_str());
        sleep(2000);
        this->quit();
    }
    printf("CPS FIle found: %s\n", filename.c_str());
    fseek(source, 0, SEEK_END);
    filesize = ftell(source);
    fseek(source, 0, SEEK_SET);

    fread(src, 1, filesize, source);
    fclose(source);

    //Dateigr��e
    filesize = getWord(src, 0) + 2;

    //Komprimierungsmethode
    if (src[2] != 0x04)
        printf("no valid CPS file: %s\n", filename);

    //Bilddaten beginnen bei Position 10
    for (int i = 10; i < 64000; i++)
        srcdata[i - 10] = src[i];

    // SDL Surface erstellen
    SDL_Color colors[256];
    int colorcount = 0;
    for (int i = 0; i < 768; i += 3) {
        //6 Bit Palette der .PAL Files auf 8 Bit Palette portieren und schwarz in weiss umwandeln f�r sp�tere Transparenz
        colors[colorcount].r = pal[i] << 2;
        colors[colorcount].g = pal[i + 1] << 2;
        colors[colorcount].b = pal[i + 2] << 2;
        if (!sprite && transp && colors[colorcount].r == 0 && colors[colorcount].g == 0 && colors[colorcount].b == 0) {
            colors[colorcount].r = 255;
            colors[colorcount].g = 255;
            colors[colorcount].b = 255;
        }
        colorcount++;
    }
    colors[255].r = 1;
    colors[255].g = 1;
    colors[255].b = 1;

    SDL_Surface* tempImage = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 8, 0, 0, 0, 0);
#ifdef SDL_V1
    SDL_SetPalette(tempImage, SDL_LOGPAL | SDL_PHYSPAL, colors, 0, 256);
#endif
    SDL_SetPaletteColors(tempImage->format->palette, colors, 0, 256);
    SDL_Rect dstrect;

    // CPS Daten decodieren
    format80decode(srcdata, dest);

    //Transparent Pixel setzen
    int count = 0;
    for (int h = 0; h < 200; h++) {
        for (int w = 0; w < 320; w++) {
            dstrect.h = 1;
            dstrect.w = 1;
            dstrect.x = w;
            dstrect.y = h;
            if (sprite && colors[dest[count]].r == 0 && colors[dest[count]].g == 0 && colors[dest[count]].b == 0) {
                bool rightfree = true, leftfree = true, topfree = true, bottomfree = true;

                for (int x = 1; x <= 8; x++) {
                    if (count - x >= 0 && count - x <= 63999)
                        if (colors[dest[count - x]].r != 0 || colors[dest[count - x]].g != 0 || colors[dest[count - x]].b != 0)
                            leftfree = false;
                    if (count + x >= 0 && count + x <= 63999)
                        if (colors[dest[count + x]].r != 0 || colors[dest[count + x]].g != 0 || colors[dest[count + x]].b != 0)
                            rightfree = false;
                }
                for (int x = 1; x <= 32; x++) {
                    if (count - (320 * x) >= 0 && count - (320 * x) <= 63999)
                        if (colors[dest[count - (320 * x)]].r != 0 || colors[dest[count - (320 * x)]].g != 0 || colors[dest[count - (320 * x)]].b != 0)
                            topfree = false;
                    if (count + (320 * x) >= 0 && count + (320 * x) <= 63999)
                        if (colors[dest[count + (320 * x)]].r != 0 || colors[dest[count + (320 * x)]].g != 0 || colors[dest[count + (320 * x)]].b != 0)
                            bottomfree = false;
                }

                if (!rightfree && !leftfree && !topfree && !bottomfree)
                    SDL_FillRect(tempImage, &dstrect, 255);
                else
                    SDL_FillRect(tempImage, &dstrect, dest[count]);
            } else
                SDL_FillRect(tempImage, &dstrect, dest[count]);
            count++;
        }
    }

    mSurfaceImages[imageID] = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
    SDL_Rect rcSrc = { posX, posY, width, height };
    SDL_Rect rcDst = { 0, 0, width, height };
    std::cout << "Rendering stuff !!!! " << __FUNCTION__ << '\n';
    mTexture = SDL_CreateTextureFromSurface(mRenderer, mSurfaceImages[imageID]);
    SDL_UpdateTexture(mTexture, NULL, mScreen->pixels, mScreen->pitch);

    SDL_RenderClear(mRenderer);
    SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
    SDL_RenderPresent(mRenderer);
    sleep(2000);
    std::cout << "..................\n";
    SDL_BlitSurface(tempImage, &rcSrc, mSurfaceImages[imageID], &rcDst);
    SDL_FreeSurface(tempImage);

    std::cout << "Leaving " << __FUNCTION__ << '\n';
}

// WORD Wert aus CPS File auslesen
int MediaWrapper::getWord(unsigned char string[64000], int pos)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    return (string[pos + 1] << 8) + string[pos];
    std::cout << "Leaving " << __FUNCTION__ << '\n';
}

int MediaWrapper::format80decode(unsigned char image_in[], unsigned char image_out[])
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    unsigned char* copyp;
    unsigned char* readp = image_in;
    unsigned char* writep = image_out;
    unsigned short code;
    unsigned short count;

    while (1) {
        code = *readp++;
        if (~code & 0x80) {
            //bit 7 = 0
            //command 0 (0cccpppp p): copy
            count = (code >> 4) + 3;
            copyp = writep - (((code & 0xf) << 8) + *readp++);
            while (count--)
                *writep++ = *copyp++;
        } else {
            //bit 7 = 1
            count = code & 0x3f;
            if (~code & 0x40) {
                //bit 6 = 0
                if (!count)
                    //end of image
                    break;
                //command 1 (10cccccc): copy
                while (count--)
                    *writep++ = *readp++;
            } else {
                //bit 6 = 1
                if (count < 0x3e) {
                    //command 2 (11cccccc p p): copy
                    count += 3;

                    copyp = (unsigned char*)&image_out[*(unsigned short*)readp];
                    readp += 2;

                    while (count--)
                        *writep++ = *copyp++;
                } else if (count == 0x3e) {
                    //command 3 (11111110 c c v): fill

                    count = *(unsigned short*)readp;
                    readp += 2;
                    code = *readp++;
                    memset(writep, code, count);
                    writep += count;
                } else {
                    //command 4 (copy 11111111 c c p p): copy

                    count = *(unsigned short*)readp;
                    readp += 2;

                    copyp = (unsigned char*)&image_out[*(unsigned short*)readp];
                    readp += 2;
                    while (count--)
                        *writep++ = *copyp++;
                }
            }
        }
    }
    std::cout << "Leaving " << __FUNCTION__ << '\n';
    return (writep - image_out);
}

//Bild zeichnen
void MediaWrapper::drawImage(int imageID, int posX, int posY, int fromPosX, int fromPosY, int width, int height)
{
    SDL_Rect rcDest = { posX, posY, 0, 0 };
    SDL_Rect rcSrc = { fromPosX, fromPosY, width, height };
    if (width > 0 && height > 0) {
        if (imageID >= 0 && mSurfaceImages[imageID]) {
            //weiss transparent setzen
            SDL_SetColorKey(mSurfaceImages[imageID], SDL_TRUE, SDL_MapRGB(mSurfaceImages[imageID]->format, 255, 255, 255));
            SDL_BlitSurface(mSurfaceImages[imageID], &rcSrc, mScreen, &rcDest);
        }
    } else {
        if (imageID >= 0 && mSurfaceImages[imageID]) {
            //weiss transparent setzen
            SDL_SetColorKey(mSurfaceImages[imageID], SDL_TRUE, SDL_MapRGB(mSurfaceImages[imageID]->format, 255, 255, 255));
            SDL_BlitSurface(mSurfaceImages[imageID], NULL, mScreen, &rcDest);
        }
    }
}

// leeres Bild erstellen
void MediaWrapper::createImage(int imageID, int paletteSourceImageID, int width, int height)
{
    if (mSurfaceImages[imageID] != NULL)
        mSurfaceImages[imageID] = NULL;

    mSurfaceImages[imageID] = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
}

// Bild zu Bild blitten
void MediaWrapper::copyImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int transpR, int transpG, int transpB)
{
    SDL_Rect rcSrc = { fromPosX, fromPosY, width, height };
    SDL_Rect rcDst = { toPosX, toPosY, 0, 0 };
    if (transpR != -1 && transpG != -1 && transpB != -1)
        SDL_SetColorKey(mSurfaceImages[imageFromID], SDL_TRUE, SDL_MapRGB(mSurfaceImages[imageFromID]->format, transpR, transpG, transpB));
    SDL_BlitSurface(mSurfaceImages[imageFromID], &rcSrc, mSurfaceImages[imageToID], &rcDst);
    //SDL_SaveBMP(images[imageToID], "data/out.bmp");
}

// Bild zu Bild blitten mit Gr��en�nderung
bool MediaWrapper::copyScaledImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int toWidth, int toHeight, int transpR, int transpG, int transpB, bool mirror)
{
    if (toWidth != width || toHeight != height || mirror) {
        SDL_Rect from = { 0, 0, 0, 0 };
        SDL_Rect to = { 0, 0, 0, 0 };
        float xfactor = 0;
        float yfactor = 0;
        float temp = 0;
        int tempcount = 0;

        xfactor = (float)((float)(toWidth - 0) / (float)(width - 0));
        yfactor = (float)((float)(toHeight - 0) / (float)(height - 0));

        for (float y = fromPosY; y < fromPosY + height; y++) {
            tempcount = 0;
            if (!mirror) {
                for (float x = fromPosX; x < fromPosX + width; x++) {
                    from.x = x;
                    from.y = y;
                    from.w = 1;
                    from.h = 1;

                    for (int yf = 0; yf < yfactor; yf++)
                        for (int xf = 0; xf < xfactor; xf++) {
                            to.x = toPosX + (x - fromPosX) * xfactor + xf;
                            to.y = toPosY + (y - fromPosY) * yfactor - yf;
                            to.w = 1;
                            to.h = 1;
                            SDL_BlitSurface(mSurfaceImages[imageFromID], &from, mSurfaceImages[imageToID], &to);
                        }
                }
            } else {
                for (float x = fromPosX; x < fromPosX + width; x++) {
                    from.x = fromPosX + width - tempcount++;
                    from.y = y;
                    from.w = 1;
                    from.h = 1;

                    for (int yf = 0; yf < yfactor; yf++)
                        for (int xf = 0; xf < xfactor; xf++) {
                            to.x = toPosX + (x - fromPosX) * xfactor + xf;
                            to.y = toPosY + (y - fromPosY) * yfactor - yf;
                            to.w = 1;
                            to.h = 1;
                            SDL_BlitSurface(mSurfaceImages[imageFromID], &from, mSurfaceImages[imageToID], &to);
                        }
                }
            }
        }
    } else {
        SDL_Rect rcSrc = { fromPosX, fromPosY, width, height };
        SDL_Rect rcDst = { 0, 0, 0, 0 };
        SDL_BlitSurface(mSurfaceImages[imageFromID], &rcSrc, mSurfaceImages[imageToID], &rcDst);
    }

    return true;
}

typedef struct tColorRGBA {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
} tColorRGBA;

// Text zeichen
void MediaWrapper::drawText(int fontID, int posX, int posY, int r, int g, int b, std::string text, bool center)
{
    if (fontID < 4) {
        posY += 1;
        if (center)
            posX = (this->mScreen->w / 2) - (SFont_TextWidth(mFont[0], text.c_str()) / 2);
        if (r == 0 && g == 0 && b == 0)
            SFont_Write(mScreen, mFont[0], posX, posY, text.c_str());
        if (r == 0 && g == 0 && b == 255)
            SFont_Write(mScreen, mFont[1], posX, posY, text.c_str());
        if (r == 255 && g == 255 && b == 255)
            SFont_Write(mScreen, mFont[2], posX, posY, text.c_str());
        if (r == 116 && g == 232 && b == 252)
            SFont_Write(mScreen, mFont[3], posX, posY, text.c_str());
    } else {
        //schwarzen Schatten zeichnen
        SFont_BigWrite(mScreen, mFont[7], posX - 1, posY + 1, text.c_str());

        //Font zeichnen
        SFont_BigWrite(mScreen, mFont[fontID], posX, posY, text.c_str());
    }
}

// SDL beenden
void MediaWrapper::quit()
{
    SDL_Quit();
    exit(0);
}

// x ms warten
void MediaWrapper::sleep(int ms)
{
    SDL_Delay(ms);
}

// Sound laden
void MediaWrapper::loadSound(int nr, std::string path)
{
    char realpath[128];
    sprintf_s(realpath, "%s.ogg", path);
#ifdef SDL_V1
    mSound[nr] = Mix_LoadWAV(realpath);
#endif
}

// Sound l�schen
void MediaWrapper::freeSound(int nr)
{
#ifdef SDL_V1
    if (mSound[nr] != NULL) {
        Mix_FreeChunk(mSound[nr]);
        mSound[nr] = NULL;
    }
#endif
}

// Sound abspielen
void MediaWrapper::playSound(int nr)
{
#ifdef SDL_V1
    Mix_PlayChannel(-1, mSound[nr], 0);
#endif
}

// Sound stoppen
void MediaWrapper::stopSound(int nr)
{
#ifdef SDL_V1
    if (mSound[nr] != NULL)
        Mix_HaltChannel(nr);
#endif
}

// Sound spielt
bool MediaWrapper::isPlaying(int nr)
{
#ifdef SDL_V1
    if (mSound[nr] != NULL)
        return true;
    else
        return false;
#endif
    return false;
}

// Screen Refresh
void MediaWrapper::refresh()
{
    SDL_RenderPresent(mRenderer);
}

// Maus Koordinaten zur�ckgeben
void MediaWrapper::getMouseState(int* x, int* y)
{
    SDL_PumpEvents();
    SDL_GetMouseState(x, y);
}

// pr�fen ob linke Maustaste gedr�ckt wurde
bool MediaWrapper::getMouseLeft()
{
    SDL_PumpEvents();
    if (SDL_GetMouseState(0, 0) & SDL_BUTTON(1))
        return true;
    else
        return false;
}

// pr�fen ob rechte Maustaste gedr�ckt wurde
bool MediaWrapper::getMouseRight()
{
    SDL_PumpEvents();
    if (SDL_GetMouseState(0, 0) & SDL_BUTTON(3))
        return true;
    else
        return false;
}

// Taste down
void MediaWrapper::updateKeys()
{
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            // Tastatur Events
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_ESCAPE:
                this->mEscapeKey = true;
                break;
            case SDLK_DOWN:
                this->mDownKey = true;
                break;
            case SDLK_UP:
                this->mUpKey = true;
                break;
            case SDLK_LEFT:
                this->mLeftKey = true;
                break;
            case SDLK_RIGHT:
                this->mRightKey = true;
                break;
            default:
                break;
            }
            break;
        case SDL_KEYUP:
            this->mDownKey = this->mEscapeKey = this->mLeftKey = this->mRightKey = this->mUpKey = false;
            break;
        }
    }
}

// gef�lltest Viereck zeichnen
void MediaWrapper::fillRect(int posX, int posY, int width, int height, int r, int g, int b, int imageID)
{
    SDL_Rect dstrect = { posX, posY, width, height };
    SDL_FillRect(imageID == -1 ? mScreen : mSurfaceImages[imageID], &dstrect, SDL_MapRGB(imageID == -1 ? mScreen->format : mSurfaceImages[imageID]->format, r, g, b));
}

// Palette laden f�r sp�tere Pixel Aktionen
void MediaWrapper::loadPal(char* filename)
{
    FILE* palfile;
    unsigned char pal[768];
    int filesize;

    // Palette laden
    palfile = NULL;
    palfile = fopen(filename, "rb");
    if (palfile == NULL) {
        printf("palette not found: %s\n", filename);
        this->sleep(2000);
        this->quit();
    }
    fseek(palfile, 0, SEEK_END);
    filesize = ftell(palfile);
    fseek(palfile, 0, SEEK_SET);
    if (filesize != 768) {
        printf("no valid PAL file: %s\n", filename);
        this->sleep(2000);
        this->quit();
    }
    fread(pal, 1, 768, palfile);
    fclose(palfile);

    // SDL Surface erstellen
    int colorcount = 0;
    for (int i = 0; i < 768; i += 3) {
        //6 Bit Palette der .PAL Files auf 8 Bit Palette portieren
        mCurrentColors[colorcount].r = pal[i] << 2;
        mCurrentColors[colorcount].g = pal[i + 1] << 2;
        mCurrentColors[colorcount].b = pal[i + 2] << 2;
        colorcount++;
    }
}

// Pixel zeichnen mit aktueller Palette
void MediaWrapper::drawPixel(int posX, int posY, int palOffset, int imageID = -1)
{
    SDL_Rect dstrect = { posX, posY, 2, 1 };
    SDL_FillRect(imageID == -1 ? mScreen : mSurfaceImages[imageID], &dstrect, SDL_MapRGB(imageID == -1 ? mScreen->format : mSurfaceImages[imageID]->format, mCurrentColors[palOffset].r, mCurrentColors[palOffset].g, mCurrentColors[palOffset].b));
}

// Gamescreen in echten screen surface kopieren und alles anzeigen
void MediaWrapper::updateVideo()
{
    // Skalierung nur n�tig wenn Aufl�sung != screen_game
    if (this->mScreenWidth > 320 && this->mScreenHeight > 200) {
        SDL_Rect from = { 0, 0, 0, 0 };
        SDL_Rect to = { 0, 0, 0, 0 };
        float xfactor = 0;
        float yfactor = 0;
        float temp = 0;

        xfactor = (float)((float)(mScreenWidth - 0) / (float)(mScreenWidth - 0));
        yfactor = (float)((float)(mScreenHeight - 0) / (float)(mScreenHeight - 0));

        for (float y = 0; y < mScreen->h; y++) {
            for (float x = 0; x < mScreen->w; x++) {
                from.x = x;
                from.y = y;
                from.w = 1;
                from.h = 1;

                //screen_game entsprechend der eingestellten Aufl�sung rendern
                for (int yf = 0; yf < yfactor; yf++)
                    for (int xf = 0; xf < xfactor; xf++) {
                        to.x = 0 + x * xfactor + xf;
                        to.y = 0 + y * yfactor - yf;
                        to.w = 1;
                        to.h = 1;
                        SDL_BlitSurface(mScreen, &from, mScreenDisplay, &to); // changed from mWindow to mSurface
                    }
            }
        }
    } else {
        SDL_Rect rcSrc = { 0, 0, mScreen->w, mScreen->h };
        SDL_Rect rcDst = { 0, 0, 0, 0 };
        SDL_BlitSurface(mScreen, &rcSrc, mScreenDisplay, &rcDst);
    }

    mTexture = SDL_CreateTextureFromSurface(mRenderer, mScreen);
    SDL_UpdateTexture(mTexture, NULL, mScreen->pixels, mScreen->pitch);
    SDL_RenderClear(mRenderer);
    SDL_RenderCopy(mRenderer, mTexture, NULL, NULL);
    SDL_RenderPresent(mRenderer);
}

//Gammewert setzen
void MediaWrapper::setGamma(int r, int g, int b)
{
#ifdef SDL_V1
    SDL_SetGamma(r, g, b);
#endif
}

//Millisekunden auslesen
int MediaWrapper::getMilliSeconds()
{
    return SDL_GetTicks();
}
