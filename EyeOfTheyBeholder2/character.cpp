#include "character.h"
#include <cstdio>
#include <cstdlib>

// Initialisierung
void Char::init(MediaWrapper* mediaObject, Map* gameMap)
{
    //Item Init
    this->mediaObject = mediaObject;
    this->mGameMap = gameMap;
    this->mItemsInHand[0] = 85;
    this->mItemsInHand[1] = 86;
    for (short i = 0; i < 14; i++)
        this->mItemsInInventory[i] = 52;
    this->mItemOnHead = 52;
    this->mItemAroundNeck = 52;
    this->mItemOnTorso = 52;
    this->mItemsOnBelt[0] = this->mItemsOnBelt[1] = this->mItemsOnBelt[2] = 52;
    this->mItemOnFeet = 52;
    this->mItemOnWrist = 52;
    this->damageTaken = this->damageDone[0] = this->damageDone[1] = false;
    this->damageDoneValue[0] = this->damageDoneValue[1] = 0;
    this->doDamageToMonster = -1;
    this->mMenuPos = 0;
}

// Schaden hinzufügen (diesem Char)
void Char::doDamage(short damage)
{
    if (damage < 0)
        damage = 0;
    if (currenthp >= 0)
        currenthp -= damage;
    if (currenthp < 0)
        currenthp = 0;

    damageTakenValue = damage;
    damageTaken = true;
}

// Angreifen
void Char::attackNow(short handPos)
{
    short damage = 0;
    mediaObject->playSound(3);

    //Map-Position vor dem Spieler finden
    short frontPos = 0;
    if (mGameMap->playerFace == 0)
        frontPos = mGameMap->playerPos - 32;
    if (mGameMap->playerFace == 1)
        frontPos = mGameMap->playerPos + 32;
    if (mGameMap->playerFace == 2)
        frontPos = mGameMap->playerPos + 1;
    if (mGameMap->playerFace == 3)
        frontPos = mGameMap->playerPos - 1;

    //prüfen ob sich Monster vor dem Char befinden
    short monsterID = -1;
    for (short m = 0; m < mGameMap->monsterCount; m++)
        if (mGameMap->monsterInfo[m][0] == frontPos)
            monsterID = m;

    //wurde ein Monster gefunden diesem Schaden zufügen
    if (monsterID >= 0) {
        srand(mediaObject->getMilliSeconds());
        damage = rand() % 4 + 3;

        //Schaden wird in C3DEngine - updateView verarbeitet
        doDamageToMonsterValue = damage;
        doDamageToMonster = monsterID;
    }

    damageDoneValue[handPos] = damage;
    damageDone[handPos] = true;
}

// Char speichern
void Char::saveChar(FILE* saveFile)
{
    //ID des Chars im Spiel
    fwrite(&mCharacterID, sizeof(short), 1, saveFile);

    //Pos des Chars im Charmenu
    fwrite(&mMenuPos, sizeof(short), 1, saveFile);

    //Bild dieses Chars
    fwrite(&mImageID, sizeof(short), 1, saveFile);

    //Items in den Händen des Chars
    for (int i = 0; i < 2; i++)
        fwrite(&mItemsInHand[i], sizeof(short), 1, saveFile);

    //Items im Inventar
    for (int i = 0; i < 14; i++)
        fwrite(&mItemsInInventory[i], sizeof(short), 1, saveFile);

    //Kopf Item
    fwrite(&mItemOnHead, sizeof(short), 1, saveFile);

    //Hals Item
    fwrite(&mItemAroundNeck, sizeof(short), 1, saveFile);

    //Brust Item
    fwrite(&mItemOnTorso, sizeof(short), 1, saveFile);

    //Handgelenk Item
    fwrite(&mItemOnWrist, sizeof(short), 1, saveFile);

    //Fuss Item
    fwrite(&mItemOnFeet, sizeof(short), 1, saveFile);

    //Gürtel Items
    for (int i = 0; i < 3; i++)
        fwrite(&mItemsOnBelt[i], sizeof(short), 1, saveFile);

    //Name des Chars
    for (int i = 0; i < 32; i++)
        fwrite(&mName[i], sizeof(char), 1, saveFile);

    //Klasse
    for (int i = 0; i < 32; i++)
        fwrite(&mClass[i], sizeof(char), 1, saveFile);

    //Ausrichtung
    for (int i = 0; i < 32; i++)
        fwrite(&mSocial[i], sizeof(char), 1, saveFile);

    //Rasse
    for (int i = 0; i < 32; i++)
        fwrite(&mRace[i], sizeof(char), 1, saveFile);

    //Geschlecht
    for (int i = 0; i < 8; i++)
        fwrite(&mGender[i], sizeof(char), 1, saveFile);

    //Hitpoints
    fwrite(&hp, sizeof(short), 1, saveFile);
    fwrite(&currenthp, sizeof(short), 1, saveFile);

    //Hunger
    fwrite(&food, sizeof(short), 1, saveFile);
    fwrite(&currentfood, sizeof(short), 1, saveFile);

    //Stärke
    fwrite(&strength, sizeof(short), 1, saveFile);

    //max. Stärke
    fwrite(&maxstrength, sizeof(short), 1, saveFile);

    //Intelligenz
    fwrite(&intelligence, sizeof(short), 1, saveFile);

    //Wissen
    fwrite(&wisdom, sizeof(short), 1, saveFile);

    //Geschick
    fwrite(&dext, sizeof(short), 1, saveFile);

    //Ausdauer
    fwrite(&constitution, sizeof(short), 1, saveFile);

    //Charisma
    fwrite(&charisma, sizeof(short), 1, saveFile);

    //Rüstungs Klasse
    fwrite(&armorclass, sizeof(short), 1, saveFile);

    //Erfahrung
    fwrite(&exp, sizeof(int), 1, saveFile);

    //Level
    fwrite(&lvl, sizeof(short), 1, saveFile);

    //Pfeile im Inv.
    fwrite(&mNumberOfArrows, sizeof(short), 1, saveFile);
}

