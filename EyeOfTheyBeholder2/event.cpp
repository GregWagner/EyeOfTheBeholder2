#include "event.h"
#include <cstdio>

void Event::init(Map* map, MediaWrapper* mediaObject)
{
    mMediaObject = mediaObject;
    mMap = map;

    mTime = mediaObject->getMilliSeconds();
    mTimeChange = mTime + 20000;
    mStatusText[0][0] = mStatusText[1][0] = '\0';
}

// Process mouse events
void Event::mouseState(short posX, short posY)
{
    mMousePosX = posX;
    mMousePosY = posY;
    mMouseClicked = true;
}

// Background area for event text
void Event::drawEventMessageBackground()
{
    mMediaObject->fillRect(0, 121, 320, 79, 108, 108, 136);
    mMediaObject->fillRect(1, 121, 319, 1, 148, 148, 172);
    mMediaObject->fillRect(319, 121, 1, 79, 148, 148, 172);
    mMediaObject->fillRect(0, 199, 320, 1, 52, 52, 80);
    mMediaObject->fillRect(0, 121, 1, 79, 52, 52, 80);
}

// Button draw
void Event::drawEventButton(char* text, short posX, short posY)
{
    mMediaObject->fillRect(posX, posY, 95, 1, 148, 148, 172);
    mMediaObject->fillRect(posX + 95, posY, 1, 9, 148, 148, 172);
    mMediaObject->fillRect(posX, posY + 9, 95, 1, 52, 52, 80);
    mMediaObject->fillRect(posX, posY, 1, 9, 52, 52, 80);
    mMediaObject->drawText(0, posX + 5, posY, 0xFF, 0xFF, 0xFF, text);
}

