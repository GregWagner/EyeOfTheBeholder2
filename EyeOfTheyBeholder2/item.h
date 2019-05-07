#pragma once

#include "event.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"

class Item {
public:
    void init(MediaWrapper* mediaObject, Language* languageData);

    void drawIcon(short itemID, short posX, short posY);

    short mLargePicID[112];
    short mItemTyp[112];
    char mItemName[112][128];

private:
    MediaWrapper* mMediaObject;
};
