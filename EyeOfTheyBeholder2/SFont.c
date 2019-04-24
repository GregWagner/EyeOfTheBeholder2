/*  SFont: a simple font-library that uses special .pngs as fonts
Copyright (C) 2003 Karl Bartel

License: GPL or LGPL (at your choice)
WWW: http://www.linux-games.com/sfont/

This program is free software; you can redistribute it and/or modify        
it under the terms of the GNU General Public License as published by        
the Free Software Foundation; either version 2 of the License, or           
(at your option) any later version.                                         

This program is distributed in the hope that it will be useful,       
but WITHOUT ANY WARRANTY; without even the implied warranty of              
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the               
GNU General Public License for more details.                

You should have received a copy of the GNU General Public License           
along with this program; if not, write to the Free Software                 
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA   

Karl Bartel
Cecilienstr. 14                                                    
12307 Berlin
GERMANY
karlb@gmx.net                                                      
*/
#include <SDL.h>

#include "SFont.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

static Uint32 GetPixelU32(SDL_Surface* surface, int x, int y)
{
    int bpp = surface->format->BytesPerPixel;
    Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;
    switch (bpp) {
    case 1:
        return *p;
    case 2:
        return *(Uint16*)p;
    case 3:
        if (SDL_BYTEORDER == SDL_BIG_ENDIAN)
            return p[0] << 16 | p[1] << 8 | p[2];
        else
            return p[0] | p[1] << 8 | p[2] << 16;
    case 4:
        return *(Uint32*)p;
    default:
        return 0;
    }
}

static void SetPixelU32(SDL_Surface* screen, int x, int y, Uint32 newcolor)
{
    Uint8* ubuff8;
    Uint16* ubuff16;
    Uint32* ubuff32;
    Uint32 color = newcolor;
    char c1, c2, c3;

    if (SDL_MUSTLOCK(screen))
        if (SDL_LockSurface(screen) < 0)
            return;

    switch (screen->format->BytesPerPixel) {
    case 1:
        ubuff8 = (Uint8*)screen->pixels;
        ubuff8 += (y * screen->pitch) + x;
        *ubuff8 = (Uint8)color;
        break;
    case 2:
        ubuff8 = (Uint8*)screen->pixels;
        ubuff8 += (y * screen->pitch) + (x * 2);
        ubuff16 = (Uint16*)ubuff8;
        *ubuff16 = (Uint16)color;
        break;
    case 3:
        ubuff8 = (Uint8*)screen->pixels;
        ubuff8 += (y * screen->pitch) + (x * 3);
        if (SDL_BYTEORDER == SDL_LIL_ENDIAN) {
            c1 = (color & 0xFF0000) >> 16;
            c2 = (color & 0x00FF00) >> 8;
            c3 = (color & 0x0000FF);
        } else {
            c3 = (color & 0xFF0000) >> 16;
            c2 = (color & 0x00FF00) >> 8;
            c1 = (color & 0x0000FF);
        }
        ubuff8[0] = c3;
        ubuff8[1] = c2;
        ubuff8[2] = c1;
        break;
    case 4:
        ubuff8 = (Uint8*)screen->pixels;
        ubuff8 += (y * screen->pitch) + (x * 4);
        ubuff32 = (Uint32*)ubuff8;
        *ubuff32 = color;
        break;
    default:
        fprintf(stderr, "Error: Unknown bitdepth!\n");
    }
    if (SDL_MUSTLOCK(screen))
        SDL_UnlockSurface(screen);
}

static SDL_Color GetPixelColor(SDL_Surface* surface, int X, int Y)
{
    SDL_Color clr;
    SDL_LockSurface(surface);
    Uint32 pixel = GetPixelU32(surface, X, Y);
    SDL_UnlockSurface(surface);
    SDL_GetRGB(pixel, surface->format, &clr.r, &clr.g, &clr.b);
    return clr;
}

static void SetPixelColor(SDL_Surface* screen, int x, int y, SDL_Color clr)
{
    Uint32 color = SDL_MapRGB(screen->format, clr.r, clr.g, clr.b);
    SetPixelU32(screen, x, y, color);
}

void ReplaceColor(SDL_Surface* surface, SDL_Color oldColor, SDL_Color newColor)
{
    for (int x = 0; x < surface->w; x++) {
        for (int y = 0; y < surface->h; y++) {
            SDL_LockSurface(surface);
            Uint32 pixel = GetPixelU32(surface, x, y);
            SDL_UnlockSurface(surface);
            SDL_Color clr = GetPixelColor(surface, x, y);
            if (clr.r == oldColor.r && clr.g == oldColor.g && clr.b == oldColor.b)
                SetPixelColor(surface, x, y, newColor);
        }
    }
}

