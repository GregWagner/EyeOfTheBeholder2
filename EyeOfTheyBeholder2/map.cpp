#pragma warning(disable : 4996)

#include "map.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>

// Basis Setup
void Map::loadBaseData(short mapID)
{
    mazeObjects = new MazeObjects();
    mazeObjects->mediaObject = this->mediaObject;
    mazeObjects->init(mapID);
    lastLoadedMapID = mapID;
}

// Wandfront zeichnen - level 4 (ganz hinten) bis 1 (neben Player)
void Map::drawWalls(int level)
{
    for (int i = 0; i < 1024; i++) {
        mazeObjects->cell_info[i].door_is_open = cell_info[i].door_is_open;
        mazeObjects->cell_info[i].is_door = cell_info[i].is_door;
        mazeObjects->cell_info[i].deco_code = cell_info[i].deco_code;
        mazeObjects->cell_info[i].wall_code = cell_info[i].wall_code;
    }
    mazeObjects->renderWalls(level, playerPos, playerFace);
}

//auf solide Wand prüfen für Kollisionserkennung des Players
bool Map::isSolidWall(short direction, short wallpos)
{
    bool value = true;

    //playerFace: 0=N /\, 1=S \/, 2=E >, 3=W <
    if (direction == 0 && mazeObjects->getRealWallID(mazeObjects->maze_data[wallpos].n) == -1)
        value = false;

    if (direction == 1 && mazeObjects->getRealWallID(mazeObjects->maze_data[wallpos].s) == -1)
        value = false;

    if (direction == 2 && mazeObjects->getRealWallID(mazeObjects->maze_data[wallpos].e) == -1)
        value = false;

    if (direction == 3 && mazeObjects->getRealWallID(mazeObjects->maze_data[wallpos].w) == -1)
        value = false;

    if (mazeObjects->cell_info[wallpos].is_door && !mazeObjects->cell_info[wallpos].door_is_open)
        value = true;

    return value;
}

