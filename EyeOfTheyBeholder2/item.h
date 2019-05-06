#pragma once

#include "event.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"

class CItem {
public:
    //Init
    void init(MEDIAWrapper* mediaObject, CEvent* mapEvent, CLanguage* languageData);

    //Item Icon zeichnen
    void drawIcon(short itemID, short posX, short posY);

    //Events
    CEvent* mapEvent;

    //Texte
    CLanguage* languageData;

    //ID des Items im Spiel
    short itemID;

    //Bild Typ "gross" für dieses Item
    short largePicID[112];

    //Typ des Items
    short itemTyp[112];

    //Bezeichnung des Items
    char itemName[112][128];

    //Media Objekt
    MEDIAWrapper* mediaObject;
};