static Uint32 GetPixel(SDL_Surface* Surface, Sint32 X, Sint32 Y)
{
    Uint8* bits;
    Uint32 Bpp;

    assert(X >= 0);
    assert(X < Surface->w);

    Bpp = Surface->format->BytesPerPixel;
    bits = ((Uint8*)Surface->pixels) + Y * Surface->pitch + X * Bpp;

    // Get the pixel
    switch (Bpp) {
    case 1:
        return *((Uint8*)Surface->pixels + Y * Surface->pitch + X);
        break;
    case 2:
        return *((Uint16*)Surface->pixels + Y * Surface->pitch / 2 + X);
        break;
    case 3: { // Format/endian independent
        Uint8 r, g, b;
        r = *((bits) + Surface->format->Rshift / 8);
        g = *((bits) + Surface->format->Gshift / 8);
        b = *((bits) + Surface->format->Bshift / 8);
        return SDL_MapRGB(Surface->format, r, g, b);
    } break;
    case 4:
        return *((Uint32*)Surface->pixels + Y * Surface->pitch / 4 + X);
        break;
    }

    return -1;
}

SFont_Font* SFont_InitFont(SDL_Surface* Surface, int r, int g, int b)
{
    int x = 0, i = 0;
    Uint32 pixel;
    SFont_Font* Font;
    Uint32 pink;

    if (Surface == NULL)
        return NULL;

    Font = (SFont_Font*)malloc(sizeof(SFont_Font));
    Font->Surface = Surface;

    SDL_LockSurface(Surface);

    pink = SDL_MapRGB(Surface->format, 255, 0, 255);
    while (x < Surface->w) {
        if (GetPixel(Surface, x, 0) == pink) {
            Font->CharPos[i++] = x;
            while ((x < Surface->w) && (GetPixel(Surface, x, 0) == pink))
                x++;
            Font->CharPos[i++] = x;
        }
        x++;
    }
    Font->MaxPos = x - 1;

    pixel = GetPixel(Surface, 0, Surface->h - 1);
    SDL_UnlockSurface(Surface);
#ifdef TODO
    SDL_SetColorKey(Surface, SDL_SRCCOLORKEY, pixel);
#endif

    SDL_Color oldColor = { 0, 0, 255 };
    SDL_Color newColor = { r, g, b };
    ReplaceColor(Surface, oldColor, newColor);
    return Font;
}

SFont_Font* SFont_InitBigFont(SDL_Surface* Surface, int r, int g, int b)
{
    int x = 0, i = 0;
    Uint32 pixel;
    SFont_Font* Font;
    Uint32 pink;

    if (Surface == NULL)
        return NULL;

    Font = (SFont_Font*)malloc(sizeof(SFont_Font));
    Font->Surface = Surface;

    SDL_LockSurface(Surface);

    pink = SDL_MapRGB(Surface->format, 255, 0, 255);
    while (x < Surface->w) {
        if (GetPixel(Surface, x, 0) == pink) {
            Font->CharPos[i++] = x;
            while ((x < Surface->w) && (GetPixel(Surface, x, 0) == pink))
                x++;
            Font->CharPos[i++] = x;
        }
        x++;
    }
    Font->MaxPos = x - 1;

    pixel = GetPixel(Surface, 0, Surface->h - 1);
    SDL_UnlockSurface(Surface);
#ifdef TODO
    SDL_SetColorKey(Surface, SDL_SRCCOLORKEY, pixel);
#endif

    SDL_Color oldColor = { 0, 0, 255 };
    SDL_Color newColor = { r, g, b };
    ReplaceColor(Surface, oldColor, newColor);

    return Font;
}

SFont_Font* _SFont_InitFont(SDL_Surface* Surface)
{
    int x = 0, i = 0;
    Uint32 pixel;
    SFont_Font* Font;
    Uint32 pink;

    if (Surface == NULL)
        return NULL;

    Font = (SFont_Font*)malloc(sizeof(SFont_Font));
    Font->Surface = Surface;

    SDL_LockSurface(Surface);

    pink = SDL_MapRGB(Surface->format, 255, 0, 255);
    while (x < Surface->w) {
        if (GetPixel(Surface, x, 0) == pink) {
            Font->CharPos[i++] = x;
            while ((x < Surface->w) && (GetPixel(Surface, x, 0) == pink))
                x++;
            Font->CharPos[i++] = x;
        }
        x++;
    }
    Font->MaxPos = x - 1;

    pixel = GetPixel(Surface, 0, Surface->h - 1);
    SDL_UnlockSurface(Surface);
#ifdef TODO
    SDL_SetColorKey(Surface, SDL_SRCCOLORKEY, pixel);
#endif

    return Font;
}