// Initialisierung
void Map::init(short mapID, MediaWrapper* mediaObject, bool fastinit)
{
    std::cout << "Entering " << __FUNCTION__ << '\n';
    /*
		Parameter fastinit bedeutet daß NUR die Bilder der Karte geladen werden müssen
	*/

    // diverse Variablen initialisieren
    this->mediaObject = mediaObject;

    // Default Daten der Karte nur einlesen wenn keine gespeicherte Version existiert (fastinit = true)
    if (!fastinit) {
        char linedata[512];
        char temp[5];
        char tempval[3];
        short arrayPointer = 0;
        short mapdatastart = 0;
        short itemdatastart = 0;
        bool readItemData = false;
        short eventdatastart = 0;
        bool readEventData = false;
        short monsterdatastart = 0;
        bool readMonsterData = false;
        short linecount = 0;
        maxMapVal = 0;

        memoryPosPointer = 0;

        for (short m = 0; m < 64; m++) {
            memoryPos[m] = memoryFace[m] = 0;
        }
        for (short m = 0; m < 128; m++) {
            monsterInfo[m][0] = 0;
            monsterInfo[m][1] = 0;
            monsterInfo[m][2] = 0;
            monsterInfo[m][3] = 0;
            monsterInfo[m][4] = 0;
        }
        for (short i = 0; i < 1089; i++) {
            item_counter[i] = 0;
            mapEvent[i] = 0;
            for (short j = 0; j < 40; j++) {
                item[i][j] = 0;
                item_flying[i][j] = false;
                item_flying_timer[i][j] = 0;
            }
            cell_info[i].deco_code = 0;
            cell_info[i].wall_code = 0;
            cell_info[i].is_door = false;
            cell_info[i].door_is_open = false;
        }
        // Mapfile öffnen
        FILE* file;
        char fileName[256];
        sprintf_s(fileName, "maps/map_%d/map.dat", mapID);

        file = fopen(fileName, "r");
        std::cout << "Opening map file: " << fileName << '\n';
        //Inhalt Zeilenweise einlesen
        while (!feof(file)) {

            fgets(linedata, 512, file);
            if (strlen(linedata) > 1) {
                linedata[strlen(linedata) - 1] = '\0';
                linecount++;
                if (strcmp(linedata, "[MAPSTART]") == 0)
                    mapdatastart = linecount + 1;
                if (strcmp(linedata, "[ITEMDATA]") == 0) {
                    itemdatastart = linecount + 1;
                    readItemData = true;
                }
                if (strcmp(linedata, "[ITEMDATAEND]") == 0)
                    readItemData = false;

                if (strcmp(linedata, "[EVENTDATA]") == 0) {
                    eventdatastart = linecount + 1;
                    readEventData = true;
                }
                if (strcmp(linedata, "[EVENTDATAEND]") == 0)
                    readEventData = false;

                if (strcmp(linedata, "[MONSTERDATA]") == 0) {
                    monsterCount = 0;
                    monsterdatastart = linecount + 1;
                    readMonsterData = true;
                }
                if (strcmp(linedata, "[MONSTERDATAEND]") == 0)
                    readMonsterData = false;

                //Blickwinkel auslesen
                if (linecount == mapdatastart) {
                    strncpy(temp, linedata, 4);
                    this->playerFace = atoi(temp);
                }

                //Startposition auslesen
                if (linecount == mapdatastart + 1) {
                    strncpy(temp, linedata, 4);
                    this->playerPos = atoi(temp);
                }

                temp[0] = '\0';

                //32 Mapreihen auslesen
                if (linecount > 4 && linecount >= (mapdatastart + 2) && linecount <= (mapdatastart + 35)) {
                    for (short i = 0; i <= 128; i += 4) {
                        tempval[0] = linedata[i];
                        tempval[1] = linedata[i + 1];
                        tempval[2] = linedata[i + 2];
                        map[arrayPointer++] = atoi(tempval);
                        tempval[0] = '\0';
                    }
                }

                //Item Positionen einlesen
                if (linecount >= itemdatastart && readItemData) {
                    //Position
                    short pos = 0;
                    for (short i = 0; i < 4; i++)
                        temp[i] = linedata[i];
                    temp[4] = '\0';
                    pos = atoi(temp);
                    tempval[0] = '\0';

                    //ItemID
                    short id = 0;
                    for (short i = 5; i < 8; i++)
                        temp[i - 5] = linedata[i];
                    temp[3] = '\0';
                    id = atoi(temp);
                    temp[0] = '\0';
                    item[pos][item_counter[pos]] = id;

                    //Links oder rechts
                    temp[0] = linedata[9];
                    temp[1] = '\0';
                    item_floorpos[pos][item_counter[pos]] = atoi(temp);
                    temp[0] = '\0';
                    item_counter[pos]++;
                }

                //Events einlesen
                if (linecount >= eventdatastart && readEventData) {
                    //Position
                    short pos = 0;
                    for (short i = 0; i < 4; i++)
                        temp[i] = linedata[i];
                    temp[4] = '\0';
                    pos = atoi(temp);
                    temp[0] = '\0';

                    //EventTyp
                    short typ = 0;
                    for (short i = 5; i < 9; i++)
                        temp[i - 5] = linedata[i];
                    temp[4] = '\0';
                    typ = atoi(temp);
                    temp[0] = '\0';
                    mapEvent[pos] = typ;

                    //Param 0 bis 9
                    for (short p = 0; p < 10; p++) {
                        short param = 0;
                        for (short i = (p * 5) + 10; i < (p * 5) + 14; i++)
                            temp[i - ((p * 5) + 10)] = linedata[i];
                        temp[4] = '\0';
                        param = atoi(temp);
                        temp[0] = '\0';
                        mapEventParam[pos][p] = param;
                    }

                    //Türen als solche markieren
                    if (mapEvent[pos] == 6) {
                        this->cell_info[pos].is_door = true;
                        this->cell_info[pos].door_is_open = false;
                    }

                    //printf("Event: %d %d %d %d %d %d %d %d\n", pos, mapEvent[pos], mapEventParam[pos][0], mapEventParam[pos][1], mapEventParam[pos][2], mapEventParam[pos][3], mapEventParam[pos][4], mapEventParam[pos][5]);
                }

                //Monster einlesen
                if (linecount >= monsterdatastart && readMonsterData) {
                    //Position
                    short pos = 0;
                    for (short i = 0; i < 4; i++)
                        temp[i] = linedata[i];
                    temp[4] = '\0';
                    pos = atoi(temp);
                    temp[0] = '\0';

                    //MonsterTyp
                    short typ = 0;
                    for (short i = 5; i < 9; i++)
                        temp[i - 5] = linedata[i];
                    temp[4] = '\0';
                    typ = atoi(temp);
                    temp[0] = '\0';

                    //Monster Blickrichtung
                    short face = 0;
                    for (short i = 10; i < 14; i++)
                        temp[i - 10] = linedata[i];
                    temp[4] = '\0';
                    face = atoi(temp);
                    temp[0] = '\0';

                    //feind-oder-freund
                    short friendly = 0;
                    for (short i = 15; i < 19; i++)
                        temp[i - 15] = linedata[i];
                    temp[4] = '\0';
                    friendly = atoi(temp);
                    temp[0] = '\0';

                    monsterInfo[monsterCount][0] = pos;
                    monsterInfo[monsterCount][1] = typ;
                    monsterInfo[monsterCount][2] = monsterCount;
                    monsterInfo[monsterCount][3] = face;
                    monsterInfo[monsterCount][4] = friendly;
                    monsterCount++;

                    //printf("%d: %d\n", monsterInfo[pos][0], monsterInfo[pos][1]);
                }
            }
            linedata[0] = '\0';
        }
        fclose(file);

        mapCols = 32;
    }

    loadBaseData(mapID);
    std::cout << "Leaving " << __FUNCTION__ << '\n';
}

