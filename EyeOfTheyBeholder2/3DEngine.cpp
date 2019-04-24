#include "3DEngine.h"

#include "char.h"
#include "config.h"
#include "event.h"
#include "item.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"
#include <cstdio>
#include <cstdlib>

//
// 3D window initialisation, loading of bitmaps eg.
//

void C3DEngine::initViewArea()
{
    //allg. Setup
    for (short i = 0; i < 32; i++)
        savestatus_gameMap[i] = nullptr;
    gameMap = nullptr;
    mapEvent = new CEvent;

    //Konfiguration laden
    configuration.openConf("spectalum.conf");

    //Media Wrapper
    if (configuration.resolution == 0)
        mediaObject.setupWindow(0, 0, 320, 200, configuration.fullscreen);
    else
        mediaObject.setupWindow(0, 0, 640, 400, configuration.fullscreen);

    //Loading Screen
    mediaObject.fillRect(0, 0, 320, 240, 0, 0, 0);
    mediaObject.drawText(0, 20, 20, 255, 255, 255, "...LOADING...", true);
    mediaObject.drawText(0, 20, 40, 255, 255, 255, "ENGINE ... ", false);
    mediaObject.refresh();

    //Engine Setup
    keyPressed = false;
    buttonLeft = false;
    buttonRight = false;
    buttonPressed = false;
    mousePosX = 0;
    mousePosY = 0;
    mouseItem = 0;
    mediaObject.loadCPS(150, "original/PLAYFLD.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mediaObject.loadCPS(165, "original/DECORATE.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mediaObject.loadCPS(166, "original/THROWN.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mediaObject.drawText(0, 140, 40, 255, 255, 255, "OK", false);
    mediaObject.drawText(0, 20, 50, 255, 255, 255, "LANGUAGE DATA ... ", false);
    mediaObject.refresh();
    languageData.init(configuration.language);
    mediaObject.drawText(0, 140, 50, 255, 255, 255, "OK", false);
    mediaObject.refresh();

    //Items initialisieren
    mediaObject.drawText(0, 20, 60, 255, 255, 255, "ITEM DATA ... ", false);
    mediaObject.refresh();
    item.init(&mediaObject, mapEvent, &languageData);
    mediaObject.drawText(0, 140, 60, 255, 255, 255, "OK", false);
    mediaObject.refresh();

    //die 4 Charactere initialisieren
    mediaObject.drawText(0, 20, 70, 255, 255, 255, "CHARACTER DATA ...", false);
    mediaObject.refresh();
    character[0].init(&mediaObject, gameMap);
    character[0].menuPos = 0;
    character[0].charID = 0;
    character[0].imageID = 0;
    sprintf_s(character[0].name, "%s", "SHADUM");
    sprintf_s(character[0].charclass, "%s", "PALADIN");
    sprintf_s(character[0].social, "%s", "LAWFUL GOOD");
    sprintf_s(character[0].race, "%s", "HUMAN");
    sprintf_s(character[0].gender, "%s", "MALE");
    character[0].hp = character[0].currenthp = 100;
    character[0].food = character[0].currentfood = 100;
    character[0].strength = 18;
    character[0].maxstrength = 80;
    character[0].intelligence = 9;
    character[0].wisdom = 16;
    character[0].dext = 15;
    character[0].constitution = 17;
    character[0].charisma = 17;
    character[0].armorclass = 1;
    character[0].exp = 69000;
    character[0].lvl = 6;
    character[0].arrows = 0;
    character[0].itemHand[0] = 1;
    character[0].itemHand[1] = 23;
    character[1].init(&mediaObject, gameMap);
    character[1].menuPos = 1;
    character[1].charID = 1;
    character[1].imageID = 1;
    sprintf_s(character[1].name, "%s", "ZORG");
    sprintf_s(character[1].charclass, "%s", "FIGHTER");
    sprintf_s(character[1].social, "%s", "TRUE NEUTRAL");
    sprintf_s(character[1].race, "%s", "DWARF");
    sprintf_s(character[1].gender, "%s", "MALE");
    character[1].hp = character[1].currenthp = 60;
    character[1].food = character[1].currentfood = 100;
    character[1].strength = 18;
    character[1].maxstrength = 80;
    character[1].intelligence = 10;
    character[1].wisdom = 6;
    character[1].dext = 17;
    character[1].constitution = 19;
    character[1].charisma = 3;
    character[1].armorclass = 0;
    character[1].exp = 69000;
    character[1].lvl = 6;
    character[1].arrows = 0;
    character[1].itemHand[0] = 1;
    character[1].itemHand[1] = 23;
    character[2].init(&mediaObject, gameMap);
    character[2].menuPos = 2;
    character[2].charID = 2;
    character[2].imageID = 2;
    sprintf_s(character[2].name, "%s", "MERLIN");
    sprintf_s(character[2].charclass, "%s", "CLERIC");
    sprintf_s(character[2].social, "%s", "CHAOTIC GOOD");
    sprintf_s(character[2].race, "%s", "HUMAN");
    sprintf_s(character[2].gender, "%s", "MALE");
    character[2].hp = character[2].currenthp = 80;
    character[2].food = character[2].currentfood = 100;
    character[2].strength = 15;
    character[2].maxstrength = 80;
    character[2].intelligence = 18;
    character[2].wisdom = 10;
    character[2].dext = 19;
    character[2].constitution = 11;
    character[2].charisma = 15;
    character[2].armorclass = 6;
    character[2].exp = 69000;
    character[2].lvl = 7;
    character[2].arrows = 0;
    character[2].itemHand[0] = 1;
    character[2].itemHand[1] = 23;
    character[3].init(&mediaObject, gameMap);
    character[3].menuPos = 3;
    character[3].charID = 3;
    character[3].imageID = 31;
    sprintf_s(character[3].name, "%s", "VALERIA");
    sprintf_s(character[3].charclass, "%s", "MAGE");
    sprintf_s(character[3].social, "%s", "GOOD");
    sprintf_s(character[3].race, "%s", "ELF");
    sprintf_s(character[3].gender, "%s", "FEMALE");
    character[3].hp = character[3].currenthp = 50;
    character[3].food = character[3].currentfood = 100;
    character[3].strength = 12;
    character[3].maxstrength = 80;
    character[3].intelligence = 11;
    character[3].wisdom = 18;
    character[3].dext = 16;
    character[3].constitution = 16;
    character[3].charisma = 13;
    character[3].armorclass = 1;
    character[3].exp = 69000;
    character[3].lvl = 7;
    character[3].arrows = 0;
    character[3].itemHand[0] = 1;
    character[3].itemHand[1] = 23;
    mediaObject.drawText(0, 140, 70, 255, 255, 255, "OK", false);
    mediaObject.refresh();

    // Inventar, Character und Stats Menu initialisieren
    mediaObject.drawText(0, 20, 80, 255, 255, 255, "INVENTORY ...", false);
    mediaObject.refresh();
    for (short i = 0; i < 4; i++)
        charmenu.character[i] = &character[i];
    charmenu.languageData = &languageData;
    charmenu.initMenu(&mediaObject, mapEvent);
    charmenu.item = &this->item;
    charmenu.gameMap = this->gameMap;
    charmenu.mouseItem = &this->mouseItem;

    for (short i = 0; i < 4; i++)
        mapEvent->chars[i] = &character[i];
    mediaObject.drawText(0, 140, 80, 255, 255, 255, "OK", false);
    mediaObject.refresh();

    //Sounds laden
    mediaObject.drawText(0, 20, 90, 255, 255, 255, "SOUNDS ...", false);
    mediaObject.refresh();
    mediaObject.loadSound(2, "sound/hit");
    mediaObject.loadSound(3, "sound/miss");
    mediaObject.drawText(0, 140, 90, 255, 255, 255, "OK", false);
    mediaObject.refresh();

    //Monster Setup
    for (short m = 0; m < 128; m++)
        monster[m] = nullptr;

    mediaObject.drawText(0, 20, 140, 255, 255, 255, "PLEASE WAIT ...", false);
    mediaObject.refresh();
}

//
// Funktion zum Testen diverser Aktionen
//

void C3DEngine::test()
{
}

//
// Intro abspielen
//
bool C3DEngine::playIntro()
{
    //Intro abspielen
    intro.languageData = languageData;
    intro.init(&mediaObject);
    intro.play();
    return true;
}

//
// Karte laden mit allen Grafiken und Koordinaten usw.
//

void C3DEngine::loadMap(short mapID, bool savedMap = false)
{
    //existiert Karte als Statusbackup dieses jetzt initialisieren
    if (savestatus_gameMap[mapID] != nullptr && !savedMap) {
        this->currentMapID = mapID;
        if (gameMap == nullptr)
            gameMap = new CMap;
        *gameMap = *savestatus_gameMap[mapID];
        gameMap->init(mapID, &mediaObject, true);
    }
    //Karte wurde aus Spielstand geladen
    else if (savedMap) {
        gameMap->init(mapID, &mediaObject, true);
    }
    //ansonsten Karte normal neu laden
    else {
        //Karte initialisieren
        this->currentMapID = mapID;
        if (gameMap != nullptr) {
            gameMap->clear();
        }
        gameMap = nullptr;
        delete (gameMap);
        gameMap = new CMap;
        gameMap->init(mapID, &mediaObject);
    }

    //Zeiger der Karte an alle Objekt weiterleiten die diesen benötigen
    charmenu.gameMap = gameMap;
    for (short i = 0; i < 4; i++)
        character[i].gameMap = gameMap;

    //Monster initialisieren
    for (int i = 0; i < 149; i++)
        mediaObject.freeImage(i);
    for (short m = 0; m < gameMap->monsterCount; m++) {
        if (monster[m] != nullptr) {
            delete (monster[m]);
            monster[m] = nullptr;
        }
        monster[m] = new CMonster;
        monster[m]->init(gameMap, &mediaObject, &languageData, mapEvent, gameMap->monsterInfo[m][1], gameMap->monsterInfo[m][0], gameMap->monsterInfo[m][2], gameMap->monsterInfo[m][3], gameMap->monsterInfo[m][4]);
        for (short i = 0; i < 4; i++)
            monster[m]->character[i] = &character[i];
    }

    //Events initialisieren
    if (mapEvent != nullptr) {
        mapEvent->clear();
        delete (mapEvent);
        mapEvent = nullptr;
    }
    mapEvent = new CEvent;
    for (short i = 0; i < 4; i++)
        mapEvent->chars[i] = &character[i];
    mapEvent->init(gameMap, &mediaObject);
    mapEvent->languageData = &languageData;
    mapEvent->statusMessage(languageData.text[41]);
    charmenu.mapEvent = mapEvent;
}

//
// Spiel laden
//

void C3DEngine::loadGame()
{
    //Savefile öffnen
    FILE* saveFile = fopen("spectalum.sav", "rb");

    //Header lesen
    char header[32];
    fread(&header, 32, 1, saveFile);

    //mapID lesen
    fread(&currentMapID, sizeof(short), 1, saveFile);

    //Spieler Position
    short tempPlayerPos = 0;
    fread(&tempPlayerPos, sizeof(short), 1, saveFile);

    //Spieler Blickrichtung
    short tempPlayerFace = 0;
    fread(&tempPlayerFace, sizeof(short), 1, saveFile);

    //GameMap Objekt
    if (gameMap) {
        gameMap->clear();
        gameMap = nullptr;
        delete (gameMap);
        gameMap = new CMap;
    }
    gameMap->loadMap(saveFile);

    //Anzahl der bereits besuchten Karten
    unsigned char mapSaved[33];
    fread(&mapSaved, 33 * sizeof(unsigned char), 1, saveFile);

    //bereits besuchte Karten
    for (short i = 1; i < 32; i++) {
        if (mapSaved[i] == 1) {
            if (savestatus_gameMap[i] != nullptr)
                savestatus_gameMap[i]->clear();
            savestatus_gameMap[i] = nullptr;
            delete (savestatus_gameMap[i]);
            savestatus_gameMap[i] = new CMap;
            savestatus_gameMap[i]->loadMap(saveFile);
        }
    }

    //Chars
    for (short i = 0; i < 4; i++) {
        character[i].loadChar(saveFile);
    }

    //Savefile schliessen
    fclose(saveFile);

    //Karte initialisieren
    loadMap(this->currentMapID, true);
    gameMap->playerPos = tempPlayerPos;
    gameMap->playerFace = tempPlayerFace;
}

//
// Spiel speichern
//

void C3DEngine::saveGame()
{
    //Savefile öffnen
    FILE* saveFile = fopen("spectalum.sav", "wb");

    //Header schreiben
    char header[32] = "SPECTALUM SAVEGAME";
    fwrite(&header, 32, 1, saveFile);

    //mapID
    fwrite(&currentMapID, sizeof(short), 1, saveFile);

    //Spieler Position
    fwrite(&gameMap->playerPos, sizeof(short), 1, saveFile);

    //Spieler Blickrichtung
    fwrite(&gameMap->playerFace, sizeof(short), 1, saveFile);

    //Gamemap
    gameMap->saveMap(saveFile);

    //Anzahl der bereits besuchten Karten
    unsigned char mapSaved[33];
    for (int i = 0; i < 33; i++)
        mapSaved[i] = 0;
    for (short i = 1; i <= 32; i++) {
        if (savestatus_gameMap[i] != nullptr)
            mapSaved[i] = 1;
        else
            mapSaved[i] = 0;
    }
    fwrite(&mapSaved, 33 * sizeof(unsigned char), 1, saveFile);
    for (short i = 1; i < 32; i++) {
        if (mapSaved[i] == 1)
            savestatus_gameMap[i]->saveMap(saveFile);
    }

    //Chars
    for (short i = 0; i < 4; i++)
        character[i].saveChar(saveFile);

    //Savefile schliessen
    fclose(saveFile);

    mapEvent->statusMessage(languageData.text[195]);
}

//
// 3D Fenster neu zeichnen
//

void C3DEngine::updateView()
{

    mediaObject.fillRect(0, 0, 3200, 200, 0, 0, 0);

    //auf "Spiel laden" action prüfen
    if (charmenu.loadGame) {
        charmenu.loadGame = false;
        loadGame();
    }

    //auf "Spiel speichern" action prüfen
    if (charmenu.saveGame) {
        charmenu.saveGame = false;
        saveGame();
    }

    if (drawBG) {
        mediaObject.drawImage(163, 0, 0, 0, 0, 176, 120);
    } else {
        mediaObject.drawImage(164, 0, 0, 0, 0, 176, 120);
    }

    //
    // Spielefenster zeichnen
    //

    drawGameWindowNew();

    //
    // Hintergrund zeichnen
    //

    mediaObject.drawImage(150, 0, 0);

    //
    // Kompass zeichnen
    //

    drawCompass();

    //
    // Character Menus zeichnen
    //

    charmenu.update();

    //
    // Statustext updaten
    //

    mapEvent->updateStatusMessage();

    if (mapEvent->eventInProgress) {
        //
        // Hintergrund zeichnen
        //

        mediaObject.drawImage(150, 0, 0);

        //
        // Character Menus zeichnen
        //

        charmenu.update();
        //Event Ereignisse updaten und zeichnen
        mapEvent->update();

        if (mapEvent->showGameWindow) {
            if (drawBG) {
                mediaObject.drawImage(163, 0, 0, 0, 0, 176, 120);
            } else {
                mediaObject.drawImage(164, 0, 0, 0, 0, 176, 120);
            }
            drawGameWindowNew();
        }

        //auf Kartenwechsel prüfen
        if (mapEvent->mapChange) {
            //Kartenstatus sichern
            savestatus_gameMap[this->currentMapID] = nullptr;
            savestatus_gameMap[this->currentMapID] = new CMap;
            *savestatus_gameMap[this->currentMapID] = *gameMap;

            mapEvent->mapChange = false;
            short mapID = mapEvent->newMapID;
            short warpToPos = mapEvent->warpToPos;
            short faceTo = mapEvent->faceTo;

            //Monster löschen
            for (short m = 0; m < 128; m++) {
                if (monster[m] != nullptr) {
                    delete (monster[m]);
                    monster[m] = nullptr;
                }
            }

            //Events löschen
            mapEvent->clear();
            mapEvent = nullptr;
            delete (mapEvent);

            //Karte löschen und aktuellen Status sichern
            gameMap->clear();
            delete (gameMap);
            gameMap = nullptr;

            //neue Karte laden
            loadMap(mapID);

            //Spieler-Position setzen
            if (warpToPos > 0) {
                gameMap->playerPos = warpToPos;
                gameMap->playerFace = faceTo;
            }
        }
    }

    int count_w = 0;

    //Mauszeiger zeichnen
    item.drawIcon(this->mouseItem, this->realMouseX, this->realMouseY);

    //Refresh
    mediaObject.refresh();

#ifdef WINCE
//
#else
    //Bildschirmrefresh
    mediaObject.updateVideo();
#endif
}

//
// Monster updaten und zeichnen
//

void C3DEngine::drawMonster(short mapWall, short wNr)
{
    for (short m = 0; m < gameMap->monsterCount; m++) {
        monster[m]->update(mediaObject.getMilliSeconds(), mapWall, wNr);
        gameMap->monsterInfo[m][0] = monster[m]->monsterPos;
    }
}

//
//Funktion um Gegenstände in Umgebungsbildschirm zu zeichnen
//

void C3DEngine::drawStuff(short mapWall, short wNr)
{
    /*
		wNr: 0 bis 11
		-1: aktueller Standpunkt - w:64, h:24 bzw. w:32, h:24 - x0,y96 und x110,y96
		0: ganz vorn links
		1: ganz vorn rechts
		2: 1 feld entfernt links
		3: 1 feld entfernt mitte - w:45, h:17 bzw. w:22, h:17 - 36,85 und 95,85
		4: 1 feld entfernt rechts
		5: 2 felder entfernt links
		6: 2 felder entfernt mitte - w:32, h:13 bzw. w:17, h:13 - 50, 65 und 92, 65
		7: 2 felder entfernt rechts
		8: ungenutzt
		9: 3 felder entfernt links
		10: 3 felder entfernt mitte - w:26, h:10 bzw. w:13, h:10 - 64, 53 und 86, 53
		11: 3 felder entfernt rechts
		12: ungenutzt
	*/

    //
    // Items
    //
    short imageID = 0;
    bool left = false;
    short fromPosX = 0, fromPosY = 0;
    short width = 0, height = 0;
    short toPosX = 0, toPosY = 0;
    short yOffset = 0;

    for (short i = 0; i < 40; i++) {
        yOffset = 0;
        if (gameMap->item[mapWall][i] > 0 && gameMap->item[mapWall][i] < 112) {
            //printf("map %d: %d, %d\n", mapWall, item.largePicID[gameMap->item[mapWall][i]], wNr);
            //prüfen welches Image genommen wird: ITEMS1.CPS (0-30), dann ITEML1.CPS (31-45)
            imageID = item.largePicID[gameMap->item[mapWall][i]] < 31 ? 211 : 212;
            if (imageID == 211) {
                fromPosX = (item.largePicID[gameMap->item[mapWall][i]] * 32) % 128;
                fromPosY = (item.largePicID[gameMap->item[mapWall][i]] / 4) * 24;
            } else if (imageID == 212) {
                fromPosX = ((item.largePicID[gameMap->item[mapWall][i]] - 31) * 64) % 128;
                fromPosY = ((item.largePicID[gameMap->item[mapWall][i]] - 31) / 2) * 24;
            }

            //links oder rechts
            left = gameMap->item_floorpos[mapWall][i] == 1 ? true : false;
            if (gameMap->playerFace == 1 || gameMap->playerFace == 2)
                left = !left;

            //werfen
            if (gameMap->item_flying[mapWall][i])
                gameMap->throwItem(mapWall, i);

            //zeichnen
            switch (wNr) {
            case -1:
                if (imageID == 211) {
                    width = 32;
                    height = 24;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 64;
                    height = 24;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 45;
                if (left) {
                    mediaObject.drawImage(255, 0, 96 - yOffset);
                } else {
                    mediaObject.drawImage(255, 110, 96 - yOffset);
                }
                mediaObject.freeImage(255);
                break;
            case 0:
                break;
            case 1:
                break;
            case 2:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 40;
                if (!left) {
                    mediaObject.drawImage(255, 36 - 55, 85 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            case 3:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 40;
                if (left) {
                    mediaObject.drawImage(255, 36, 85 - yOffset);
                } else {
                    mediaObject.drawImage(255, 95, 85 - yOffset);
                }
                mediaObject.freeImage(255);
                break;
            case 4:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 40;
                if (left) {
                    mediaObject.drawImage(255, 95 + 55, 85 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            case 5:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 32;
                if (!left) {
                    mediaObject.drawImage(255, 52 - 45, 65 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            case 6:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 32;
                if (left) {
                    mediaObject.drawImage(255, 50, 65 - yOffset);
                } else {
                    mediaObject.drawImage(255, 92, 65 - yOffset);
                }
                mediaObject.freeImage(255);
                break;
            case 7:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 32;
                if (left) {
                    mediaObject.drawImage(255, 92 + 45, 65 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            case 8:
                break;
            case 9:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 25;
                if (!left) {
                    mediaObject.drawImage(255, 86 - 55, 53 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            case 10:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 25;
                if (left) {
                    mediaObject.drawImage(255, 64, 53 - yOffset);
                } else {
                    mediaObject.drawImage(255, 86, 53 - yOffset);
                }
                mediaObject.freeImage(255);
                break;
            case 11:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mediaObject.createImage(255, imageID, width, height);
                    mediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (gameMap->item_flying[mapWall][i])
                    yOffset = 25;
                if (left) {
                    mediaObject.drawImage(255, 84 + 55, 53 - yOffset);
                }
                mediaObject.freeImage(255);

                break;
            default:
                break;
            }
        }
    }

    //Monster
    drawMonster(mapWall, wNr);
}

//
//Funktion um den Kompass zu zeichnen
//

void C3DEngine::drawCompass()
{
    if (gameMap->playerFace == 0) {
        mediaObject.drawImage(165, 114, 132, 0, 120, 21, 17);
        mediaObject.drawImage(165, 79, 158, 0, 137, 17, 10);
        mediaObject.drawImage(165, 150, 158, 0, 147, 17, 10);
    }

    else if (gameMap->playerFace == 1) {
        mediaObject.drawImage(165, 114, 132, 48, 120, 21, 17);
        mediaObject.drawImage(165, 79, 158, 48, 137, 17, 10);
        mediaObject.drawImage(165, 150, 158, 48, 147, 17, 10);
    }

    else if (gameMap->playerFace == 2) {
        mediaObject.drawImage(165, 114, 132, 24, 120, 21, 17);
        mediaObject.drawImage(165, 79, 158, 24, 137, 17, 10);
        mediaObject.drawImage(165, 150, 158, 24, 147, 17, 10);
    }

    else if (gameMap->playerFace == 3) {
        mediaObject.drawImage(165, 114, 132, 71, 120, 21, 17);
        mediaObject.drawImage(165, 79, 158, 72, 137, 17, 10);
        mediaObject.drawImage(165, 150, 158, 72, 147, 17, 10);
    }

    else {
        mediaObject.drawImage(165, 114, 132, 0, 120, 21, 17);
    }
}

//
// Rendern des Umgebungsfensters
//

void C3DEngine::drawGameWindowNew()
{
    //Wände Ebene 1 zeichnen - ganz hinten
    gameMap->drawWalls(1);
    drawStuffWindow(1);

    //Wände Ebene 2 zeichnen - hinten
    gameMap->drawWalls(2);
    drawStuffWindow(2);

    //Wände Ebene 3 zeichnen - vor Player
    gameMap->drawWalls(3);
    drawStuffWindow(3);

    //Wände Ebene 4 zeichnen - neben Player
    gameMap->drawWalls(4);
    drawStuffWindow(4);
}

//
// Rendern des Items und Monster
//

void C3DEngine::drawStuffWindow(int level)
{
    short root, wallNrRoot, wallNr;
    if (gameMap->playerFace == 0) {
        if (level == 1) {
            //the east and west wall of BB
            wallNrRoot = wallNr = gameMap->playerPos - ((3 * gameMap->mapCols) + 1);
            drawStuff(wallNr, 9);

            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 11);

            //the north wall of DD, EE, FF
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            //the east and west wall of EE
            wallNrRoot = wallNr = gameMap->playerPos - ((2 * gameMap->mapCols) + 1);
            drawStuff(wallNr, 5);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 7);

            //the north wall of GG, HH, II
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            //the east and west wall of HH
            wallNrRoot = wallNr = gameMap->playerPos - ((1 * gameMap->mapCols) + 1);
            drawStuff(wallNr, 2);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 4);

            //the north wall of JJ, KK, LL
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNrRoot = wallNr = gameMap->playerPos - 1;
            drawStuff(wallNr, 0);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 1);
        }
    }

    if (gameMap->playerFace == 1) {
        if (level == 1) {
            wallNrRoot = wallNr = gameMap->playerPos + ((3 * gameMap->mapCols) + 1);
            drawStuff(wallNr, 9);
            wallNr = wallNrRoot - 2;

            drawStuff(wallNr, 11);

            wallNr = wallNrRoot - 1;

            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            wallNrRoot = wallNr = gameMap->playerPos + ((2 * gameMap->mapCols) + 1);

            drawStuff(wallNr, 5);
            wallNr = wallNrRoot - 2;

            drawStuff(wallNr, 7);
            wallNr = wallNrRoot - 1;

            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNrRoot = wallNr = gameMap->playerPos + ((1 * gameMap->mapCols) + 1);

            drawStuff(wallNr, 2);
            wallNr = wallNrRoot - 2;

            drawStuff(wallNr, 4);
            wallNr = wallNrRoot - 1;

            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNrRoot = wallNr = gameMap->playerPos + 1;

            drawStuff(wallNr, 0);
            wallNr = wallNrRoot - 2;

            drawStuff(wallNr, 1);
        }
    }

    if (gameMap->playerFace == 3) {
        if (level == 1) {
            wallNr = gameMap->playerPos + 1 + (gameMap->mapCols - 4);

            drawStuff(wallNr, 9);
            wallNr = gameMap->playerPos + 1 - (gameMap->mapCols + 4);

            drawStuff(wallNr, 11);

            wallNr = gameMap->playerPos + 1 - 4;

            drawStuff(wallNr, 10);
        }

        if (level == 2) {

            wallNr = gameMap->playerPos + 2 + (gameMap->mapCols - 4);

            drawStuff(wallNr, 5);
            wallNr = gameMap->playerPos + 2 - (gameMap->mapCols + 4);

            drawStuff(wallNr, 7);

            wallNr = gameMap->playerPos + 2 - 4;

            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNr = gameMap->playerPos + 3 + (gameMap->mapCols - 4);

            drawStuff(wallNr, 2);
            wallNr = gameMap->playerPos + 3 - (gameMap->mapCols + 4);

            drawStuff(wallNr, 4);

            wallNr = gameMap->playerPos + 3 - 4;

            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNr = gameMap->playerPos + 4 + (gameMap->mapCols - 4);

            drawStuff(wallNr, 0);
            wallNr = gameMap->playerPos + 4 - (gameMap->mapCols + 4);

            drawStuff(wallNr, 1);
        }
    }

    if (gameMap->playerFace == 2) {
        if (level == 1) {
            wallNr = gameMap->playerPos - 1 - (gameMap->mapCols - 4);

            drawStuff(wallNr, 9);
            wallNr = gameMap->playerPos - 1 + (gameMap->mapCols + 4);

            drawStuff(wallNr, 11);

            wallNr = gameMap->playerPos - 1 + 4;

            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            wallNr = gameMap->playerPos - 2 - (gameMap->mapCols - 4);

            drawStuff(wallNr, 5);
            wallNr = gameMap->playerPos - 2 + (gameMap->mapCols + 4);

            drawStuff(wallNr, 7);

            wallNr = gameMap->playerPos - 2 + 4;

            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNr = gameMap->playerPos - 3 - (gameMap->mapCols - 4);

            drawStuff(wallNr, 2);
            wallNr = gameMap->playerPos - 3 + (gameMap->mapCols + 4);

            drawStuff(wallNr, 4);

            wallNr = gameMap->playerPos - 3 + 4;

            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNr = gameMap->playerPos - 4 - (gameMap->mapCols - 4);

            drawStuff(wallNr, 0);
            wallNr = gameMap->playerPos - 4 + (gameMap->mapCols + 4);

            drawStuff(wallNr, 1);
        }
    }

    //wurde keine Wand gezeichnet trotzdem den "Stuff" der aktuellen Position zeichnen
    drawStuff(gameMap->playerPos, -1);
}

//
// Kollision mit einem Monster prüfen
//

bool C3DEngine::checkMonsterCollision(short direction)
{
    bool collision = false;

    for (short m = 0; m < gameMap->monsterCount; m++) {
        //0=N, 1=S, 2=E, 3=W
        switch (direction) {
        case 0:
            switch (gameMap->playerFace) {
            case 0:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - gameMap->mapCols))
                    collision = true;
                break;
            case 1:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + gameMap->mapCols))
                    collision = true;
                break;
            case 2:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + 1))
                    collision = true;
                break;
            case 3:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - 1))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (gameMap->playerFace) {
            case 0:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + gameMap->mapCols))
                    collision = true;
                break;
            case 1:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - gameMap->mapCols))
                    collision = true;
                break;
            case 2:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - 1))
                    collision = true;
                break;
            case 3:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + 1))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (gameMap->playerFace) {
            case 0:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - 1))
                    collision = true;
                break;
            case 1:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + 1))
                    collision = true;
                break;
            case 2:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - gameMap->mapCols))
                    collision = true;
                break;
            case 3:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + gameMap->mapCols))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (gameMap->playerFace) {
            case 0:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + 1))
                    collision = true;
                break;
            case 1:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - 1))
                    collision = true;
                break;
            case 2:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos + gameMap->mapCols))
                    collision = true;
                break;
            case 3:
                if (gameMap->monsterInfo[m][0] == (gameMap->playerPos - gameMap->mapCols))
                    collision = true;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }

    return collision;
}

