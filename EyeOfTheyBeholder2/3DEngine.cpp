#pragma warning(disable : 4996)

#include "3DEngine.h"
#include <cstdio>
#include <cstdlib>

// 3D window initialisation, loading of bitmaps etc
Engine::Engine()
{
    for (short i = 0; i < 32; i++) {
        mSaveStatusGameMap[i] = nullptr;
    }

    mMapEvent = new Event;

    mConfiguration.openConf("eob2.conf");

    if (mConfiguration.mResolution == 0)
        mMediaObject.setupWindow(0, 0, 320, 200, mConfiguration.mFullscreen);
    else
        mMediaObject.setupWindow(0, 0, 640, 400, mConfiguration.mFullscreen);

    // Loading Screen
    mMediaObject.fillRect(0, 0, 320, 240, 0, 0, 0);

    // TODO: Centering is not working
    mMediaObject.drawText(0, 20, 20, 255, 255, 255, "...LOADING...", false);

    // Engine Setup
    mMediaObject.drawText(0, 20, 40, 255, 255, 255, "ENGINE ... ");
    mMediaObject.refresh();
    mMediaObject.loadCPS(150, "original/PLAYFLD.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mMediaObject.loadCPS(165, "original/DECORATE.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mMediaObject.loadCPS(166, "original/THROWN.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);
    mMediaObject.drawText(0, 140, 40, 255, 255, 255, "OK");

    // Language Setup
    mMediaObject.drawText(0, 20, 50, 255, 255, 255, "LANGUAGE DATA ... ");
    mMediaObject.refresh();
    mLanguageData.init(mConfiguration.mLanguage);
    mMediaObject.drawText(0, 140, 50, 255, 255, 255, "OK");
    mMediaObject.refresh();

    // Initialize items
    mMediaObject.drawText(0, 20, 60, 255, 255, 255, "ITEM DATA ... ");
    mMediaObject.refresh();
    mItem.init(&mMediaObject, &mLanguageData);
    mMediaObject.drawText(0, 140, 60, 255, 255, 255, "OK");
    mMediaObject.refresh();

    // Initialize the 4 characters
    mMediaObject.drawText(0, 20, 70, 255, 255, 255, "CHARACTER DATA ...");
    mMediaObject.refresh();
    mCharacter[0].init(&mMediaObject, mGameMap);
    mCharacter[0].mMenuPos = 0;
    mCharacter[0].mCharacterID = 0;
    mCharacter[0].mImageID = 0;
    sprintf_s(mCharacter[0].mName, "%s", "SHADUM");
    sprintf_s(mCharacter[0].mClass, "%s", "PALADIN");
    sprintf_s(mCharacter[0].mSocial, "%s", "LAWFUL GOOD");
    sprintf_s(mCharacter[0].mRace, "%s", "HUMAN");
    sprintf_s(mCharacter[0].mGender, "%s", "MALE");
    mCharacter[0].hp = mCharacter[0].currenthp = 100;
    mCharacter[0].food = mCharacter[0].currentfood = 100;
    mCharacter[0].strength = 18;
    mCharacter[0].maxstrength = 80;
    mCharacter[0].intelligence = 9;
    mCharacter[0].wisdom = 16;
    mCharacter[0].dext = 15;
    mCharacter[0].constitution = 17;
    mCharacter[0].charisma = 17;
    mCharacter[0].armorclass = 1;
    mCharacter[0].exp = 69000;
    mCharacter[0].lvl = 6;
    mCharacter[0].mNumberOfArrows = 0;
    mCharacter[0].mItemsInHand[0] = 1;
    mCharacter[0].mItemsInHand[1] = 23;
    mCharacter[1].init(&mMediaObject, mGameMap);
    mCharacter[1].mMenuPos = 1;
    mCharacter[1].mCharacterID = 1;
    mCharacter[1].mImageID = 1;
    sprintf_s(mCharacter[1].mName, "%s", "ZORG");
    sprintf_s(mCharacter[1].mClass, "%s", "FIGHTER");
    sprintf_s(mCharacter[1].mSocial, "%s", "TRUE NEUTRAL");
    sprintf_s(mCharacter[1].mRace, "%s", "DWARF");
    sprintf_s(mCharacter[1].mGender, "%s", "MALE");
    mCharacter[1].hp = mCharacter[1].currenthp = 60;
    mCharacter[1].food = mCharacter[1].currentfood = 100;
    mCharacter[1].strength = 18;
    mCharacter[1].maxstrength = 80;
    mCharacter[1].intelligence = 10;
    mCharacter[1].wisdom = 6;
    mCharacter[1].dext = 17;
    mCharacter[1].constitution = 19;
    mCharacter[1].charisma = 3;
    mCharacter[1].armorclass = 0;
    mCharacter[1].exp = 69000;
    mCharacter[1].lvl = 6;
    mCharacter[1].mNumberOfArrows = 0;
    mCharacter[1].mItemsInHand[0] = 1;
    mCharacter[1].mItemsInHand[1] = 23;
    mCharacter[2].init(&mMediaObject, mGameMap);
    mCharacter[2].mMenuPos = 2;
    mCharacter[2].mCharacterID = 2;
    mCharacter[2].mImageID = 2;
    sprintf_s(mCharacter[2].mName, "%s", "MERLIN");
    sprintf_s(mCharacter[2].mClass, "%s", "CLERIC");
    sprintf_s(mCharacter[2].mSocial, "%s", "CHAOTIC GOOD");
    sprintf_s(mCharacter[2].mRace, "%s", "HUMAN");
    sprintf_s(mCharacter[2].mGender, "%s", "MALE");
    mCharacter[2].hp = mCharacter[2].currenthp = 80;
    mCharacter[2].food = mCharacter[2].currentfood = 100;
    mCharacter[2].strength = 15;
    mCharacter[2].maxstrength = 80;
    mCharacter[2].intelligence = 18;
    mCharacter[2].wisdom = 10;
    mCharacter[2].dext = 19;
    mCharacter[2].constitution = 11;
    mCharacter[2].charisma = 15;
    mCharacter[2].armorclass = 6;
    mCharacter[2].exp = 69000;
    mCharacter[2].lvl = 7;
    mCharacter[2].mNumberOfArrows = 0;
    mCharacter[2].mItemsInHand[0] = 1;
    mCharacter[2].mItemsInHand[1] = 23;
    mCharacter[3].init(&mMediaObject, mGameMap);
    mCharacter[3].mMenuPos = 3;
    mCharacter[3].mCharacterID = 3;
    mCharacter[3].mImageID = 31;
    sprintf_s(mCharacter[3].mName, "%s", "VALERIA");
    sprintf_s(mCharacter[3].mClass, "%s", "MAGE");
    sprintf_s(mCharacter[3].mSocial, "%s", "GOOD");
    sprintf_s(mCharacter[3].mRace, "%s", "ELF");
    sprintf_s(mCharacter[3].mGender, "%s", "FEMALE");
    mCharacter[3].hp = mCharacter[3].currenthp = 50;
    mCharacter[3].food = mCharacter[3].currentfood = 100;
    mCharacter[3].strength = 12;
    mCharacter[3].maxstrength = 80;
    mCharacter[3].intelligence = 11;
    mCharacter[3].wisdom = 18;
    mCharacter[3].dext = 16;
    mCharacter[3].constitution = 16;
    mCharacter[3].charisma = 13;
    mCharacter[3].armorclass = 1;
    mCharacter[3].exp = 69000;
    mCharacter[3].lvl = 7;
    mCharacter[3].mNumberOfArrows = 0;
    mCharacter[3].mItemsInHand[0] = 1;
    mCharacter[3].mItemsInHand[1] = 23;
    mMediaObject.drawText(0, 140, 70, 255, 255, 255, "OK");
    mMediaObject.refresh();

    // Initialize inventory, character and stats menu
    mMediaObject.drawText(0, 20, 80, 255, 255, 255, "INVENTORY ...");
    mMediaObject.refresh();
    for (short i = 0; i < 4; i++) {
        mCharMenu.character[i] = &mCharacter[i];
    }
    mCharMenu.languageData = &mLanguageData;
    mCharMenu.initMenu(&mMediaObject, mMapEvent);
    mCharMenu.item = &mItem;
    mCharMenu.gameMap = mGameMap;
    mCharMenu.mouseItem = &mMouseItem;

    for (short i = 0; i < 4; i++) {
        mMapEvent->mChars[i] = &mCharacter[i];
    }
    mMediaObject.drawText(0, 140, 80, 255, 255, 255, "OK");
    mMediaObject.refresh();

    // Load sounds
    mMediaObject.drawText(0, 20, 90, 255, 255, 255, "SOUNDS ...");
    mMediaObject.refresh();
    mMediaObject.loadSound(2, "sound/hit");
    mMediaObject.loadSound(3, "sound/miss");
    mMediaObject.drawText(0, 140, 90, 255, 255, 255, "OK");
    mMediaObject.refresh();

    // Monster Setup
    for (short m = 0; m < 128; m++) {
        mMonster[m] = nullptr;
    }

    mMediaObject.drawText(0, 20, 140, 255, 255, 255, "PLEASE WAIT ...");
    mMediaObject.refresh();
}

void Engine::playIntro()
{
    Intro intro;

    intro.languageData = mLanguageData;
    intro.init(&mMediaObject);
    intro.play();
}

// Load map with all graphics, coordinates etc
void Engine::loadMap(short mapID, bool savedMap)
{
    //existiert Karte als Statusbackup dieses jetzt initialisieren
    if (mSaveStatusGameMap[mapID] && !savedMap) {
        mCurrentMapID = mapID;
        if (!mGameMap) {
            mGameMap = new Map;
        }
        *mGameMap = *mSaveStatusGameMap[mapID];
        mGameMap->init(mapID, &mMediaObject, true);
    } else if (savedMap) {
        mGameMap->init(mapID, &mMediaObject, true);
    } else {
        // Otherwise reload map normally
        mCurrentMapID = mapID;
        if (mGameMap) {
            mGameMap->clear();
        }
        delete (mGameMap);
        mGameMap = nullptr;
        mGameMap = new Map;
        mGameMap->init(mapID, &mMediaObject);
    }

    // Forward the map to all the objects that need it
    mCharMenu.gameMap = mGameMap;
    for (short i = 0; i < 4; i++) {
        mCharacter[i].mGameMap = mGameMap;
    }

    // Initialize monsters
    for (int i = 0; i < 149; i++) {
        mMediaObject.freeImage(i);
    }
    for (short m = 0; m < mGameMap->monsterCount; m++) {
        if (mMonster[m]) {
            delete (mMonster[m]);
            mMonster[m] = nullptr;
        }
        mMonster[m] = new Monster;
        mMonster[m]->init(mGameMap, &mMediaObject, &mLanguageData, mMapEvent, mGameMap->monsterInfo[m][1],
            mGameMap->monsterInfo[m][0], mGameMap->monsterInfo[m][2], mGameMap->monsterInfo[m][3], mGameMap->monsterInfo[m][4]);
        for (short i = 0; i < 4; i++) {
            mMonster[m]->character[i] = &mCharacter[i];
        }
    }

    // Initialize events
    if (mMapEvent) {
        mMapEvent->clear();
        delete (mMapEvent);
        mMapEvent = nullptr;
    }
    mMapEvent = new Event;
    for (short i = 0; i < 4; i++) {
        mMapEvent->mChars[i] = &mCharacter[i];
    }
    mMapEvent->init(mGameMap, &mMediaObject);
    mMapEvent->mLanguageData = &mLanguageData;
    mMapEvent->statusMessage(mLanguageData.mText[41]);
    mCharMenu.mapEvent = mMapEvent;
}

void Engine::loadGame()
{
    // Open savefile
    FILE* saveFile = fopen("spectalum.sav", "rb");

    // Read headers
    char header[32];
    fread(&header, 32, 1, saveFile);

    // Read map id
    fread(&mCurrentMapID, sizeof(short), 1, saveFile);

    // Read player position
    short tempPlayerPos {};
    fread(&tempPlayerPos, sizeof(short), 1, saveFile);

    // Read player point of view
    short tempPlayerFace {};
    fread(&tempPlayerFace, sizeof(short), 1, saveFile);

    // Read game map
    if (mGameMap) {
        mGameMap->clear();
        delete (mGameMap);
        mGameMap = nullptr;
        mGameMap = new Map;
    }
    mGameMap->loadMap(saveFile);

    // Read number of tickets already visited
    unsigned char mapSaved[33];
    fread(&mapSaved, 33 * sizeof(unsigned char), 1, saveFile);

    // Read tickets already visited
    for (short i = 1; i < 32; i++) {
        if (mapSaved[i] == 1) {
            if (mSaveStatusGameMap[i]) {
                mSaveStatusGameMap[i]->clear();
            }
            delete (mSaveStatusGameMap[i]);
            mSaveStatusGameMap[i] = nullptr;
            mSaveStatusGameMap[i] = new Map;
            mSaveStatusGameMap[i]->loadMap(saveFile);
        }
    }

    for (short i = 0; i < 4; i++) {
        mCharacter[i].loadChar(saveFile);
    }
    fclose(saveFile);

    // Initialize map
    loadMap(mCurrentMapID, true);
    mGameMap->playerPos = tempPlayerPos;
    mGameMap->playerFace = tempPlayerFace;
}

void Engine::saveGame()
{
    // Open savefile
    FILE* saveFile = fopen("spectalum.sav", "wb");

    // Write headers
    char header[32] = "SPECTALUM SAVEGAME";
    fwrite(&header, 32, 1, saveFile);

    // Write map id
    fwrite(&mCurrentMapID, sizeof(short), 1, saveFile);

    // Write player position
    fwrite(&mGameMap->playerPos, sizeof(short), 1, saveFile);

    // Write player point of view
    fwrite(&mGameMap->playerFace, sizeof(short), 1, saveFile);

    // Write game map
    mGameMap->saveMap(saveFile);

    // Write number of tickets already visited
    unsigned char mapSaved[33];
    for (int i = 0; i < 33; i++) {
        mapSaved[i] = 0;
    }
    for (short i = 1; i <= 32; i++) {
        mapSaved[i] = mSaveStatusGameMap[i] ? 1 : 0;
    }
    fwrite(&mapSaved, 33 * sizeof(unsigned char), 1, saveFile);
    for (short i = 1; i < 32; i++) {
        if (mapSaved[i] == 1) {
            mSaveStatusGameMap[i]->saveMap(saveFile);
        }
    }

    // Write characters
    for (short i = 0; i < 4; i++) {
        mCharacter[i].saveChar(saveFile);
    }
    fclose(saveFile);

    mMapEvent->statusMessage(mLanguageData.mText[195]);
}

// Redraw 3D windows
void Engine::updateView()
{
    mMediaObject.fillRect(0, 0, 320, 200, 0, 0, 0);

    // Check download game option
    if (mCharMenu.loadGame) {
        mCharMenu.loadGame = false;
        loadGame();
    }

    // Check save game option
    if (mCharMenu.saveGame) {
        mCharMenu.saveGame = false;
        saveGame();
    }

    if (mDrawBackground) {
        mMediaObject.drawImage(163, 0, 0, 0, 0, 176, 120);
    } else {
        mMediaObject.drawImage(164, 0, 0, 0, 0, 176, 120);
    }

    drawGameWindowNew();

    mMediaObject.drawImage(150, 0, 0); // draw background
    drawCompass();
    mCharMenu.update();
    mMapEvent->updateStatusMessage();

    if (mMapEvent->mEventInProgress) {
        // draw background
        mMediaObject.drawImage(150, 0, 0);

        mCharMenu.update();
        // Update events and draw
        mMapEvent->update();

        if (mMapEvent->mShowGameWindow) {
            if (mDrawBackground) {
                mMediaObject.drawImage(163, 0, 0, 0, 0, 176, 120);
            } else {
                mMediaObject.drawImage(164, 0, 0, 0, 0, 176, 120);
            }
            drawGameWindowNew();
        }

        // Check for map changes
        if (mMapEvent->mMapChange) {
            mSaveStatusGameMap[mCurrentMapID] = nullptr;
            mSaveStatusGameMap[mCurrentMapID] = new Map;
            *mSaveStatusGameMap[mCurrentMapID] = *mGameMap;

            mMapEvent->mMapChange = false;
            short mapID = mMapEvent->mNewMapID;
            short warpToPos = mMapEvent->mWarpToPos;
            short faceTo = mMapEvent->mFaceTo;

            // Delete all monsters
            for (short m = 0; m < 128; m++) {
                if (mMonster[m]) {
                    delete (mMonster[m]);
                    mMonster[m] = nullptr;
                }
            }

            // Delete events
            mMapEvent->clear();
            delete (mMapEvent);
            mMapEvent = nullptr;

            // Delete map and secure current status
            mGameMap->clear();
            delete (mGameMap);
            mGameMap = nullptr;

            // Finally load the new map
            loadMap(mapID);

            // Setup player position
            if (warpToPos > 0) {
                mGameMap->playerPos = warpToPos;
                mGameMap->playerFace = faceTo;
            }
        }
    }

    // Draw crusor
    mItem.drawIcon(this->mMouseItem, this->mRealMouseX, this->mRealMouseY);

    // Refresh screen
    mMediaObject.refresh();
    mMediaObject.updateVideo();
}

// Update and draw monsters
void Engine::drawMonster(short mapWall, short wNr)
{
    for (short m = 0; m < mGameMap->monsterCount; m++) {
        mMonster[m]->update(mMediaObject.getMilliSeconds(), mapWall, wNr);
        mGameMap->monsterInfo[m][0] = mMonster[m]->monsterPos;
    }
}

void Engine::drawStuff(short mapWall, short wNr)
{
    /*
		wNr: 0 until 11
		-1: current point f view - w:64, h:24 respectively w:32, h:24 - x0,y96 and x110,y96
		0: at the front left
		1: the far right
		2: 1 field removed left
		3: 1 field removed mid - w:45, h:17 respectively w:22, h:17 - 36,85 and 95,85
		4: 1 field removed right
		5: 2 fields removed left
		6: 2 fields removed mid - w:32, h:13 respectively w:17, h:13 - 50, 65 and 92, 65
		7: 2 fields removed right
		8: unused
		9: 3 fields removed left
		10: 3 fields remove mid - w:26, h:10 respectively w:13, h:10 - 64, 53 and 86, 53
		11: 3 fields removed right
		12: unused
	*/

    // Items
    short imageID {};
    bool left {};
    short fromPosX {};
    short fromPosY {};
    short width {};
    short height {};
    short toPosX {};
    short toPosY {};
    short yOffset {};

    for (short i = 0; i < 40; i++) {
        yOffset = 0;
        if (mGameMap->item[mapWall][i] > 0 && mGameMap->item[mapWall][i] < 112) {
            //         printf("map %d: %d, %d\n", mapWall, item.largePicID[gameMap->item[mapWall][i]], wNr);

            // check which image is taken: ITEMS1.CPS (0-30), then ITEML1.CPS (31-45)
            imageID = mItem.mLargePicID[mGameMap->item[mapWall][i]] < 31 ? 211 : 212;
            if (imageID == 211) {
                fromPosX = (mItem.mLargePicID[mGameMap->item[mapWall][i]] * 32) % 128;
                fromPosY = (mItem.mLargePicID[mGameMap->item[mapWall][i]] / 4) * 24;
            } else if (imageID == 212) {
                fromPosX = ((mItem.mLargePicID[mGameMap->item[mapWall][i]] - 31) * 64) % 128;
                fromPosY = ((mItem.mLargePicID[mGameMap->item[mapWall][i]] - 31) / 2) * 24;
            }

            // left or right
            left = mGameMap->item_floorpos[mapWall][i] == 1 ? true : false;
            if (mGameMap->playerFace == 1 || mGameMap->playerFace == 2) {
                left = !left;
            }

            // to throw
            if (mGameMap->item_flying[mapWall][i]) {
                mGameMap->throwItem(mapWall, i);
            }

            switch (wNr) {
            case -1:
                if (imageID == 211) {
                    width = 32;
                    height = 24;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 64;
                    height = 24;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 45;
                }
                if (left) {
                    mMediaObject.drawImage(255, 0, 96 - yOffset);
                } else {
                    mMediaObject.drawImage(255, 110, 96 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 0:
            case 1:
                break;
            case 2:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 40;
                }
                if (!left) {
                    mMediaObject.drawImage(255, 36 - 55, 85 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 3:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 40;
                }
                if (left) {
                    mMediaObject.drawImage(255, 36, 85 - yOffset);
                } else {
                    mMediaObject.drawImage(255, 95, 85 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 4:
                if (imageID == 211) {
                    width = 22;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 45;
                    height = 17;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 40;
                }
                if (left) {
                    mMediaObject.drawImage(255, 95 + 55, 85 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 5:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 32;
                }
                if (!left) {
                    mMediaObject.drawImage(255, 52 - 45, 65 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 6:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 32;
                }
                if (left) {
                    mMediaObject.drawImage(255, 50, 65 - yOffset);
                } else {
                    mMediaObject.drawImage(255, 92, 65 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 7:
                if (imageID == 211) {
                    width = 17;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 33;
                    height = 13;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 32;
                }
                if (left) {
                    mMediaObject.drawImage(255, 92 + 45, 65 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 8:
                break;
            case 9:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 25;
                }
                if (!left) {
                    mMediaObject.drawImage(255, 86 - 55, 53 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 10:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 25;
                }
                if (left) {
                    mMediaObject.drawImage(255, 64, 53 - yOffset);
                } else {
                    mMediaObject.drawImage(255, 86, 53 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            case 11:
                if (imageID == 211) {
                    width = 13;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 32, 24, width, height);
                } else if (imageID == 212) {
                    width = 26;
                    height = 10;
                    mMediaObject.createImage(255, imageID, width, height);
                    mMediaObject.copyScaledImage(imageID, 255, fromPosX, fromPosY, 0, 0, 64, 24, width, height);
                }
                if (mGameMap->item_flying[mapWall][i]) {
                    yOffset = 25;
                }
                if (left) {
                    mMediaObject.drawImage(255, 84 + 55, 53 - yOffset);
                }
                mMediaObject.freeImage(255);
                break;
            default:
                break;
            }
        }
    }
    drawMonster(mapWall, wNr);
}

void Engine::drawCompass()
{
    if (mGameMap->playerFace == 0) {
        mMediaObject.drawImage(165, 114, 132, 0, 120, 21, 17);
        mMediaObject.drawImage(165, 79, 158, 0, 137, 17, 10);
        mMediaObject.drawImage(165, 150, 158, 0, 147, 17, 10);
    } else if (mGameMap->playerFace == 1) {
        mMediaObject.drawImage(165, 114, 132, 48, 120, 21, 17);
        mMediaObject.drawImage(165, 79, 158, 48, 137, 17, 10);
        mMediaObject.drawImage(165, 150, 158, 48, 147, 17, 10);
    } else if (mGameMap->playerFace == 2) {
        mMediaObject.drawImage(165, 114, 132, 24, 120, 21, 17);
        mMediaObject.drawImage(165, 79, 158, 24, 137, 17, 10);
        mMediaObject.drawImage(165, 150, 158, 24, 147, 17, 10);
    } else if (mGameMap->playerFace == 3) {
        mMediaObject.drawImage(165, 114, 132, 71, 120, 21, 17);
        mMediaObject.drawImage(165, 79, 158, 72, 137, 17, 10);
        mMediaObject.drawImage(165, 150, 158, 72, 147, 17, 10);
    } else {
        mMediaObject.drawImage(165, 114, 132, 0, 120, 21, 17);
    }
}

void Engine::drawGameWindowNew()
{
    // wall level 1 draw - at the very back
    mGameMap->drawWalls(1);
    drawStuffWindow(1);

    // walls level 2 draw - rear
    mGameMap->drawWalls(2);
    drawStuffWindow(2);

    // walls level 3 draw -  in front of player
    mGameMap->drawWalls(3);
    drawStuffWindow(3);

    // wall level 4 draw -  next to player
    mGameMap->drawWalls(4);
    drawStuffWindow(4);
}

// Render the item and the monster
void Engine::drawStuffWindow(int level)
{
    short wallNrRoot, wallNr;
    if (mGameMap->playerFace == 0) {
        if (level == 1) {
            // the east and west wall of BB
            wallNrRoot = wallNr = mGameMap->playerPos - ((3 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 9);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 11);

            // the north wall of DD, EE, FF
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            // the east and west wall of EE
            wallNrRoot = wallNr = mGameMap->playerPos - ((2 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 5);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 7);

            // the north wall of GG, HH, II
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            // the east and west wall of HH
            wallNrRoot = wallNr = mGameMap->playerPos - ((1 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 2);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 4);

            // the north wall of JJ, KK, LL
            wallNr = wallNrRoot + 1;
            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNrRoot = wallNr = mGameMap->playerPos - 1;
            drawStuff(wallNr, 0);
            wallNr = wallNrRoot + 2;
            drawStuff(wallNr, 1);
        }
    }

    if (mGameMap->playerFace == 1) {
        if (level == 1) {
            wallNrRoot = wallNr = mGameMap->playerPos + ((3 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 9);
            wallNr = wallNrRoot - 2;
            drawStuff(wallNr, 11);
            wallNr = wallNrRoot - 1;
            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            wallNrRoot = wallNr = mGameMap->playerPos + ((2 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 5);
            wallNr = wallNrRoot - 2;
            drawStuff(wallNr, 7);
            wallNr = wallNrRoot - 1;
            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNrRoot = wallNr = mGameMap->playerPos + ((1 * mGameMap->mapCols) + 1);
            drawStuff(wallNr, 2);
            wallNr = wallNrRoot - 2;
            drawStuff(wallNr, 4);
            wallNr = wallNrRoot - 1;
            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNrRoot = wallNr = mGameMap->playerPos + 1;
            drawStuff(wallNr, 0);
            wallNr = wallNrRoot - 2;
            drawStuff(wallNr, 1);
        }
    }

    if (mGameMap->playerFace == 3) {
        if (level == 1) {
            wallNr = mGameMap->playerPos + 1 + (mGameMap->mapCols - 4);
            drawStuff(wallNr, 9);
            wallNr = mGameMap->playerPos + 1 - (mGameMap->mapCols + 4);
            drawStuff(wallNr, 11);
            wallNr = mGameMap->playerPos + 1 - 4;
            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            wallNr = mGameMap->playerPos + 2 + (mGameMap->mapCols - 4);
            drawStuff(wallNr, 5);
            wallNr = mGameMap->playerPos + 2 - (mGameMap->mapCols + 4);
            drawStuff(wallNr, 7);
            wallNr = mGameMap->playerPos + 2 - 4;
            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNr = mGameMap->playerPos + 3 + (mGameMap->mapCols - 4);
            drawStuff(wallNr, 2);
            wallNr = mGameMap->playerPos + 3 - (mGameMap->mapCols + 4);
            drawStuff(wallNr, 4);
            wallNr = mGameMap->playerPos + 3 - 4;
            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNr = mGameMap->playerPos + 4 + (mGameMap->mapCols - 4);
            drawStuff(wallNr, 0);
            wallNr = mGameMap->playerPos + 4 - (mGameMap->mapCols + 4);
            drawStuff(wallNr, 1);
        }
    }

    if (mGameMap->playerFace == 2) {
        if (level == 1) {
            wallNr = mGameMap->playerPos - 1 - (mGameMap->mapCols - 4);
            drawStuff(wallNr, 9);
            wallNr = mGameMap->playerPos - 1 + (mGameMap->mapCols + 4);
            drawStuff(wallNr, 11);
            wallNr = mGameMap->playerPos - 1 + 4;
            drawStuff(wallNr, 10);
        }

        if (level == 2) {
            wallNr = mGameMap->playerPos - 2 - (mGameMap->mapCols - 4);
            drawStuff(wallNr, 5);
            wallNr = mGameMap->playerPos - 2 + (mGameMap->mapCols + 4);
            drawStuff(wallNr, 7);
            wallNr = mGameMap->playerPos - 2 + 4;
            drawStuff(wallNr, 6);
        }

        if (level == 3) {
            wallNr = mGameMap->playerPos - 3 - (mGameMap->mapCols - 4);
            drawStuff(wallNr, 2);
            wallNr = mGameMap->playerPos - 3 + (mGameMap->mapCols + 4);
            drawStuff(wallNr, 4);
            wallNr = mGameMap->playerPos - 3 + 4;
            drawStuff(wallNr, 3);
        }

        if (level == 4) {
            wallNr = mGameMap->playerPos - 4 - (mGameMap->mapCols - 4);
            drawStuff(wallNr, 0);
            wallNr = mGameMap->playerPos - 4 + (mGameMap->mapCols + 4);
            drawStuff(wallNr, 1);
        }
    }

    // No wall was drawn anyway to draw the  "Stuff " of the current position
    drawStuff(mGameMap->playerPos, -1);
}

// Check collision with monster
bool Engine::checkMonsterCollision(short direction)
{
    bool collision {};

    for (short m = 0; m < mGameMap->monsterCount; m++) {
        // 0=N, 1=S, 2=E, 3=W
        switch (direction) {
        case 0:
            switch (mGameMap->playerFace) {
            case 0:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - mGameMap->mapCols))
                    collision = true;
                break;
            case 1:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + mGameMap->mapCols))
                    collision = true;
                break;
            case 2:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + 1))
                    collision = true;
                break;
            case 3:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - 1))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (mGameMap->playerFace) {
            case 0:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + mGameMap->mapCols))
                    collision = true;
                break;
            case 1:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - mGameMap->mapCols))
                    collision = true;
                break;
            case 2:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - 1))
                    collision = true;
                break;
            case 3:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + 1))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (mGameMap->playerFace) {
            case 0:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - 1))
                    collision = true;
                break;
            case 1:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + 1))
                    collision = true;
                break;
            case 2:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - mGameMap->mapCols))
                    collision = true;
                break;
            case 3:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + mGameMap->mapCols))
                    collision = true;
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (mGameMap->playerFace) {
            case 0:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + 1))
                    collision = true;
                break;
            case 1:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - 1))
                    collision = true;
                break;
            case 2:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos + mGameMap->mapCols))
                    collision = true;
                break;
            case 3:
                if (mGameMap->monsterInfo[m][0] == (mGameMap->playerPos - mGameMap->mapCols))
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