// Karte zurücksetzen
void Map::clear()
{
    playerFace = 0;
    playerPos = 0;
    mapOffset = 0;
    monsterCount = 0;
    memoryPosPointer = 0;

    for (short i = 0; i < 1089; i++) {
        map[i] = 0;
        item_counter[i] = 0;
        for (short j = 0; j < 40; j++) {
            item[i][j] = 0;
            item_floorpos[i][j] = 0;
            item_flying[i][j] = 0;
            item_flying_direction[i][j] = 0;
            item_flying_timer[i][j] = 0;
        }
    }

    for (short i = 0; i < 256; i++) {
        mapBG[i] = 0;
    }

    for (short i = 0; i < 4096; i++) {
        mapEvent[i] = 0;
        for (short j = 0; j < 10; j++)
            mapEventParam[i][j] = 0;
    }

    for (short i = 0; i < 128; i++) {
        for (short j = 0; j < 5; j++)
            monsterInfo[i][j] = 0;
    }

    for (short i = 0; i < 64; i++) {
        memoryPos[i] = 0;
        memoryFace[i] = 0;
    }

    mediaObject->freeImage(163);
    mediaObject->freeImage(164);

    for (short i = 0; i < 212; i++) {
        mediaObject->freeImage(300 + i);
    }
}

// Karte speichern
void Map::saveMap(FILE* saveFile)
{
    //Blickrichtung des Spielers speichern
    fwrite(&playerFace, sizeof(short), 1, saveFile);

    //Position des Spielers speichern
    fwrite(&playerPos, sizeof(short), 1, saveFile);

    //Start-Nr der Kartenbilder
    fwrite(&mapOffset, sizeof(short), 1, saveFile);

    //Anzahl unterschiedlicher Wände
    fwrite(&maxMapVal, sizeof(short), 1, saveFile);

    //Anzahl der Monster in dieser Karte
    fwrite(&monsterCount, sizeof(short), 1, saveFile);

    for (short i = 0; i < 1089; i++) {
        //Wandtypen dieser Karte
        fwrite(&map[i], sizeof(short), 1, saveFile);

        //Anzahl der Items dieser Karte
        fwrite(&item_counter[i], sizeof(short), 1, saveFile);

        for (short j = 0; j < 40; j++) {
            //Item Eigenschaften
            fwrite(&item[i][j], sizeof(short), 1, saveFile);
            fwrite(&item_floorpos[i][j], sizeof(short), 1, saveFile);
            fwrite(&item_flying[i][j], sizeof(bool), 1, saveFile);
            fwrite(&item_flying_direction[i][j], sizeof(short), 1, saveFile);
            fwrite(&item_flying_timer[i][j], sizeof(long), 1, saveFile);
        }

        fwrite(&cell_info[i].door_is_open, sizeof(bool), 1, saveFile);
        fwrite(&cell_info[i].is_door, sizeof(bool), 1, saveFile);
        fwrite(&cell_info[i].deco_code, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].wall_code, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space04, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space05, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space06, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space07, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space08, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space09, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space10, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space11, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space12, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space13, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space14, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space15, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space16, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space17, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space18, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space19, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space20, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space21, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space22, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space23, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space24, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space25, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space26, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space27, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space28, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space29, sizeof(unsigned char), 1, saveFile);
        fwrite(&cell_info[i].empty_space30, sizeof(unsigned char), 1, saveFile);
    }

    for (short i = 0; i < 256; i++) {
        //Karten Hintergrund Bild
        fwrite(&mapBG[i], sizeof(short), 1, saveFile);
    }

    for (short i = 0; i < 4096; i++) {
        //Events der Karte
        fwrite(&mapEvent[i], sizeof(short), 1, saveFile);

        for (short j = 0; j < 10; j++)
            //Event Parameter der Karte
            fwrite(&mapEventParam[i][j], sizeof(short), 1, saveFile);
    }

    for (short i = 0; i < 128; i++) {
        for (short j = 0; j < 5; j++)
            //Parameter der Monster dieser Karte
            fwrite(&monsterInfo[i][j], sizeof(short), 1, saveFile);
    }
}