//
// Kollision mit einer Wand zu prüfen
//

bool C3DEngine::checkWallCollision(short direction)
{
    bool collision = false;
    //
    // Bewegung nach vorn - prüfen auf Hinderniss
    //

    if (direction == 0) {

        if (gameMap->playerFace == 0) {
            if (gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos - gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 1) {
            if (gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos + gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 3) {
            if (gameMap->mapEvent[gameMap->playerPos - 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - 1];
                mapEvent->mapPos = gameMap->playerPos - 1;
                mapEvent->eventInProgress = true;
            }

            if (gameMap->isSolidWall(0, gameMap->playerPos - 1))
                collision = true;
        }
        if (gameMap->playerFace == 2) {
            if (gameMap->mapEvent[gameMap->playerPos + 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + 1];
                mapEvent->mapPos = gameMap->playerPos + 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + 1))
                collision = true;
        }
    }

    //
    // Bewegung nach hinten - prüfen auf Hinderniss
    //

    if (direction == 1) {
        if (gameMap->playerFace == 0) {
            if (gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos + gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 1) {
            if (gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos - gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 3) {
            if (gameMap->mapEvent[gameMap->playerPos + 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + 1];
                mapEvent->mapPos = gameMap->playerPos + 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + 1))
                collision = true;
        }
        if (gameMap->playerFace == 2) {
            if (gameMap->mapEvent[gameMap->playerPos - 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - 1];
                mapEvent->mapPos = gameMap->playerPos - 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - 1))
                collision = true;
        }
    }

    //
    // Bewegung nach links (Strafe left) - prüfen auf Hinderniss
    //

    if (direction == 2) {
        if (gameMap->playerFace == 0) {
            if (gameMap->mapEvent[gameMap->playerPos - 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - 1];
                mapEvent->mapPos = gameMap->playerPos - 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - 1))
                collision = true;
        }
        if (gameMap->playerFace == 1) {
            if (gameMap->mapEvent[gameMap->playerPos + 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + 1];
                mapEvent->mapPos = gameMap->playerPos + 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + 1))
                collision = true;
        }
        if (gameMap->playerFace == 3) {
            if (gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos + gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 2) {
            if (gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos - gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - gameMap->mapCols))
                collision = true;
        }
    }

    //
    // Bewegung nach rechts (Strafe right) - prüfen auf Hinderniss
    //

    if (direction == 3) {
        if (gameMap->playerFace == 0) {
            if (gameMap->mapEvent[gameMap->playerPos + 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + 1];
                mapEvent->mapPos = gameMap->playerPos + 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + 1))
                collision = true;
        }
        if (gameMap->playerFace == 1) {
            if (gameMap->mapEvent[gameMap->playerPos - 1] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - 1];
                mapEvent->mapPos = gameMap->playerPos - 1;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - 1))
                collision = true;
        }
        if (gameMap->playerFace == 3) {
            if (gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos - gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos - gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos - gameMap->mapCols))
                collision = true;
        }
        if (gameMap->playerFace == 2) {
            if (gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols] > 0) {
                mapEvent->eventTyp = gameMap->mapEvent[gameMap->playerPos + gameMap->mapCols];
                mapEvent->mapPos = gameMap->playerPos + gameMap->mapCols;
                mapEvent->eventInProgress = true;
            }
            if (gameMap->isSolidWall(0, gameMap->playerPos + gameMap->mapCols))
                collision = true;
        }
    }

    if (!collision)
        //auf Kollision mit Monster prüfen
        collision = checkMonsterCollision(direction);

    return collision;
}

