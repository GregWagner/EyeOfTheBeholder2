
/**********************************
Class CChar
***********************************/

#include "character.h"
#include <cstdio>
#include <cstdlib>

//
// Initialisierung
//

void CChar::init(MEDIAWrapper* mediaObject, CMap* gameMap)
{
    //Item Init
    this->mediaObject = mediaObject;
    this->gameMap = gameMap;
    this->itemHand[0] = 85;
    this->itemHand[1] = 86;
    for (short i = 0; i < 14; i++)
        this->itemInventar[i] = 52;
    this->itemHead = 52;
    this->itemNeck = 52;
    this->itemTorso = 52;
    this->itemBelt[0] = this->itemBelt[1] = this->itemBelt[2] = 52;
    this->itemFoot = 52;
    this->itemWrist = 52;
    this->damageTaken = this->damageDone[0] = this->damageDone[1] = false;
    this->damageDoneValue[0] = this->damageDoneValue[1] = 0;
    this->doDamageToMonster = -1;
    this->menuPos = 0;
}

//
// Schaden hinzufügen (diesem Char)
//

void CChar::doDamage(short damage)
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

//
// Angreifen
//

void CChar::attackNow(short handPos)
{
    short damage = 0;
    mediaObject->playSound(3);

    //Map-Position vor dem Spieler finden
    short frontPos = 0;
    if (gameMap->playerFace == 0)
        frontPos = gameMap->playerPos - 32;
    if (gameMap->playerFace == 1)
        frontPos = gameMap->playerPos + 32;
    if (gameMap->playerFace == 2)
        frontPos = gameMap->playerPos + 1;
    if (gameMap->playerFace == 3)
        frontPos = gameMap->playerPos - 1;

    //prüfen ob sich Monster vor dem Char befinden
    short monsterID = -1;
    for (short m = 0; m < gameMap->monsterCount; m++)
        if (gameMap->monsterInfo[m][0] == frontPos)
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

//
// Char speichern
//

void CChar::saveChar(FILE* saveFile)
{
    //ID des Chars im Spiel
    fwrite(&charID, sizeof(short), 1, saveFile);

    //Pos des Chars im Charmenu
    fwrite(&menuPos, sizeof(short), 1, saveFile);

    //Bild dieses Chars
    fwrite(&imageID, sizeof(short), 1, saveFile);

    //Items in den Händen des Chars
    for (int i = 0; i < 2; i++)
        fwrite(&itemHand[i], sizeof(short), 1, saveFile);

    //Items im Inventar
    for (int i = 0; i < 14; i++)
        fwrite(&itemInventar[i], sizeof(short), 1, saveFile);

    //Kopf Item
    fwrite(&itemHead, sizeof(short), 1, saveFile);

    //Hals Item
    fwrite(&itemNeck, sizeof(short), 1, saveFile);

    //Brust Item
    fwrite(&itemTorso, sizeof(short), 1, saveFile);

    //Handgelenk Item
    fwrite(&itemWrist, sizeof(short), 1, saveFile);

    //Fuss Item
    fwrite(&itemFoot, sizeof(short), 1, saveFile);

    //Gürtel Items
    for (int i = 0; i < 3; i++)
        fwrite(&itemBelt[i], sizeof(short), 1, saveFile);

    //Name des Chars
    for (int i = 0; i < 32; i++)
        fwrite(&name[i], sizeof(char), 1, saveFile);

    //Klasse
    for (int i = 0; i < 32; i++)
        fwrite(&charclass[i], sizeof(char), 1, saveFile);

    //Ausrichtung
    for (int i = 0; i < 32; i++)
        fwrite(&social[i], sizeof(char), 1, saveFile);

    //Rasse
    for (int i = 0; i < 32; i++)
        fwrite(&race[i], sizeof(char), 1, saveFile);

    //Geschlecht
    for (int i = 0; i < 8; i++)
        fwrite(&gender[i], sizeof(char), 1, saveFile);

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
    fwrite(&arrows, sizeof(short), 1, saveFile);
}

//
// Char laden
//

void CChar::loadChar(FILE* saveFile)
{
    //ID des Chars im Spiel
    fread(&charID, sizeof(short), 1, saveFile);

    //Pos des Chars im Charmenu
    fread(&menuPos, sizeof(short), 1, saveFile);

    //Bild dieses Chars
    fread(&imageID, sizeof(short), 1, saveFile);

    //Items in den Händen des Chars
    for (int i = 0; i < 2; i++)
        fread(&itemHand[i], sizeof(short), 1, saveFile);

    //Items im Inventar
    for (int i = 0; i < 14; i++)
        fread(&itemInventar[i], sizeof(short), 1, saveFile);

    //Kopf Item
    fread(&itemHead, sizeof(short), 1, saveFile);

    //Hals Item
    fread(&itemNeck, sizeof(short), 1, saveFile);

    //Brust Item
    fread(&itemTorso, sizeof(short), 1, saveFile);

    //Handgelenk Item
    fread(&itemWrist, sizeof(short), 1, saveFile);

    //Fuss Item
    fread(&itemFoot, sizeof(short), 1, saveFile);

    //Gürtel Items
    for (int i = 0; i < 3; i++)
        fread(&itemBelt[i], sizeof(short), 1, saveFile);

    //Name des Chars
    for (int i = 0; i < 32; i++)
        fread(&name[i], sizeof(char), 1, saveFile);

    //Klasse
    for (int i = 0; i < 32; i++)
        fread(&charclass[i], sizeof(char), 1, saveFile);

    //Ausrichtung
    for (int i = 0; i < 32; i++)
        fread(&social[i], sizeof(char), 1, saveFile);

    //Rasse
    for (int i = 0; i < 32; i++)
        fread(&race[i], sizeof(char), 1, saveFile);

    //Geschlecht
    for (int i = 0; i < 8; i++)
        fread(&gender[i], sizeof(char), 1, saveFile);

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
    fread(&arrows, sizeof(short), 1, saveFile);
}