// Check collision with wall
bool Engine::checkWallCollision(short direction)
{
    bool collision {};

    // Movement forward - check of obstacle
    if (direction == 0) {
        if (mGameMap->playerFace == 0) {
            if (mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos - mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 1) {
            if (mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos + mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 3) {
            if (mGameMap->mapEvent[mGameMap->playerPos - 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - 1];
                mMapEvent->mMapPos = mGameMap->playerPos - 1;
                mMapEvent->mEventInProgress = true;
            }

            if (mGameMap->isSolidWall(0, mGameMap->playerPos - 1))
                collision = true;
        }
        if (mGameMap->playerFace == 2) {
            if (mGameMap->mapEvent[mGameMap->playerPos + 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + 1];
                mMapEvent->mMapPos = mGameMap->playerPos + 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + 1))
                collision = true;
        }
    }

    // Movement backwards - check for obstacle
    if (direction == 1) {
        if (mGameMap->playerFace == 0) {
            if (mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos + mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 1) {
            if (mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos - mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 3) {
            if (mGameMap->mapEvent[mGameMap->playerPos + 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + 1];
                mMapEvent->mMapPos = mGameMap->playerPos + 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + 1))
                collision = true;
        }
        if (mGameMap->playerFace == 2) {
            if (mGameMap->mapEvent[mGameMap->playerPos - 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - 1];
                mMapEvent->mMapPos = mGameMap->playerPos - 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - 1))
                collision = true;
        }
    }

    // Movement to the left - check for obstacle
    if (direction == 2) {
        if (mGameMap->playerFace == 0) {
            if (mGameMap->mapEvent[mGameMap->playerPos - 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - 1];
                mMapEvent->mMapPos = mGameMap->playerPos - 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - 1))
                collision = true;
        }
        if (mGameMap->playerFace == 1) {
            if (mGameMap->mapEvent[mGameMap->playerPos + 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + 1];
                mMapEvent->mMapPos = mGameMap->playerPos + 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + 1))
                collision = true;
        }
        if (mGameMap->playerFace == 3) {
            if (mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos + mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 2) {
            if (mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos - mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - mGameMap->mapCols))
                collision = true;
        }
    }

    // Movement to the right - check for obstacle
    if (direction == 3) {
        if (mGameMap->playerFace == 0) {
            if (mGameMap->mapEvent[mGameMap->playerPos + 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + 1];
                mMapEvent->mMapPos = mGameMap->playerPos + 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + 1))
                collision = true;
        }
        if (mGameMap->playerFace == 1) {
            if (mGameMap->mapEvent[mGameMap->playerPos - 1] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - 1];
                mMapEvent->mMapPos = mGameMap->playerPos - 1;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - 1))
                collision = true;
        }
        if (mGameMap->playerFace == 3) {
            if (mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos - mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos - mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos - mGameMap->mapCols))
                collision = true;
        }
        if (mGameMap->playerFace == 2) {
            if (mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols] > 0) {
                mMapEvent->mEventType = mGameMap->mapEvent[mGameMap->playerPos + mGameMap->mapCols];
                mMapEvent->mMapPos = mGameMap->playerPos + mGameMap->mapCols;
                mMapEvent->mEventInProgress = true;
            }
            if (mGameMap->isSolidWall(0, mGameMap->playerPos + mGameMap->mapCols))
                collision = true;
        }
    }

    if (!collision) {
        // if no collision with wall check for collsion with monster
        collision = checkMonsterCollision(direction);
    }

    return collision;
}