//
// auf Tastendruch "rechts" reagieren
//

void C3DEngine::keyRight()
{
    //
    //	Hintergrundbild wechseln
    //
    gameMap->mazeObjects->p_shift++;
    drawBG = drawBG ? false : true;

    if (gameMap->playerFace == 0)
        gameMap->playerFace = 2;

    else if (gameMap->playerFace == 2)
        gameMap->playerFace = 1;

    else if (gameMap->playerFace == 1)
        gameMap->playerFace = 3;

    else if (gameMap->playerFace == 3)
        gameMap->playerFace = 0;

    keyPressed = true;
}

//
// auf Tastendruch "links" reagieren
//

void C3DEngine::keyLeft()
{
    //
    //	Hintergrundbild wechseln
    //
    gameMap->mazeObjects->p_shift--;
    drawBG = drawBG ? false : true;

    if (gameMap->playerFace == 0)
        gameMap->playerFace = 3;

    else if (gameMap->playerFace == 3)
        gameMap->playerFace = 1;

    else if (gameMap->playerFace == 1)
        gameMap->playerFace = 2;

    else if (gameMap->playerFace == 2)
        gameMap->playerFace = 0;

    keyPressed = true;
}

//
// auf Tastendruch "hoch" reagieren
//

void C3DEngine::keyUp()
{
    //
    //	Hintergrundbild wechseln
    //

    gameMap->mazeObjects->p_shift2++;
    drawBG = drawBG ? false : true;

    if (gameMap->playerFace == 0 && !checkWallCollision(0)) {
        gameMap->addPosToMemory();
        gameMap->playerPos -= gameMap->mapCols;
    }

    if (gameMap->playerFace == 1 && !checkWallCollision(0)) {
        gameMap->addPosToMemory();
        gameMap->playerPos += gameMap->mapCols;
    }

    if (gameMap->playerFace == 3 && !checkWallCollision(0)) {
        gameMap->addPosToMemory();
        gameMap->playerPos--;
    }

    if (gameMap->playerFace == 2 && !checkWallCollision(0)) {
        gameMap->addPosToMemory();
        gameMap->playerPos++;
    }

    if (checkWallCollision(0)) {
        //
        // Hintergrundbildwechsel rückgängig machen da keine Bewegung erfolgen darf
        //

        drawBG = drawBG ? false : true;
    }

    keyPressed = true;
}