// Char laden
void Char::loadChar(FILE* saveFile)
{
    //ID des Chars im Spiel
    fread(&mCharacterID, sizeof(short), 1, saveFile);

    //Pos des Chars im Charmenu
    fread(&mMenuPos, sizeof(short), 1, saveFile);

    //Bild dieses Chars
    fread(&mImageID, sizeof(short), 1, saveFile);

    //Items in den Händen des Chars
    for (int i = 0; i < 2; i++)
        fread(&mItemsInHand[i], sizeof(short), 1, saveFile);

    //Items im Inventar
    for (int i = 0; i < 14; i++)
        fread(&mItemsInInventory[i], sizeof(short), 1, saveFile);

    //Kopf Item
    fread(&mItemOnHead, sizeof(short), 1, saveFile);

    //Hals Item
    fread(&mItemAroundNeck, sizeof(short), 1, saveFile);

    //Brust Item
    fread(&mItemOnTorso, sizeof(short), 1, saveFile);

    //Handgelenk Item
    fread(&mItemOnWrist, sizeof(short), 1, saveFile);

    //Fuss Item
    fread(&mItemOnFeet, sizeof(short), 1, saveFile);

    //Gürtel Items
    for (int i = 0; i < 3; i++)
        fread(&mItemsOnBelt[i], sizeof(short), 1, saveFile);

    //Name des Chars
    for (int i = 0; i < 32; i++)
        fread(&mName[i], sizeof(char), 1, saveFile);

    //Klasse
    for (int i = 0; i < 32; i++)
        fread(&mClass[i], sizeof(char), 1, saveFile);

    //Ausrichtung
    for (int i = 0; i < 32; i++)
        fread(&mSocial[i], sizeof(char), 1, saveFile);

    //Rasse
    for (int i = 0; i < 32; i++)
        fread(&mRace[i], sizeof(char), 1, saveFile);

    //Geschlecht
    for (int i = 0; i < 8; i++)
        fread(&mGender[i], sizeof(char), 1, saveFile);

    //Hitpoints
    fread(&hp, sizeof(short), 1, saveFile);
    fread(&currenthp, sizeof(short), 1, saveFile);

    //Hunger
    fread(&food, sizeof(short), 1, saveFile);
    fread(&currentfood, sizeof(short), 1, saveFile);

    //Stärke
    fread(&strength, sizeof(short), 1, saveFile);

    //max. Stärke
    fread(&maxstrength, sizeof(short), 1, saveFile);

    //Intelligenz
    fread(&intelligence, sizeof(short), 1, saveFile);

    //Wissen
    fread(&wisdom, sizeof(short), 1, saveFile);

    //Geschick
    fread(&dext, sizeof(short), 1, saveFile);

    //Ausdauer
    fread(&constitution, sizeof(short), 1, saveFile);

    //Charisma
    fread(&charisma, sizeof(short), 1, saveFile);

    //Rüstungs Klasse
    fread(&armorclass, sizeof(short), 1, saveFile);

    //Erfahrung
    fread(&exp, sizeof(int), 1, saveFile);

    //Level
    fread(&lvl, sizeof(short), 1, saveFile);

    //Pfeile im Inv.
    fread(&mNumberOfArrows, sizeof(short), 1, saveFile);
}
