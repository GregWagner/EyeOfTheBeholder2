#pragma once

#include "map.h"
#include "mediawrapper.h"

class Char {
public:
    void init(MediaWrapper* mediaObject, Map* gameMap);

    void doDamage(short damage);
    void attackNow(short handPos);

    void saveChar(FILE* saveFile);
    void loadChar(FILE* saveFile);

    Map* mGameMap { nullptr };

    short mCharacterID;
    short mMenuPos;
    short mImageID;

    short mItemsInHand[2];
    short mItemsInInventory[14];
    short mItemOnHead;
    short mItemAroundNeck;
    short mItemOnTorso;
    short mItemOnWrist;
    short mItemOnFeet;
    short mItemsOnBelt[3];
    short mNumberOfArrows;

    char mName[32];
    char mClass[32];
    char mSocial[32];
    char mRace[32];
    char mGender[8];

    //Hitpoints
    short hp;
    short currenthp;

    short food;
    short currentfood;

    short strength;
    short maxstrength;
    short intelligence;
    short wisdom;
    short dext;
    short constitution;
    short charisma;

    short armorclass;
    int exp;
    short lvl;

    // When true, character has taken damage
    bool damageTaken {};
    short damageTakenValue;

    // When true, character has done damage
    bool damageDone[2] {};
    short damageDoneValue[2];
    short doDamageToMonster;
    short doDamageToMonsterValue;

private:
    MediaWrapper* mediaObject { nullptr };
};
