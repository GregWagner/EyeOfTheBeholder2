#pragma once

#include "character.h"
#include "charmenu.h"
#include "config.h"
#include "event.h"
#include "intro.h"
#include "item.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"
#include "monster.h"

class Engine {
public:
    // Create and view windows
    Engine();
    ~Engine() = default;

    void loadMap(short mapID, bool savedMap = false);

    void playIntro();
    void run();

    // Recreae 3D view
    void updateView();

    // Build 3D walls
    void drawStuffWindow(int level);
    void drawGameWindowNew();

    // Check collision
    bool checkWallCollision(short direction);
    bool checkMonsterCollision(short direction);

    void drawStuff(short mapWall, short wNr);
    void drawMonster(short mapWall, short wNr);
    void drawCompass();

    // Keyboard event handlers
    void handleDownKeyPressed();
    void handleUpKeyPressed();
    void handleLeftKeyPressed();
    void handleRightKeyPressed();

    // Mouse event handlers
    void mouseState();
    void mouseMove();
    void handleGameWindowClick(int mousePosX, int mousePosY);

    void saveGame();
    void loadGame();

    Config mConfiguration;

private:
    // Map info
    Map* mGameMap { nullptr };
    Map* mSaveStatusGameMap[33] {}; // Secure map status for a maximum of 16 maps when changing maps
    short mCurrentMapID {};
    Event* mMapEvent { nullptr };

    MediaWrapper mMediaObject;

    CharMenu mCharMenu;

    Language mLanguageData;

    Char mCharacter[4];
    Monster* mMonster[128];

    Item mItem;

    short mMouseItem {};
    short mMouseItemOld {};

    // Specify which background to draw
    bool mDrawBackground {};

    bool mIsKeyPressed {};

    // Mouse event
    bool mButtonLeft {};
    bool mButtonRight {};
    bool mButtonPressed {};
    int mMousePosX {};
    int mRealMouseX {};
    int mMousePosY {};
    int mRealMouseY {};
};