//
// auf Tastendruch "runter" reagieren
//

void C3DEngine::keyDown()
{
    //
    //	Hintergrundbild wechseln
    //
    gameMap->mazeObjects->p_shift2--;
    drawBG = drawBG ? false : true;

    if (gameMap->playerFace == 0 && !checkWallCollision(1)) {
        gameMap->addPosToMemory();
        gameMap->playerPos += gameMap->mapCols;
    }

    if (gameMap->playerFace == 1 && !checkWallCollision(1)) {
        gameMap->addPosToMemory();
        gameMap->playerPos -= gameMap->mapCols;
    }

    if (gameMap->playerFace == 3 && !checkWallCollision(1)) {
        gameMap->addPosToMemory();
        gameMap->playerPos++;
    }

    if (gameMap->playerFace == 2 && !checkWallCollision(1)) {
        gameMap->addPosToMemory();
        gameMap->playerPos--;
    }

    if (checkWallCollision(1)) {
        //
        // Hintergrundbildwechsel rückgängig machen da keine Bewegung erfolgen darf
        //

        drawBG = drawBG ? false : true;
    }

    keyPressed = true;
}

//
// auf Mausklicks reagieren
//

void C3DEngine::mouseState()
{
    // Events updaten
    mediaObject.getMouseState(&mousePosX, &mousePosY);
    this->realMouseX = this->mousePosX;
    this->realMouseY = this->mousePosY;

    //640x400
    if (configuration.resolution == 1) {
        mousePosX /= 2;
        mousePosY /= 2;
        this->realMouseX = this->mousePosX;
        this->realMouseY = this->mousePosY;
    }

    if (!buttonPressed && mediaObject.getMouseLeft()) {
        buttonLeft = buttonPressed = true;
        buttonRight = false;
    }
    if (!buttonPressed && mediaObject.getMouseRight()) {
        buttonRight = buttonPressed = true;
        buttonLeft = false;
    } else if (buttonPressed && !mediaObject.getMouseLeft() && !mediaObject.getMouseRight())
        buttonPressed = false;

    //aktuelle Mauskoordinaten an die Charmenu Instanz übergeben
    charmenu.mouseX = this->realMouseX;
    charmenu.mouseY = this->realMouseY;

    //
    // Events verarbeiten
    //
    mapEvent->mousePosX = mousePosX;
    mapEvent->mousePosY = mousePosY;

    if (mapEvent->eventInProgress) {
        if (buttonLeft)
            mapEvent->mouseState(mousePosX, mousePosY);
    } else {
        //Bewegung der Figur
        if (buttonLeft)
            mouseMove();

        //Klicks an Charmenu weiter reichen
        if (buttonLeft)
            charmenu.mouseState(mousePosX, mousePosY, true);
        if (buttonRight)
            charmenu.mouseState(mousePosX, mousePosY, false);

        //Klicks im 3D Fenster nur verarbeiten wenn Spiel-Menu geschlossen ist
        if (charmenu.menuStyle == 0) {
            //Klicks im 3D Fenster verarbeiten
            if (buttonLeft && mousePosX >= 0 && mousePosX <= 176 && mousePosY >= 0 && mousePosY <= 120)
                handleGameWindowClick(mousePosX, mousePosY);

            //prüfen ob auf eine Wand o.ä. geklickt wurde und ob ein Event ausgelöst werden muss
            if (buttonLeft && mousePosX >= 10 && mousePosX <= 170 && mousePosY >= 8 && mousePosY <= 93) {
                //ID der angeklickten Wand herausbekommen
                short wallToCheck = gameMap->playerPos;
                if (gameMap->playerFace == 0)
                    wallToCheck -= gameMap->mapCols;

                if (gameMap->playerFace == 1)
                    wallToCheck += gameMap->mapCols;

                if (gameMap->playerFace == 3)
                    wallToCheck--;

                if (gameMap->playerFace == 2)
                    wallToCheck++;

                //Event auslösen wenn vorhanden
                if (gameMap->mapEvent[wallToCheck] > 0) {
                    mapEvent->eventTyp = gameMap->mapEvent[wallToCheck];
                    mapEvent->clickedMapPos = wallToCheck;
                    mapEvent->eventInProgress = true;
                }
                //printf("%d\n", wallToCheck);
            }
        } else
            buttonLeft = buttonRight = false;
    }

    //wurde ein Angriff eines Chars ausgelöst dann hier den Schaden an den Gegner weiterleiten
    for (short i = 0; i < 4; i++) {
        if (character[i].doDamageToMonsterValue > 0) {
            monster[character[i].doDamageToMonster]->doDamage(character[i].doDamageToMonsterValue);
            character[i].doDamageToMonsterValue = 0;
            character[i].doDamageToMonster = -1;
        }
    }

    //Buttonclicks wieder löschen
    buttonLeft = buttonRight = false;
}