void Event::update()
{
    bool validclick {};

    switch (mEventType) {
    case 1:
        /*
		 * Type 0001, teleport (when moved in) to map-position and show an optional image with text before with yes-no choice
		 * ""         teleport to-position lookat-position imageID(optional) stringlinenr(optional, 0-n) linecount(optional) eventmusic(optional,0=off,1-n)
		 */

        // if click takes place trigger event
        /*
        if (clickedMapPos  != 0) {
		    mapPos = clickedMapPos;
		    clickedMapPos = 0;
		}
        */

        // Event is not triggered by the click of the mouse
        if (mClickedMapPos) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
            break;
        }

        // Load graphics if needed, else perform teleport
        if (mProgress == 0) {
            // don't show graphics
            if (mMap->mapEventParam[mMapPos][2] == 0) {
                mMap->playerPos = mMap->mapEventParam[mMapPos][0];
                mMap->playerFace = mMap->mapEventParam[mMapPos][1];
                mProgress = mEventType = 0;
                if (mMap->mapEventParam[mMapPos][9] == 1) {
                    mMap->mapEvent[mMapPos] = 0;
                }
                mEventInProgress = false;

                // Display status message (if any)
                short lineNr = 0;
                if (mMap->mapEventParam[mMapPos][3] > 0) {
                    for (short t = mMap->mapEventParam[mMapPos][3]; t < mMap->mapEventParam[mMapPos][3] + mMap->mapEventParam[mMapPos][4]; t++) {
                        statusMessage(mLanguageData->mText[t]);
                        lineNr++;
                    }
                }
                break;
            }

            // temporary delete images
            mMediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
            // load imaged
            loadEventImage(241, mMap->mapEventParam[mMapPos][2]);
            // load and start music
            if (!mSequenceEvent && mMap->mapEventParam[mMapPos][5] > 0) {
                char musicfile[128];
                sprintf_s(musicfile, "sound/event%d", mMap->mapEventParam[mMapPos][5]);
                mMediaObject->loadSound(1, musicfile);
                mMediaObject->playSound(1);
            }
            mProgress = 1;
        } else if (mProgress == 1) {
            // view graphics and text, process mouse clicks
            mMediaObject->drawImage(240, 0, 0);
            mMediaObject->drawImage(241, 8, 7);
            drawEventMessageBackground();

            short lineNr = 0;
            for (short t = mMap->mapEventParam[mMapPos][3]; t < mMap->mapEventParam[mMapPos][3] + mMap->mapEventParam[mMapPos][4]; t++) {
                mMediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, mLanguageData->mText[t], false);
                lineNr++;
            }

            drawEventButton(mLanguageData->mText[39], 59, 188);
            drawEventButton(mLanguageData->mText[40], 166, 188);

            if (mMouseClicked) {
                // yes it was clicked
                if (mMousePosX >= 59 && mMousePosX <= 154 && mMousePosY >= 188 && mMousePosY <= 197) {
                    mMap->playerPos = mMap->mapEventParam[mMapPos][0];
                    mMap->playerFace = mMap->mapEventParam[mMapPos][1];
                    mProgress = mEventType = 0;
                    if (mMap->mapEventParam[mMapPos][9] == 1) {
                        mMap->mapEvent[mMapPos] = 0;
                    }
                    mEventInProgress = false;
                    mSequenceEvent = false;
                    mMediaObject->stopSound(1);
                    mMediaObject->freeSound(1);
                    break;
                }
                // No, mouse was not clicked
                if (mMousePosX >= 166 && mMousePosX <= 261 && mMousePosY >= 188 && mMousePosY <= 197) {
                    if (mMap->mapEventParam[mMapPos][9] == 1) {
                        mMap->mapEvent[mMapPos] = 0;
                    }
                    mProgress = mEventType = 0;
                    mEventInProgress = false;
                    mSequenceEvent = false;
                    mMediaObject->stopSound(1);
                    mMediaObject->freeSound(1);
                    break;
                }
            }
        }
        break;
    case 2:
        /*
		 * Type 0002, message out (when moved in), optional if a character is speaking
		 * ""		  message-out stringlinenr linecount(max: 2) charid(optional)
		 */

        // Event was not triggered by the click of a mouse
        if (mClickedMapPos) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
            break;
        }

        if (mProgress == 0) {
            // Put characters name in front of text (if necessary
            if (mLanguageData != NULL && mMap != NULL) {
                if (mMap->mapEventParam[mMapPos][2] > 0) {
                    sprintf_s(mLine, "\"%s\": %s", mChars[mMap->mapEventParam[mMapPos][3]]->mName, mLanguageData->mText[mMap->mapEventParam[mMapPos][0]]);
                } else {
                    sprintf_s(mLine, "%s", mLanguageData->mText[mMap->mapEventParam[mMapPos][0]]);
                }
                mProgress = 1;
            }
        } else if (mProgress == 1) {
            // Send text
            statusMessage(mLine);
            if (mMap->mapEventParam[mMapPos][1] > 1) {
                statusMessage(mLanguageData->mText[mMap->mapEventParam[mMapPos][0] + 1]);
            }
            mProgress = mEventType = 0;
            if (mMap->mapEventParam[mMapPos][9] == 1) {
                mMap->mapEvent[mMapPos] = 0;
            }
            mEventInProgress = false;
            mSequenceEvent = false;
        }
        break;
    case 3:
        /*
		 * Type 0003, changetileid (when clicked in) to tileid(must be avaiable in map anywhere!) and show an optional image with text before with yes-no choice, if yes items can be placed at the player position
		 * ""       changetileid newtileid imageID(optional) stringlinenr(optional, 0-n) linecount(optional) item1(optional) item2(optional) item3(optional)
		 */

        // If there is no click, there is no event
        if (mClickedMapPos == 0) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
        } else {
            //Check if an area has been defined in which needs to be clicked
            if (mProgress == 0 && mMap->mapEventParam[mClickedMapPos][6] > 0 && (mMousePosX >= mMap->mapEventParam[mClickedMapPos][6] && mMousePosX <= mMap->mapEventParam[mClickedMapPos][6] + mMap->mapEventParam[mClickedMapPos][8] && mMousePosY >= mMap->mapEventParam[mClickedMapPos][7] && mMousePosY <= mMap->mapEventParam[mClickedMapPos][7] + mMap->mapEventParam[mClickedMapPos][8])) {

            } else if (mProgress == 0 && mMap->mapEventParam[mClickedMapPos][6] > 0) {
                mProgress = mEventType = mClickedMapPos = 0;
                mEventInProgress = mShowGameWindow = false;
                break;
            }

            // Change graphics if necessary
            if (mProgress == 0) {
                // dont show graphics
                if (mMap->mapEventParam[mClickedMapPos][1] == 0) {
                    mShowGameWindow = true;
                    mProgress = 1;
                } else {
                    mMediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
                    // load images
                    loadEventImage(241, mMap->mapEventParam[mClickedMapPos][1]);
                    mProgress = 1;
                }
            } else if (mProgress == 1) {
                // View graphcs and text, process mouse clicks
                if (mMap->mapEventParam[mClickedMapPos][1] > 0) {
                    mMediaObject->drawImage(240, 0, 0);
                    mMediaObject->drawImage(241, 8, 7);
                }
                drawEventMessageBackground();
                short lineNr = 0;
                for (short t = mMap->mapEventParam[mClickedMapPos][2]; t < mMap->mapEventParam[mClickedMapPos][2] + mMap->mapEventParam[mClickedMapPos][3]; t++) {
                    mMediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, mLanguageData->mText[t], false);
                    lineNr++;
                }
                drawEventButton(mLanguageData->mText[39], 59, 188);
                drawEventButton(mLanguageData->mText[40], 166, 188);

                if (mMouseClicked) {
                    // Yes, it was clicked
                    if (mMousePosX >= 59 && mMousePosX <= 154 && mMousePosY >= 188 && mMousePosY <= 197) {
                        // change tile
                        mMap->cell_info[mClickedMapPos].wall_code = mMap->mapEventParam[mClickedMapPos][0];

                        // Put down items if necessary
                        for (short i = 0; i < 2; i++) {
                            if (mMap->mapEventParam[mClickedMapPos][4 + i] > 0) {
                                mMap->item[mMap->playerPos][mMap->item_counter[mMap->playerPos]] = mMap->mapEventParam[mClickedMapPos][4 + i];
                                mMap->item_floorpos[mMap->playerPos][mMap->item_counter[mMap->playerPos]] = 1;
                                mMap->item_counter[mMap->playerPos]++;
                            }
                        }

                        if (mMap->mapEventParam[mClickedMapPos][9] == 1)
                            mMap->mapEvent[mClickedMapPos] = 0;
                        mProgress = mEventType = mClickedMapPos = 0;
                        mEventInProgress = mShowGameWindow = false;
                        mSequenceEvent = false;
                        break;
                    }
                    // No, it was not clicked
                    if (mMousePosX >= 166 && mMousePosX <= 261 && mMousePosY >= 188 && mMousePosY <= 197) {
                        mProgress = mEventType = mClickedMapPos = 0;
                        mEventInProgress = mShowGameWindow = false;
                        mSequenceEvent = false;
                        break;
                    }
                }
            }
        }
        break;
    case 4:
        /*
		 * Type 0004, show an image with text before with yes-no choice
		 * ""		  showimage imageID stringlinenr(0-n) linecount do-event-id-after-yes(optional) do-event-id-after-no(optional)  OK-button-instead-yes_no (optional) eventmusic(optional,0=off,1-n), largepic (optional, 0=no,1=yes)
		 */

        // Event is not triggered by the click of a mouse
        if (mClickedMapPos) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
            break;
        }

        // load graphics
        if (mProgress == 0) {
            // temporary delete images
            mMediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
            // load images
            loadEventImage(241, mMap->mapEventParam[mMapPos][0]);
            // load music and start playing
            if (!mSequenceEvent && mMap->mapEventParam[mMapPos][6] > 0) {
                char musicfile[128];
                sprintf_s(musicfile, "sound/event%d", mMap->mapEventParam[mMapPos][6]);
                mMediaObject->loadSound(1, musicfile);
                mMediaObject->playSound(1);
            }

            mProgress = 1;
        } else if (mProgress == 1) {
            // View graphics and text, process mouse clicks
            // Behind and for image only draw with  "normal " small pictures, not with panoramic images
            if (mMap->mapEventParam[mMapPos][7] == 0) {
                mMediaObject->drawImage(240, 0, 0);
                mMediaObject->drawImage(241, 8, 7);
            } else {
                mMediaObject->drawImage(241, 0, 0);
            }

            drawEventMessageBackground();
            short lineNr = 0;
            for (short t = mMap->mapEventParam[mMapPos][1]; t < mMap->mapEventParam[mMapPos][1] + mMap->mapEventParam[mMapPos][2]; t++) {
                mMediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, mLanguageData->mText[t], false);
                lineNr++;
            }

            // only OK button
            if (mMap->mapEventParam[mMapPos][5] == 1) {
                drawEventButton(mLanguageData->mText[178], 59, 188);
            } else {
                // YES-NO button
                drawEventButton(mLanguageData->mText[39], 59, 188);
                drawEventButton(mLanguageData->mText[40], 166, 188);
            }

            if (mMouseClicked) {
                // YES or OK was clicked
                if (mMousePosX >= 59 && mMousePosX <= 154 && mMousePosY >= 188 && mMousePosY <= 197) {
                    mProgress = mEventType = 0;
                    if (mMap->mapEventParam[mMapPos][9] == 1) {
                        mMap->mapEvent[mMapPos] = 0;
                    }

                    // Trigger follow-up event
                    if (mMap->mapEventParam[mMapPos][3] > 0) {
                        short eventID = mMap->mapEventParam[mMapPos][3];
                        mEventType = mMap->mapEvent[eventID];
                        mMapPos = eventID;
                        mSequenceEvent = true;
                    } else {
                        mMediaObject->stopSound(1);
                        mMediaObject->freeSound(1);
                        mEventInProgress = false;
                        mSequenceEvent = false;
                    }
                    break;
                }
                // NO was clicked
                if (mMap->mapEventParam[mMapPos][5] != 1 && mMousePosX >= 166 && mMousePosX <= 261 && mMousePosY >= 188 && mMousePosY <= 197) {
                    if (mMap->mapEventParam[mMapPos][9] == 1) {
                        mMap->mapEvent[mMapPos] = 0;
                    }
                    mProgress = mEventType = 0;

                    // Trigger follow-up event
                    if (mMap->mapEventParam[mMapPos][4] > 0) {
                        short eventID = mMap->mapEventParam[mMapPos][4];
                        mEventType = mMap->mapEvent[eventID];
                        mMapPos = eventID;
                        mSequenceEvent = true;
                    } else {
                        mMediaObject->stopSound(1);
                        mMediaObject->freeSound(1);
                        mEventInProgress = false;
                        mSequenceEvent = false;
                    }
                    break;
                }
            }
        }
        break;
    case 5:
        /*
		 * Type 0005, change map
		 * changemap newmapID warpToPos (optional) faceTo (optional)
		 */

        // Event was not triggered by the click of the mouse
        if (mClickedMapPos) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
            break;
        }

        mMediaObject->stopSound(1);
        //mediaObject->freeSound(1);
        mMapChange = true;
        mNewMapID = mMap->mapEventParam[mMapPos][0];
        mWarpToPos = mMap->mapEventParam[mMapPos][1];
        mFaceTo = mMap->mapEventParam[mMapPos][2];
        mEventInProgress = false;
        mSequenceEvent = false;
        break;
    case 6:
        /*
		 * Type 0006, activate door with push-button at the given position
		 *            - no parameter
	     */

        validclick = false;

        // Event if triggered only the the click of the mouse
        if (mClickedMapPos) {
            if (mMousePosX >= 120 && mMousePosX <= 127 && mMousePosY >= 43 && mMousePosY <= 52) {
                validclick = true;

                // opening the door...
                if (mMap->cell_info[mMapPos].is_door && !mMap->cell_info[mMapPos].door_is_open) {
                    char musicfile[128];
                    sprintf_s(musicfile, "sound/door1_open", mMap->mapEventParam[mMapPos][6]);
                    mMediaObject->loadSound(1, musicfile);
                    mMediaObject->playSound(1);
                    mMap->cell_info[mMapPos].door_is_open = true;
                    mMap->mazeObjects->do_door_open = true;
                    mMap->mazeObjects->do_door_anim = true;
                } else if (mMap->cell_info[mMapPos].is_door && mMap->cell_info[mMapPos].door_is_open) {
                    // close the door
                    char musicfile[128];
                    sprintf_s(musicfile, "sound/door1_close", mMap->mapEventParam[mMapPos][6]);
                    mMediaObject->loadSound(1, musicfile);
                    mMediaObject->playSound(1);
                    mMap->cell_info[mMapPos].door_is_open = false;
                    mMap->mazeObjects->do_door_open = false;
                    mMap->mazeObjects->do_door_anim = true;
                }
                mProgress = mEventType = mClickedMapPos = 0;
                mEventInProgress = mShowGameWindow = false;
            }
        }
        if (!validclick) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
        }
        break;
    case 7:
        // If there is no click, then there is no event
        if (mClickedMapPos == 0) {
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
        } else {
            // Check if an area has been defined in which needs to be clicked
            if (mProgress == 0 && mMap->mapEventParam[mClickedMapPos][2] > 0 && (mMousePosX >= mMap->mapEventParam[mClickedMapPos][2] && mMousePosX <= mMap->mapEventParam[mClickedMapPos][2] + mMap->mapEventParam[mClickedMapPos][4] && mMousePosY >= mMap->mapEventParam[mClickedMapPos][3] && mMousePosY <= mMap->mapEventParam[mClickedMapPos][3] + mMap->mapEventParam[mClickedMapPos][4])) {

            } else if (mProgress == 0 && mMap->mapEventParam[mClickedMapPos][2] > 0) {
                mProgress = mEventType = mClickedMapPos = 0;
                mEventInProgress = mShowGameWindow = false;
                break;
            }

            // load music and start playing
            if (mMap->mapEventParam[mClickedMapPos][1] > 0) {
                char musicfile[128];
                sprintf_s(musicfile, "sound/event%d", mMap->mapEventParam[mClickedMapPos][1]);
                mMediaObject->loadSound(1, musicfile);
                mMediaObject->playSound(1);
            }

            // change tile
            mMap->cell_info[mClickedMapPos].wall_code = mMap->mapEventParam[mClickedMapPos][0];

            if (mMap->mapEventParam[mClickedMapPos][9] == 1) {
                mMap->mapEvent[mClickedMapPos] = 0;
            }
            mProgress = mEventType = mClickedMapPos = 0;
            mEventInProgress = mShowGameWindow = false;
        }
        break;

    default:
        break;
    }
    this->mMouseClicked = false;
}