// Respond to keystroke right
void Engine::handleRightKeyPressed()
{
    // Change wallpaper
    mGameMap->mazeObjects->p_shift++;
    mDrawBackground = mDrawBackground ? false : true;

    if (mGameMap->playerFace == 0) {
        mGameMap->playerFace = 2;
    } else if (mGameMap->playerFace == 2) {
        mGameMap->playerFace = 1;
    } else if (mGameMap->playerFace == 1) {
        mGameMap->playerFace = 3;
    } else if (mGameMap->playerFace == 3) {
        mGameMap->playerFace = 0;
    }
    mIsKeyPressed = true;
}

// Respond to keystroke left
void Engine::handleLeftKeyPressed()
{
    // Change wallpaper
    mGameMap->mazeObjects->p_shift--;
    mDrawBackground = mDrawBackground ? false : true;

    if (mGameMap->playerFace == 0) {
        mGameMap->playerFace = 3;
    } else if (mGameMap->playerFace == 3) {
        mGameMap->playerFace = 1;
    } else if (mGameMap->playerFace == 1) {
        mGameMap->playerFace = 2;
    } else if (mGameMap->playerFace == 2) {
        mGameMap->playerFace = 0;
    }
    mIsKeyPressed = true;
}

// Respond to keystroke up
void Engine::handleUpKeyPressed()
{
    // Change wallpaper
    mGameMap->mazeObjects->p_shift2++;
    mDrawBackground = mDrawBackground ? false : true;

    if (mGameMap->playerFace == 0 && !checkWallCollision(0)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos -= mGameMap->mapCols;
    }

    if (mGameMap->playerFace == 1 && !checkWallCollision(0)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos += mGameMap->mapCols;
    }

    if (mGameMap->playerFace == 3 && !checkWallCollision(0)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos--;
    }

    if (mGameMap->playerFace == 2 && !checkWallCollision(0)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos++;
    }

    if (checkWallCollision(0)) {
        // Reversing background image changes as no movement is allowed
        mDrawBackground = mDrawBackground ? false : true;
    }

    mIsKeyPressed = true;
}