//
// Klicks im 3D Fenster verarbeiten
//

void C3DEngine::handleGameWindowClick(int mousePosX, int mousePosY)
{

    //
    //Items links und rechts am unteren Bildschirmrand aufheben/ablegen
    //

    mouseItemOld = mouseItem;

    //links
    if (mousePosX >= 0 && mousePosX <= 88 && mousePosY <= 120) {

        //es wird kein Item in der Maus "gehalten"
        if (this->mouseItem == 0) {
            //prüfen ob im aktuellen Standpunkt an dieser Stelle Items existieren und diese aufheben
            for (short i = 99; i >= 0; i--) {
                if (gameMap->item[gameMap->playerPos][i] > 0 && gameMap->item[gameMap->playerPos][i] < 112)
                    if ((gameMap->playerFace == 0 || gameMap->playerFace == 3) && gameMap->item_floorpos[gameMap->playerPos][i] == 1) {
                        this->mouseItem = gameMap->item[gameMap->playerPos][i];
                        gameMap->item[gameMap->playerPos][i] = 0;
                        gameMap->item_counter[gameMap->playerPos]--;
                        break;
                    } else if ((gameMap->playerFace == 1 || gameMap->playerFace == 2) && gameMap->item_floorpos[gameMap->playerPos][i] == 2) {
                        this->mouseItem = gameMap->item[gameMap->playerPos][i];
                        gameMap->item[gameMap->playerPos][i] = 0;
                        gameMap->item_counter[gameMap->playerPos]--;
                        break;
                    }
            }
        } else {
            for (short i = 0; i < 40; i++) {
                //Item ablegen
                if (gameMap->item[gameMap->playerPos][i] == 0) {
                    //Item werfen
                    if (mousePosY < 60) {
                        gameMap->item_flying_direction[gameMap->playerPos][i] = gameMap->playerFace;
                        gameMap->item_flying[gameMap->playerPos][i] = true;
                    } else
                        gameMap->item_flying[gameMap->playerPos][i] = false;

                    if (gameMap->playerFace == 0 || gameMap->playerFace == 3) {
                        gameMap->item[gameMap->playerPos][i] = this->mouseItem;
                        gameMap->item_floorpos[gameMap->playerPos][i] = 1;
                        gameMap->item_counter[gameMap->playerPos]++;
                        this->mouseItem = 0;
                    } else if (gameMap->playerFace == 1 || gameMap->playerFace == 2) {
                        gameMap->item[gameMap->playerPos][i] = this->mouseItem;
                        gameMap->item_floorpos[gameMap->playerPos][i] = 2;
                        gameMap->item_counter[gameMap->playerPos]++;
                        this->mouseItem = 0;
                    }
                }
            }
        }
    }

    //rechts
    else if (mousePosX >= 89 && mousePosX <= 177 && mousePosY <= 120) {
        //es wird kein Item in der Maus "gehalten"
        if (this->mouseItem == 0) {
            //prüfen ob im aktuellen Standpunkt an dieser Stelle Items existieren und diese aufheben
            for (short i = 99; i >= 0; i--) {
                if (gameMap->item[gameMap->playerPos][i] > 0 && gameMap->item[gameMap->playerPos][i] < 112)
                    if ((gameMap->playerFace == 0 || gameMap->playerFace == 3) && gameMap->item_floorpos[gameMap->playerPos][i] == 2) {
                        this->mouseItem = gameMap->item[gameMap->playerPos][i];
                        gameMap->item[gameMap->playerPos][i] = 0;
                        gameMap->item_counter[gameMap->playerPos]--;
                        break;
                    } else if ((gameMap->playerFace == 1 || gameMap->playerFace == 2) && gameMap->item_floorpos[gameMap->playerPos][i] == 1) {
                        this->mouseItem = gameMap->item[gameMap->playerPos][i];
                        gameMap->item[gameMap->playerPos][i] = 0;
                        gameMap->item_counter[gameMap->playerPos]--;
                        break;
                    }
            }
        } else {
            for (short i = 0; i < 40; i++) {
                //Item ablegen
                if (gameMap->item[gameMap->playerPos][i] == 0) {
                    //Item werfen
                    if (mousePosY < 60) {
                        gameMap->item_flying_direction[gameMap->playerPos][i] = gameMap->playerFace;
                        gameMap->item_flying[gameMap->playerPos][i] = true;
                    } else
                        gameMap->item_flying[gameMap->playerPos][i] = false;

                    if (gameMap->playerFace == 0 || gameMap->playerFace == 3) {
                        gameMap->item[gameMap->playerPos][i] = this->mouseItem;
                        gameMap->item_floorpos[gameMap->playerPos][i] = 2;
                        gameMap->item_counter[gameMap->playerPos]++;
                        this->mouseItem = 0;
                    } else if (gameMap->playerFace == 1 || gameMap->playerFace == 2) {
                        gameMap->item[gameMap->playerPos][i] = this->mouseItem;
                        gameMap->item_floorpos[gameMap->playerPos][i] = 1;
                        gameMap->item_counter[gameMap->playerPos]++;
                        this->mouseItem = 0;
                    }
                }
            }
        }
    }

    if (mouseItemOld != mouseItem && mouseItem != 0) {
        char temp[256];
        sprintf_s(temp, "%s %s", item.itemName[mouseItem], languageData.text[42]);
        mapEvent->statusMessage(temp);
    }
}