void Event::loadEventImage(short imageID, short image)
{
    switch (image) {
    case 1:
        // Staircase down in dungeon
        mMediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/FOREST.PAL", 0, 96, 160, 96);
        break;
    case 2:
        // Old women
        mMediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/FOREST.PAL", 0, 0, 160, 96);
        break;
    case 3:
        // Darkmoon entrance
        mMediaObject->loadCPS(imageID, "original/DARKMOON.CPS", "original/FOREST.PAL", 0, 0, 320, 122);
        break;
    case 4:
        // Cleric - Darkmoon entrance
        mMediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/MEZZ.PAL", 160, 96, 160, 96);
        break;
    case 5:
        // to watch over
        mMediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/MEZZ.PAL", 160, 0, 160, 96);
        break;
    default:
        break;
    }
}

void Event::updateStatusMessage()
{
    mMediaObject->drawText(0, 3, 179, 0xFF, 0xFF, 0xFF, mStatusText[0]);
    mMediaObject->drawText(0, 5, 188, 0xFF, 0xFF, 0xFF, mStatusText[1]);
    mTime = mMediaObject->getMilliSeconds();
    // Scroll statue texts for 1 text every 10 seconds
    if (mTime >= mTimeChange) {
        sprintf_s(mStatusText[0], " %s", mStatusText[1]);
        mStatusText[1][0] = '\0';
        mTimeChange = mTime + 10000;
    }
}

// Send status text to the status window
void Event::statusMessage(char text[128])
{
    if (mStatusText[1][0] != '\0') {
        sprintf_s(mStatusText[0], " %s", mStatusText[1]);
    }
    sprintf_s(mStatusText[1], " %s", text);
    mTime = mMediaObject->getMilliSeconds();
    mTimeChange = mTime + 20000;
}

// Delete events
void Event::clear()
{
    mEventType = -1;
    mMapPos = 0;
    mClickedMapPos = 0;
    mProgress = 0;
    mShowGameWindow = false;
    mMapChange = false;
    mNewMapID = 0;
    mMousePosX = mMousePosY = 0;
    mMouseClicked = false;
    mEventInProgress = false;
    mSequenceEvent = false;
    //for(int i=240; i<=241; i++)
    //	mediaObject->freeImage(i);
}