// Respond to keystroke down
void Engine::handleDownKeyPressed()
{
    // Change wallpaper
    mGameMap->mazeObjects->p_shift2--;
    mDrawBackground = mDrawBackground ? false : true;

    if (mGameMap->playerFace == 0 && !checkWallCollision(1)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos += mGameMap->mapCols;
    }

    if (mGameMap->playerFace == 1 && !checkWallCollision(1)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos -= mGameMap->mapCols;
    }

    if (mGameMap->playerFace == 3 && !checkWallCollision(1)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos++;
    }

    if (mGameMap->playerFace == 2 && !checkWallCollision(1)) {
        mGameMap->addPosToMemory();
        mGameMap->playerPos--;
    }

    if (checkWallCollision(1)) {
        // Hintergrundbildwechsel rückgängig machen da keine Bewegung erfolgen darf
        mDrawBackground = mDrawBackground ? false : true;
    }

    mIsKeyPressed = true;
}

// Respond to mouse clicks
void Engine::mouseState()
{
    // Update events
    mMediaObject.getMouseState(&mMousePosX, &mMousePosY);
    this->mRealMouseX = this->mMousePosX;
    this->mRealMouseY = this->mMousePosY;

    // 640x400
    if (mConfiguration.mResolution == 1) {
        mMousePosX /= 2;
        mMousePosY /= 2;
        this->mRealMouseX = this->mMousePosX;
        this->mRealMouseY = this->mMousePosY;
    }

    if (!mButtonPressed && mMediaObject.getMouseLeft()) {
        mButtonLeft = mButtonPressed = true;
        mButtonRight = false;
    }
    if (!mButtonPressed && mMediaObject.getMouseRight()) {
        mButtonRight = mButtonPressed = true;
        mButtonLeft = false;
    } else if (mButtonPressed && !mMediaObject.getMouseLeft() && !mMediaObject.getMouseRight()) {
        mButtonPressed = false;
    }

    // Pass current mouse position to character menu
    mCharMenu.mouseX = this->mRealMouseX;
    mCharMenu.mouseY = this->mRealMouseY;

    // Process events
    mMapEvent->mMousePosX = mMousePosX;
    mMapEvent->mMousePosY = mMousePosY;

    if (mMapEvent->mEventInProgress) {
        if (mButtonLeft) {
            mMapEvent->mouseState(mMousePosX, mMousePosY);
        }
    } else {
        // Movement of the figure
        if (mButtonLeft) {
            mouseMove();
        }

        // Continue to submit clicks to character menu
        if (mButtonLeft)
            mCharMenu.mouseState(mMousePosX, mMousePosY, true);
        if (mButtonRight)
            mCharMenu.mouseState(mMousePosX, mMousePosY, false);

        // Only process clicks in the 3D window when game menu is closed
        if (mCharMenu.menuStyle == 0) {
            // Process clicks in 3D window
            if (mButtonLeft && mMousePosX >= 0 && mMousePosX <= 176 && mMousePosY >= 0 && mMousePosY <= 120)
                handleGameWindowClick(mMousePosX, mMousePosY);

            // Check whether a wall or the like has been clicked and whether an event needs to be triggered
            if (mButtonLeft && mMousePosX >= 10 && mMousePosX <= 170 && mMousePosY >= 8 && mMousePosY <= 93) {
                // Get ID of clicked wall
                short wallToCheck = mGameMap->playerPos;
                if (mGameMap->playerFace == 0)
                    wallToCheck -= mGameMap->mapCols;

                if (mGameMap->playerFace == 1)
                    wallToCheck += mGameMap->mapCols;

                if (mGameMap->playerFace == 3)
                    wallToCheck--;

                if (mGameMap->playerFace == 2)
                    wallToCheck++;

                // Trigger even if available
                if (mGameMap->mapEvent[wallToCheck] > 0) {
                    mMapEvent->mEventType = mGameMap->mapEvent[wallToCheck];
                    mMapEvent->mClickedMapPos = wallToCheck;
                    mMapEvent->mEventInProgress = true;
                }
                // printf("%d\n", wallToCheck);
            }
        } else {
            mButtonLeft = mButtonRight = false;
        }
    }

    // An attack of a charter was triggered then relay the damage to the opponent
    for (short i = 0; i < 4; i++) {
        if (mCharacter[i].doDamageToMonsterValue > 0) {
            mMonster[mCharacter[i].doDamageToMonster]->doDamage(mCharacter[i].doDamageToMonsterValue);
            mCharacter[i].doDamageToMonsterValue = 0;
            mCharacter[i].doDamageToMonster = -1;
        }
    }

    // Delete button clicks again
    mButtonLeft = mButtonRight = false;
}