//
// Bewegung der Party
//

void C3DEngine::mouseMove()
{
    //nach links rotieren
    if (mousePosX >= 5 && mousePosX <= 24 && mousePosY >= 128 && mousePosY <= 145)
        keyLeft();

    //nach rechts rotieren
    else if (mousePosX >= 44 && mousePosX <= 64 && mousePosY >= 128 && mousePosY <= 145)
        keyRight();

    //vorwärts
    else if (mousePosX >= 24 && mousePosX <= 44 && mousePosY >= 128 && mousePosY <= 145)
        keyUp();

    //rückwärts
    else if (mousePosX >= 24 && mousePosX <= 44 && mousePosY >= 145 && mousePosY <= 162)
        keyDown();

    //strafe left
    else if (mousePosX >= 5 && mousePosX <= 24 && mousePosY >= 145 && mousePosY <= 162) {
        //
        //	Hintergrundbild wechseln
        //

        drawBG = drawBG ? false : true;

        if (gameMap->playerFace == 0 && !checkWallCollision(2)) {
            gameMap->addPosToMemory();
            gameMap->playerPos -= 1;
        }

        if (gameMap->playerFace == 1 && !checkWallCollision(2)) {
            gameMap->addPosToMemory();
            gameMap->playerPos += 1;
        }

        if (gameMap->playerFace == 3 && !checkWallCollision(2)) {
            gameMap->addPosToMemory();
            gameMap->playerPos += gameMap->mapCols;
        }

        if (gameMap->playerFace == 2 && !checkWallCollision(2)) {
            gameMap->addPosToMemory();
            gameMap->playerPos -= gameMap->mapCols;
        }

        if ((gameMap->playerFace == 0 && checkWallCollision(2)) || (gameMap->playerFace == 1 && checkWallCollision(2)) || (gameMap->playerFace == 3 && checkWallCollision(2)) || (gameMap->playerFace == 2 && checkWallCollision(2))) {
            //
            // Hintergrundbildwechsel rückgängig machen da keine Bewegung erfolgen darf
            //

            drawBG = drawBG ? false : true;
        }
    }

    //strafe right
    else if (mousePosX >= 44 && mousePosX <= 64 && mousePosY >= 145 && mousePosY <= 162) {
        //
        //	Hintergrundbild wechseln
        //

        drawBG = drawBG ? false : true;

        if (gameMap->playerFace == 0 && !checkWallCollision(3)) {
            gameMap->addPosToMemory();
            gameMap->playerPos += 1;
        }

        if (gameMap->playerFace == 1 && !checkWallCollision(3)) {
            gameMap->addPosToMemory();
            gameMap->playerPos -= 1;
        }

        if (gameMap->playerFace == 3 && !checkWallCollision(3)) {
            gameMap->addPosToMemory();
            gameMap->playerPos -= gameMap->mapCols;
        }

        if (gameMap->playerFace == 2 && !checkWallCollision(3)) {
            gameMap->addPosToMemory();
            gameMap->playerPos += gameMap->mapCols;
        }

        if ((gameMap->playerFace == 0 && checkWallCollision(3)) || (gameMap->playerFace == 1 && checkWallCollision(3)) || (gameMap->playerFace == 3 && checkWallCollision(3)) || (gameMap->playerFace == 2 && checkWallCollision(3))) {
            //
            // Hintergrundbildwechsel rückgängig machen da keine Bewegung erfolgen darf
            //

            drawBG = drawBG ? false : true;
        }
    }
}

//
// renderloop starten
//

void C3DEngine::run()
{

    running = 1;

    while (running == 1) {
        //Screen Update
        updateView();

        // Events verarbeiten
        mouseState();
        mediaObject.updateKeys();

        //auf Taste-loslassen reagieren
        if (!mediaObject.keyDown && !mediaObject.keyLeft && !mediaObject.keyRight && !mediaObject.keyUp)
            keyPressed = false;

        //auf Taste ESC reagieren
        if (mediaObject.keyEsc)
            mediaObject.quit();

        //auf Taste DOWN reagieren
        else if (mediaObject.keyDown && !keyPressed)
            keyDown();

        //auf Taste UP reagieren
        else if (mediaObject.keyUp && !keyPressed)
            keyUp();

        //auf Taste LEFT reagieren
        else if (mediaObject.keyLeft && !keyPressed)
            keyLeft();

        //auf Taste RIGHT reagieren
        else if (mediaObject.keyRight && !keyPressed)
            keyRight();
    }
}
