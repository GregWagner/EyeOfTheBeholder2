#pragma once

class CMap {
public:
    //Init
    void init(short mapID, MEDIAWrapper* mediaObject, bool fastinit);

    //Wandfront zeichnen - level 4 (ganz hinten) bis 1 (neben Player)
    void drawWalls(int level);

    //auf solide Wand prüfen für Kollisionserkennung
    bool isSolidWall(short direction, short wallpos);

    //Karte zurücksetzen
    void clear();

    //Karte speichern
    void saveMap(FILE* saveFile);

    //Karte laden
    void loadMap(FILE* saveFile);

    //Basis Setup
    void loadBaseData(short mapID);

    //Funktion fügt aktuelle Position zum memoryPos Array hinzu
    void addPosToMemory();

    //Object mit Grafiken und Infos zum Leveldesign
    CMazeObjects* mazeObjects;

    //Item werfen
    void throwItem(short mapPos, short itemNr);

    // Variable um Ausrichtung des Spielers zu speichern
    short playerFace; //0=N, 1=S, 2=O, 3=W

    // Variable um Position des Spielers auf einer Map zu speichern
    short playerPos;

    // in der Variable map wird die Karte gespeichert
    short map[1089];
    short mapCols;
    short maxMapVal;

    //Event - und sonstige Infos zu den einzelnen Zellen der Karte
    struct cellInformation {
        bool door_is_open; //Tür - offen/geschlossen
        bool is_door; //Zelle enthält eine Tür
        unsigned char deco_code; //Decoration ID für diese Zelle überschreibt alle anderen Dekos
        unsigned char wall_code; //Wall ID für diese Zelle überschreibt Wall ID des .maz Files dieser Zelle
        unsigned char empty_space04; //Platzhalter für spätere Werte
        unsigned char empty_space05; //Platzhalter für spätere Werte
        unsigned char empty_space06; //Platzhalter für spätere Werte
        unsigned char empty_space07; //Platzhalter für spätere Werte
        unsigned char empty_space08; //Platzhalter für spätere Werte
        unsigned char empty_space09; //Platzhalter für spätere Werte
        unsigned char empty_space10; //Platzhalter für spätere Werte
        unsigned char empty_space11; //Platzhalter für spätere Werte
        unsigned char empty_space12; //Platzhalter für spätere Werte
        unsigned char empty_space13; //Platzhalter für spätere Werte
        unsigned char empty_space14; //Platzhalter für spätere Werte
        unsigned char empty_space15; //Platzhalter für spätere Werte
        unsigned char empty_space16; //Platzhalter für spätere Werte
        unsigned char empty_space17; //Platzhalter für spätere Werte
        unsigned char empty_space18; //Platzhalter für spätere Werte
        unsigned char empty_space19; //Platzhalter für spätere Werte
        unsigned char empty_space20; //Platzhalter für spätere Werte
        unsigned char empty_space21; //Platzhalter für spätere Werte
        unsigned char empty_space22; //Platzhalter für spätere Werte
        unsigned char empty_space23; //Platzhalter für spätere Werte
        unsigned char empty_space24; //Platzhalter für spätere Werte
        unsigned char empty_space25; //Platzhalter für spätere Werte
        unsigned char empty_space26; //Platzhalter für spätere Werte
        unsigned char empty_space27; //Platzhalter für spätere Werte
        unsigned char empty_space28; //Platzhalter für spätere Werte
        unsigned char empty_space29; //Platzhalter für spätere Werte
        unsigned char empty_space30; //Platzhalter für spätere Werte
    } cell_info[1089];

    //Setup - welcher KartenID ist eine Tür
    bool isDoor[256];

    //Hintergrund einer Tilegrafik, pro Grafikset kann ein anderes BG gewählt werden
    short mapBG[256];

    //Basegrafikset eines Tiles
    short mapOffset;
    short lastLoadedMapID;

    //pro Karte können 40 Items auf dem Boden liegen
    short item[1089][40];
    short item_counter[1089];
    short item_floorpos[1089][40];
    bool item_flying[1089][40];
    short item_flying_direction[1089][40];
    long item_flying_timer[1089][40];

    //Events, pro Position 1 Event mit 10 Parametern möglich
    short mapEvent[4096];
    short mapEventParam[4096][10];

    //Monster, Position, Typ, Blickrichtung, feind-oder-freund
    short monsterInfo[128][5];
    short monsterCount;

    //Array mit den letzten Positionen des Spielers und seiner Ausrichtung
    short memoryPos[64];
    short memoryFace[64];
    short memoryPosPointer;

    //Media Objekt
    MEDIAWrapper* mediaObject;
};
