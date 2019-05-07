#pragma once

#include "character.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"

class Event {
public:
    //Init
    void init(Map* map, MediaWrapper* mediaObject);

    //Events löschen
    void clear();

    //Maus Events verarbeiten
    void mouseState(short posX, short posY);

    //diverse Zeichenoperationen
    void drawEventMessageBackground();
    void drawEventButton(char* text, short posX, short posY);

    //Eventimage laden
    void loadEventImage(short imageID, short image);

    //updaten, Events verarbeiten
    void update();
    void updateStatusMessage();

    //Statustext im Statusfenster ausgeben
    void statusMessage(char text[128]);

    //Media Objekt
    MediaWrapper* mediaObject;

    //Texte
    Language* languageData;

    //Charactere
    Char* chars[4];

    //Event Typ
    short eventTyp;

    //Map Position
    short mapPos;
    short clickedMapPos;

    //Event Forschritt
    short progress;

    //3D Fenster weiterhin anzeigen
    bool showGameWindow;

    //Map
    Map* map;
    bool mapChange;
    short newMapID, warpToPos, faceTo;

    //Maus
    short mousePosX, mousePosY;
    bool mouseClicked;

    //wenn true = Game Pause
    bool eventInProgress;

    //Statustext - 2 Zeilen
    char statusText[2][256];
    char line[256];

    //Timer
    long time, time_change, next_anim_frame;

    //Event Folge
    bool sequenceEvent;
};
