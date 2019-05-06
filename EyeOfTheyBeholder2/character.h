#pragma once

#include "map.h"
#include "mediawrapper.h"

class CChar {
public:
    //Init
    void init(MEDIAWrapper* mediaObject, CMap* gameMap);

    //Schaden
    void doDamage(short damage);

    //Angriff
    void attackNow(short handPos);

    //Char speichern
    void saveChar(FILE* saveFile);

    //Char laden
    void loadChar(FILE* saveFile);

    //Referenz aufs MediaObject
    MEDIAWrapper* mediaObject;

    //Referenz auf Map
    CMap* gameMap;

    //ID des Chars im Spiel
    short charID;

    //Pos des Chars im Charmenu
    short menuPos;

    //Bild dieses Chars
    short imageID;

    //Items in den Händen des Chars
    short itemHand[2];

    //Items im Inventar
    short itemInventar[14];

    //Kopf Item
    short itemHead;

    //Hals Item
    short itemNeck;

    //Brust Item
    short itemTorso;

    //Handgelenk Item
    short itemWrist;

    //Fuss Item
    short itemFoot;

    //Gürtel Items
    short itemBelt[3];

    //Items in Inventaransicht

    //Name des Chars
    char name[32];

    //Klasse
    char charclass[32];

    //Ausrichtung
    char social[32];

    //Rasse
    char race[32];

    //Geschlecht
    char gender[8];

    //Hitpoints
    short hp;
    short currenthp;

    //Hunger
    short food;
    short currentfood;

    //Stärke
    short strength;

    //max. Stärke
    short maxstrength;

    //Intelligenz
    short intelligence;

    //Wissen
    short wisdom;

    //Geschick
    short dext;

    //Ausdauer
    short constitution;

    //Charisma
    short charisma;

    //Rüstungs Klasse
    short armorclass;

    //Erfahrung
    int exp;

    //Level
    short lvl;

    //Pfeile im Inv.
    short arrows;

    //hat Char Schaden genommen dann true
    bool damageTaken;
    short damageTakenValue;

    //hat Char Schaden verteilt dann true
    bool damageDone[2];
    short damageDoneValue[2];
    short doDamageToMonster;
    short doDamageToMonsterValue;
};