// Process clicks in 3D window
void Engine::handleGameWindowClick(int mousePosX, int mousePosY)
{
    // Items links und rechts am unteren Bildschirmrand aufheben/ablegen
    mMouseItemOld = mMouseItem;

    // links
    if (mousePosX >= 0 && mousePosX <= 88 && mousePosY <= 120) {

        // No item is held in the mouse
        if (this->mMouseItem == 0) {
            // Check whether items exist and cancel items at this point in the current point of view
            for (short i = 99; i >= 0; i--) {
                if (mGameMap->item[mGameMap->playerPos][i] > 0 && mGameMap->item[mGameMap->playerPos][i] < 112)
                    if ((mGameMap->playerFace == 0 || mGameMap->playerFace == 3) && mGameMap->item_floorpos[mGameMap->playerPos][i] == 1) {
                        mMouseItem = mGameMap->item[mGameMap->playerPos][i];
                        mGameMap->item[mGameMap->playerPos][i] = 0;
                        mGameMap->item_counter[mGameMap->playerPos]--;
                        break;
                    } else if ((mGameMap->playerFace == 1 || mGameMap->playerFace == 2) && mGameMap->item_floorpos[mGameMap->playerPos][i] == 2) {
                        mMouseItem = mGameMap->item[mGameMap->playerPos][i];
                        mGameMap->item[mGameMap->playerPos][i] = 0;
                        mGameMap->item_counter[mGameMap->playerPos]--;
                        break;
                    }
            }
        } else {
            for (short i = 0; i < 40; i++) {
                // Put down the item
                if (mGameMap->item[mGameMap->playerPos][i] == 0) {
                    // throwing
                    if (mousePosY < 60) {
                        mGameMap->item_flying_direction[mGameMap->playerPos][i] = mGameMap->playerFace;
                        mGameMap->item_flying[mGameMap->playerPos][i] = true;
                    } else
                        mGameMap->item_flying[mGameMap->playerPos][i] = false;

                    if (mGameMap->playerFace == 0 || mGameMap->playerFace == 3) {
                        mGameMap->item[mGameMap->playerPos][i] = this->mMouseItem;
                        mGameMap->item_floorpos[mGameMap->playerPos][i] = 1;
                        mGameMap->item_counter[mGameMap->playerPos]++;
                        mMouseItem = 0;
                    } else if (mGameMap->playerFace == 1 || mGameMap->playerFace == 2) {
                        mGameMap->item[mGameMap->playerPos][i] = this->mMouseItem;
                        mGameMap->item_floorpos[mGameMap->playerPos][i] = 2;
                        mGameMap->item_counter[mGameMap->playerPos]++;
                        mMouseItem = 0;
                    }
                }
            }
        }
    } else if (mousePosX >= 89 && mousePosX <= 177 && mousePosY <= 120) { // right
        // No item it held  in the mouse
        if (this->mMouseItem == 0) {
            // Check whether items exist and cancel items at this point in the current point of view
            for (short i = 99; i >= 0; i--) {
                if (mGameMap->item[mGameMap->playerPos][i] > 0 && mGameMap->item[mGameMap->playerPos][i] < 112)
                    if ((mGameMap->playerFace == 0 || mGameMap->playerFace == 3) && mGameMap->item_floorpos[mGameMap->playerPos][i] == 2) {
                        mMouseItem = mGameMap->item[mGameMap->playerPos][i];
                        mGameMap->item[mGameMap->playerPos][i] = 0;
                        mGameMap->item_counter[mGameMap->playerPos]--;
                        break;
                    } else if ((mGameMap->playerFace == 1 || mGameMap->playerFace == 2) && mGameMap->item_floorpos[mGameMap->playerPos][i] == 1) {
                        this->mMouseItem = mGameMap->item[mGameMap->playerPos][i];
                        mGameMap->item[mGameMap->playerPos][i] = 0;
                        mGameMap->item_counter[mGameMap->playerPos]--;
                        break;
                    }
            }
        } else {
            for (short i = 0; i < 40; i++) {
                // Put down the item
                if (mGameMap->item[mGameMap->playerPos][i] == 0) {
                    // throw
                    if (mousePosY < 60) {
                        mGameMap->item_flying_direction[mGameMap->playerPos][i] = mGameMap->playerFace;
                        mGameMap->item_flying[mGameMap->playerPos][i] = true;
                    } else
                        mGameMap->item_flying[mGameMap->playerPos][i] = false;

                    if (mGameMap->playerFace == 0 || mGameMap->playerFace == 3) {
                        mGameMap->item[mGameMap->playerPos][i] = this->mMouseItem;
                        mGameMap->item_floorpos[mGameMap->playerPos][i] = 2;
                        mGameMap->item_counter[mGameMap->playerPos]++;
                        this->mMouseItem = 0;
                    } else if (mGameMap->playerFace == 1 || mGameMap->playerFace == 2) {
                        mGameMap->item[mGameMap->playerPos][i] = this->mMouseItem;
                        mGameMap->item_floorpos[mGameMap->playerPos][i] = 1;
                        mGameMap->item_counter[mGameMap->playerPos]++;
                        this->mMouseItem = 0;
                    }
                }
            }
        }
    }

    if (mMouseItemOld != mMouseItem && mMouseItem != 0) {
        char temp[256];
        sprintf_s(temp, "%s %s", mItem.mItemName[mMouseItem], mLanguageData.mText[42]);
        mMapEvent->statusMessage(temp);
    }
}

