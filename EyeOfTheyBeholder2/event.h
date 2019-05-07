#pragma once

#include "character.h"
#include "language.h"
#include "map.h"
#include "mediawrapper.h"

class Event {
public:
    void init(Map* map, MediaWrapper* mediaObject);

    // Delete events
    void clear();

    // Process mouse events
    void mouseState(short posX, short posY);

    // Various drawing operations
    void drawEventMessageBackground();
    void drawEventButton(char* text, short posX, short posY);

    // Event image load
    void loadEventImage(short imageID, short image);

    void update();
    void updateStatusMessage();

    // Send statu message to status window
    void statusMessage(char text[128]);

    // Texts
    Language* mLanguageData { nullptr };

    // Characters
    Char* mChars[4];

    short mEventType { -1 };

    // Map Position
    short mMapPos {};
    short mClickedMapPos {};

    bool mShowGameWindow {};
    bool mMapChange {};

    short mNewMapID {};
    short mWarpToPos {};
    short mFaceTo {};

    short mMousePosX {};
    short mMousePosY {};

    // Pause game when true
    bool mEventInProgress {};

private:
    MediaWrapper* mMediaObject { nullptr };

    // Event step forward
    short mProgress {};

    Map* mMap { nullptr };

    bool mMouseClicked {};

    // Status text - 2 lines
    char mStatusText[2][256];
    char mLine[256];

    // Timers
    long mTime, mTimeChange, mNextAnimFrame;

    // Event episode
    bool mSequenceEvent {};
};
