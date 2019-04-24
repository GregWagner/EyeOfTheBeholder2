#pragma once
#include "char.h"
#include "event.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"

class CMonster {
public:
    //Init
    void init(CMap* gameMap, MEDIAWrapper* mediaObject, CLanguage* languageData, CEvent* mapEvent, short monsterType, short monsterPos, short mapId, short monsterFace, short friendly);

    //updaten
    bool update(long timer, short pos, short destination);
    void updatePos();

    //die verschiedenen Arten der Pfadfindung
    void doPathFinding(short pathType);

    //Hilfsfunktionen: Kollisionsprüfung
    bool checkCollision(short direction);
    bool checkPlayerCollision(short direction, bool doAttack);
    bool checkPlayerInLine(short direction, bool setMoveToPos);

    //prüfen wieviele Monster sich in der Position befinden
    short countMonster(short mapPos);

    //Angriff
    void doAttack();

    //Schaden
    void doDamage(short damage);

    //Media Objekt
    MEDIAWrapper* mediaObject;

    //Texte
    CLanguage* languageData;

    //Charactere
    CChar* character[6];

    //Map
    CMap* gameMap;

    //Events
    CEvent* mapEvent;

    //Position in Karte
    short monsterPos;

    //ID in der Karte
    short staticMapId;

    //Position - links oder rechts wenn zwei Monster an einem Punkt stehen
    short nrInPlace;

    //Position zu welcher gewandert werden soll
    short moveToPos;

    //Gehrichtung
    short face;
    short drawFace;

    //Monstertyp
    short monsterType;

    //Erfahrung welche dieses Monster gibt
    short exp_gain;

    //Feind oder Freund
    bool friendly;

    //Monster bewegt sich oder nicht
    bool isMoving;

    //Trefferpunkte
    short hitPoints;

    //Angriff im Gange
    bool attack;
    short attackFrame;

    //Pathfinding Type
    short currentPathFindingType;

    //Timer
    long currenttime, time_change, attack_time_change;

    //Animationstufe
    bool aniStep, drawMirror;
};