// Movement of the party
void Engine::mouseMove()
{
    // rotate to the  left
    if (mMousePosX >= 5 && mMousePosX <= 24 && mMousePosY >= 128 && mMousePosY <= 145)
        handleLeftKeyPressed();

    // rotate to the right
    else if (mMousePosX >= 44 && mMousePosX <= 64 && mMousePosY >= 128 && mMousePosY <= 145)
        handleRightKeyPressed();

    // forwards
    else if (mMousePosX >= 24 && mMousePosX <= 44 && mMousePosY >= 128 && mMousePosY <= 145)
        handleUpKeyPressed();

    // backwards
    else if (mMousePosX >= 24 && mMousePosX <= 44 && mMousePosY >= 145 && mMousePosY <= 162)
        handleDownKeyPressed();

    // strafe left
    else if (mMousePosX >= 5 && mMousePosX <= 24 && mMousePosY >= 145 && mMousePosY <= 162) {
        // change wallpaper
        mDrawBackground = mDrawBackground ? false : true;

        if (mGameMap->playerFace == 0 && !checkWallCollision(2)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos -= 1;
        }

        if (mGameMap->playerFace == 1 && !checkWallCollision(2)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos += 1;
        }

        if (mGameMap->playerFace == 3 && !checkWallCollision(2)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos += mGameMap->mapCols;
        }

        if (mGameMap->playerFace == 2 && !checkWallCollision(2)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos -= mGameMap->mapCols;
        }

        if ((mGameMap->playerFace == 0 && checkWallCollision(2)) || (mGameMap->playerFace == 1 && checkWallCollision(2)) || (mGameMap->playerFace == 3 && checkWallCollision(2)) || (mGameMap->playerFace == 2 && checkWallCollision(2))) {
            // Reversing background image changes as no movement is allowed
            mDrawBackground = mDrawBackground ? false : true;
        }
    }

    // strafe right
    else if (mMousePosX >= 44 && mMousePosX <= 64 && mMousePosY >= 145 && mMousePosY <= 162) {
        // Change wallpaper
        mDrawBackground = mDrawBackground ? false : true;

        if (mGameMap->playerFace == 0 && !checkWallCollision(3)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos += 1;
        }

        if (mGameMap->playerFace == 1 && !checkWallCollision(3)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos -= 1;
        }

        if (mGameMap->playerFace == 3 && !checkWallCollision(3)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos -= mGameMap->mapCols;
        }

        if (mGameMap->playerFace == 2 && !checkWallCollision(3)) {
            mGameMap->addPosToMemory();
            mGameMap->playerPos += mGameMap->mapCols;
        }

        if ((mGameMap->playerFace == 0 && checkWallCollision(3)) || (mGameMap->playerFace == 1 && checkWallCollision(3)) || (mGameMap->playerFace == 3 && checkWallCollision(3)) || (mGameMap->playerFace == 2 && checkWallCollision(3))) {
            // Reversing background image changes as no movement is allowed
            mDrawBackground = mDrawBackground ? false : true;
        }
    }
}

// Start renderloop
void Engine::run()
{
    // TODO - isRunning is never set to false
    bool isRunning { true };

    while (isRunning) {
        // screen Update
        updateView();

        // events to use
        mouseState();
        mMediaObject.updateKeys();

        // respond to keystroke
        if (!mMediaObject.mDownKey && !mMediaObject.mLeftKey && !mMediaObject.mRightKey && !mMediaObject.mUpKey)
            mIsKeyPressed = false;

        // respond to escape key pressed
        if (mMediaObject.mEscapeKey)
            mMediaObject.quit();

        // respond to down key pressed
        else if (mMediaObject.mDownKey && !mIsKeyPressed)
            handleDownKeyPressed();

        // respond to up key pressed
        else if (mMediaObject.mUpKey && !mIsKeyPressed)
            handleUpKeyPressed();

        // respond to left key pressed
        else if (mMediaObject.mLeftKey && !mIsKeyPressed)
            handleLeftKeyPressed();

        // respond to right key pressed
        else if (mMediaObject.mRightKey && !mIsKeyPressed)
            handleRightKeyPressed();
    }
}