void SFont_FreeFont(SFont_Font* FontInfo)
{
    SDL_FreeSurface(FontInfo->Surface);
    free(FontInfo);
}

void SFont_Write(SDL_Surface* Surface, const SFont_Font* Font,
    int x, int y, const char* text)
{
    const char* c;
    int charoffset;
    SDL_Rect srcrect, dstrect;

    if (text == NULL)
        return;
    y += 2;
    // these values won't change in the loop
    srcrect.y = 1;
    dstrect.y = y;
    srcrect.h = dstrect.h = Font->Surface->h - 1;

    for (c = text; *c != '\0' && x <= Surface->w; c++) {
        charoffset = ((int)(*c - 33)) * 2 + 1;
        // skip spaces and nonprintable characters
        if (*c == ' ' || charoffset < 0 || charoffset > Font->MaxPos) {
            x += (Font->CharPos[2] - Font->CharPos[1]) / 4;
            continue;
        }

        srcrect.w = dstrect.w = (Font->CharPos[charoffset + 2] + Font->CharPos[charoffset + 1]) / 2 - (Font->CharPos[charoffset] + Font->CharPos[charoffset - 1]) / 2;
        srcrect.x = (Font->CharPos[charoffset] + Font->CharPos[charoffset - 1]) / 2;
        dstrect.x = x - (float)(Font->CharPos[charoffset] - Font->CharPos[charoffset - 1]) / 2;

        SDL_BlitSurface(Font->Surface, &srcrect, Surface, &dstrect);

        //feste Schriftbreite von 6 Pixeln
        x += 6;
    }
}

void SFont_BigWrite(SDL_Surface* Surface, const SFont_Font* Font,
    int x, int y, const char* text)
{
    const char* c;
    int charoffset;
    SDL_Rect srcrect, dstrect;

    if (text == NULL)
        return;
    y += 2;
    // these values won't change in the loop
    srcrect.y = 1;
    dstrect.y = y;
    srcrect.h = dstrect.h = Font->Surface->h - 1;

    for (c = text; *c != '\0' && x <= Surface->w; c++) {
        charoffset = ((int)(*c - 33)) * 2 + 1;
        // skip spaces and nonprintable characters
        if (*c == ' ' || charoffset < 0 || charoffset > Font->MaxPos) {
            x += (Font->CharPos[2] - Font->CharPos[1]) / 4;
            continue;
        }

        srcrect.w = dstrect.w = (Font->CharPos[charoffset + 2] + Font->CharPos[charoffset + 1]) / 2 - (Font->CharPos[charoffset] + Font->CharPos[charoffset - 1]) / 2;
        srcrect.x = (Font->CharPos[charoffset] + Font->CharPos[charoffset - 1]) / 2;
        dstrect.x = x - (float)(Font->CharPos[charoffset] - Font->CharPos[charoffset - 1]) / 2;

        SDL_BlitSurface(Font->Surface, &srcrect, Surface, &dstrect);

        //feste Schriftbreite von 8 Pixeln
        x += 8;
    }
}

int SFont_TextWidth(const SFont_Font* Font, const char* text)
{
    const char* c;
    int charoffset = 0;
    int width = 0;

    if (text == NULL)
        return 0;

    for (c = text; *c != '\0'; c++) {
        charoffset = ((int)*c - 33) * 2 + 1;
        // skip spaces and nonprintable characters
        if (*c == ' ' || charoffset < 0 || charoffset > Font->MaxPos) {
            width += (Font->CharPos[2] - Font->CharPos[1]) / 4;
            continue;
        }
        width += Font->CharPos[charoffset + 1] - Font->CharPos[charoffset];
    }

    return width / 4;
}

int SFont_TextHeight(const SFont_Font* Font)
{
    return Font->Surface->h - 1;
}

void SFont_WriteCenter(SDL_Surface* Surface, const SFont_Font* Font,
    int y, const char* text)
{
    SFont_Write(Surface, Font, Surface->w / 2 - SFont_TextWidth(Font, text) / 2,
        y, text);
}