// Karte laden
void Map::loadMap(FILE* saveFile)
{
    //Blickrichtung des Spielers lesen
    fread(&playerFace, sizeof(short), 1, saveFile);

    //Position des Spielers lesen
    fread(&playerPos, sizeof(short), 1, saveFile);

    //Start-Nr der Kartenbilder
    fread(&mapOffset, sizeof(short), 1, saveFile);

    //Anzahl unterschiedlicher Wände
    fread(&maxMapVal, sizeof(short), 1, saveFile);

    //Anzahl der Monster in dieser Karte
    fread(&monsterCount, sizeof(short), 1, saveFile);

    for (short i = 0; i < 1089; i++) {
        //Wandtypen dieser Karte
        fread(&map[i], sizeof(short), 1, saveFile);

        //Anzahl der Items dieser Karte
        fread(&item_counter[i], sizeof(short), 1, saveFile);

        for (short j = 0; j < 40; j++) {
            //Item Eigenschaften
            fread(&item[i][j], sizeof(short), 1, saveFile);
            fread(&item_floorpos[i][j], sizeof(short), 1, saveFile);
            fread(&item_flying[i][j], sizeof(bool), 1, saveFile);
            fread(&item_flying_direction[i][j], sizeof(short), 1, saveFile);
            fread(&item_flying_timer[i][j], sizeof(long), 1, saveFile);
        }

        fread(&cell_info[i].door_is_open, sizeof(bool), 1, saveFile);
        fread(&cell_info[i].is_door, sizeof(bool), 1, saveFile);
        fread(&cell_info[i].deco_code, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].wall_code, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space04, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space05, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space06, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space07, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space08, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space09, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space10, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space11, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space12, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space13, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space14, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space15, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space16, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space17, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space18, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space19, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space20, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space21, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space22, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space23, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space24, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space25, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space26, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space27, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space28, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space29, sizeof(unsigned char), 1, saveFile);
        fread(&cell_info[i].empty_space30, sizeof(unsigned char), 1, saveFile);
    }

    for (short i = 0; i < 256; i++) {
        //Karten Hintergrund Bild
        fread(&mapBG[i], sizeof(short), 1, saveFile);
    }

    for (short i = 0; i < 4096; i++) {
        //Events der Karte
        fread(&mapEvent[i], sizeof(short), 1, saveFile);

        for (short j = 0; j < 10; j++)
            //Event Parameter der Karte
            fread(&mapEventParam[i][j], sizeof(short), 1, saveFile);
    }

    for (short i = 0; i < 128; i++) {
        for (short j = 0; j < 5; j++)
            //Parameter der Monster dieser Karte
            fread(&monsterInfo[i][j], sizeof(short), 1, saveFile);
    }

    mapCols = 32;
    memoryPosPointer = 0;
}

// Funktion fügt aktuelle Position zum memoryPos Array hinzu
void Map::addPosToMemory()
{
    short temp = 0, temp2 = 0;

    //Array push
    for (short m = 63; m >= 0; m--) {
        temp = memoryPos[m];
        temp2 = memoryFace[m];
        if (m < 63) {
            memoryPos[m + 1] = temp;
            memoryFace[m + 1] = temp2;
        }
    }
    memoryPos[0] = this->playerPos;
    memoryFace[0] = this->playerFace;
}

//Item werfen
void Map::throwItem(short mapPos, short itemNr)
{
    if (mediaObject->getMilliSeconds() > item_flying_timer[mapPos][itemNr]) {
        short mapOffset = 0;

        switch (item_flying_direction[mapPos][itemNr]) {
        case 0:
            mapOffset = -32;
            break;
        case 1:
            mapOffset = +32;
            break;
        case 2:
            mapOffset = +1;
            break;
        case 3:
            mapOffset = -1;
            break;
        default:
            break;
        }

        bool monster_collision = false;
        for (short m = 0; m < monsterCount; m++) {
            if (monsterInfo[m][0] == (mapPos + mapOffset)) {
                monster_collision = true;
            }
        }

        if (isSolidWall(item_flying_direction[mapPos][itemNr], mapPos + mapOffset) == 0 && !monster_collision) {
            //Item-Setup im nächsten Feld
            for (short i = 0; i < 40; i++) {
                if (item[mapPos + mapOffset][i] == 0) {
                    item_counter[mapPos + mapOffset]++;
                    item[mapPos + mapOffset][i] = item[mapPos][itemNr];
                    item_floorpos[mapPos + mapOffset][i] = item_floorpos[mapPos][itemNr];
                    item_flying[mapPos + mapOffset][i] = true;
                    item_flying_timer[mapPos + mapOffset][i] = mediaObject->getMilliSeconds() + 500;
                    item_flying_direction[mapPos + mapOffset][i] = item_flying_direction[mapPos][itemNr];

                    //Item in diesem Feld löschen
                    item[mapPos][itemNr] = 0;
                    item_floorpos[mapPos][itemNr] = 0;
                    item_flying_timer[mapPos][itemNr] = 0;
                    item_counter[mapPos]--;
                }
            }
        } else {
            item_flying[mapPos][itemNr] = false;
            item_flying_timer[mapPos][itemNr] = 0;
        }
    }
}
