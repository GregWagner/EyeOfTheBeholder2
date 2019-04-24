#pragma once

#include "char.h"
#include "charmenu.h"
#include "config.h"
#include "event.h"
#include "intro.h"
#include "item.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"
#include "monster.h"

class C3DEngine {
public:
    //allgemeine Debug-Funktion zum Testen diverser Aktionen
    void test();

    //Fenster erstellen und anzeigen
    void initViewArea();

    //Karte laden
    void loadMap(short mapID, bool savedMap = false);

    //Intro spielen
    bool playIntro();

    //3D Karte anzeigen, Renderloop
    void run();

    //3D Ansicht neu zeichnen
    void updateView();

    //3D Wände aufbauen
    void drawStuffWindow(int level);
    void drawGameWindowNew();

    //Kollision prüfen
    bool checkWallCollision(short direction);
    bool checkMonsterCollision(short direction);

    //Stuff zeichnen
    void drawStuff(short mapWall, short wNr);

    //Monster zeichnen
    void drawMonster(short mapWall, short wNr);

    //Kompass zeichnen
    void drawCompass();

    //Tastatur Events
    void keyDown();
    void keyUp();
    void keyLeft();
    void keyRight();

    //Maus Events
    void mouseState();
    void mouseMove();
    void handleGameWindowClick(int mousePosX, int mousePosY);

    //Spiel speichern
    void saveGame();

    //Spiel laden
    void loadGame();

    //Steuerung für MainLoop
    short running;

    //Objekt für Konfigurationsdaten
    CConfig configuration;

    //Intro
    CIntro intro;

    //Karte
    CMap* gameMap;
    CMap* savestatus_gameMap[33]; //Kartenstatus für max. 16 Karten sichern beim Kartenwechsel
    short mapOffset;
    short currentMapID;

    //Karten Events
    CEvent* mapEvent;

    //Media Objekt
    MEDIAWrapper mediaObject;

    //Instanz für Character Menus
    CCharMenu charmenu;

    //Instanz der Sprachdaten
    CLanguage languageData;

    //4 Charachter Instanzen
    CChar character[4];

    //128 Instanzen für Monster
    CMonster* monster[128];

    //Items
    CItem item;

    //Item des Mauszeigers
    short mouseItem, mouseItemOld;

    //X,Y und Breite,Höhe des eigentlichen 3D Fensters
    short posX, posY, width, height;

    // Variable um anzugeben welcher Hintergrund gezeichnet wird
    bool drawBG;

    //Offset des BG Imaages
    short bgOffset;

    //speichern ob Taste geradegedrückt wurde
    bool keyPressed;

    //Variablen für Maus Events
    bool buttonLeft, buttonRight;
    bool buttonPressed;
    int mousePosX, realMouseX;
    int mousePosY, realMouseY;
};
