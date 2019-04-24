#pragma once

class CCharMenu {
public:
    //Init
    void initMenu(MEDIAWrapper* mediaObject, CEvent* mapEvent);

    //updaten
    void update();

    //die drei Menu Arten
    void drawPartyWindow();
    void drawInventar();
    void drawStats();

    //Character Portrait zeichnen
    void drawPortrait(short ID, short poxX, short posY);

    //Gesundheitsbalken klein zeichnen
    void drawSmallBar(short posX, short posY, short currentPos, short maxPos);

    //Gesundheitsbalken gross zeichnen
    void drawBigBar(short posX, short posY, short currentPos, short maxPos);

    //Extra Attack Button beim  Pocket PC
    void drawAttackButton(short posX, short posY);

    //Punkte-Overlay über Itemsymbol legen
    void drawItemOverlay(short posX, short posY);

    //Overlay "zu weit" über Itemsymbol legen
    void drawItemOverlayNotReachable(short posX, short posY);

    //Maus Events verarbeiten
    void mouseState(short posX, short posY, bool buttonLeft);

    //Inventar, Items
    void handleInventar4Char(short posX, short posY, bool buttonLeft);
    void handleInventarFull(short posX, short posY, short charID);

    //Camp Menu
    void drawCampMenu();
    void handleCampMenu(short posX, short posY);

    //Save/Load/Quit Menu
    void drawSaveMenu();
    void handleSaveMenu(short posX, short posY);

    //Rest Menu
    void drawRestMenu();
    void handleRestMenu(short posX, short posY);

    //Button für Campstyle-Menus
    void drawCampButton(short posX, short posY, short width, char text[64]);

    //Media Wrapper
    MEDIAWrapper* mediaObject;

    //Events
    CEvent* mapEvent;

    //4 Charachter Instanzen
    CChar* character[6];
    short menuPosID[6];

    //Items
    CItem* item;

    //Karte
    CMap* gameMap;

    //Mauszeiger
    short* mouseItem;
    short mouseItemOld;

    //aktuelle Position des Mauszeigers
    short mouseX, mouseY;

    //Texte
    CLanguage* languageData;

    // Menu Styles
    short menuStyle;

    //aktuell gewählter Char
    short charID;

    //Angriff für Hand 1 oder 2 des Chars im Gange oder nicht
    bool attackInProgress[6][2];

    //allgemeiner timer
    long timer;

    //Timer für Angriffe
    long attackTimer[6][2];
    short attackInterval;

    //Schadensanzeige am Char
    long damageTakenTimer[6];
    char damageTakenValue[6][3];
    long damageDoneTimer[6][2];
    char damageDoneValue[6][2][8];

    //Char Swap
    bool swapping;
    short swapFromPos;

    //Spiel laden/speichern auslösen
    bool loadGame, saveGame;
};
