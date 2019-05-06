
/**********************************
Class CEvent 
***********************************/
#include "event.h"
#include <cstdio>

void CEvent::init(CMap* map, MEDIAWrapper* mediaObject)
{
    this->mediaObject = NULL;
    this->mediaObject = mediaObject;
    languageData = NULL;
    eventTyp = -1;
    mapPos = 0;
    clickedMapPos = 0;
    progress = 0;
    sequenceEvent = false;
    showGameWindow = false;
    mapChange = false;
    newMapID = warpToPos = faceTo = 0;
    this->map = NULL;
    this->map = map;
    mousePosX = mousePosY = 0;
    mouseClicked = false;
    eventInProgress = false;
    time = mediaObject->getMilliSeconds();
    time_change = time + 20000;
    statusText[0][0] = statusText[1][0] = '\0';
}

//
// Maus Events verarbeiten
//

void CEvent::mouseState(short posX, short posY)
{
    this->mousePosX = posX;
    this->mousePosY = posY;
    this->mouseClicked = true;
}

//
// Hintergrund-Bereich für Event-Texte zeichnen
//

void CEvent::drawEventMessageBackground()
{
    mediaObject->fillRect(0, 121, 320, 79, 108, 108, 136);
    mediaObject->fillRect(1, 121, 319, 1, 148, 148, 172);
    mediaObject->fillRect(319, 121, 1, 79, 148, 148, 172);
    mediaObject->fillRect(0, 199, 320, 1, 52, 52, 80);
    mediaObject->fillRect(0, 121, 1, 79, 52, 52, 80);
}

//
// Button zeichnen
//

void CEvent::drawEventButton(char* text, short posX, short posY)
{
    mediaObject->fillRect(posX, posY, 95, 1, 148, 148, 172);
    mediaObject->fillRect(posX + 95, posY, 1, 9, 148, 148, 172);
    mediaObject->fillRect(posX, posY + 9, 95, 1, 52, 52, 80);
    mediaObject->fillRect(posX, posY, 1, 9, 52, 52, 80);
    mediaObject->drawText(0, posX + 5, posY, 0xFF, 0xFF, 0xFF, text);
}

//
//updaten
//

void CEvent::update()
{

    bool validclick;

    switch (eventTyp) {
    case 1:
        /*****
			Typ 0001, teleport (when moved in) to map-position and show an optional image with text before with yes-no choice
				""    teleport to-position lookat-position imageID(optional) stringlinenr(optional, 0-n) linecount(optional) eventmusic(optional,0=off,1-n)
			*****/

        //wenn Click erfolgt dann auch Event auslösen
        /*if(clickedMapPos  != 0)
			{
				mapPos = clickedMapPos;
				clickedMapPos = 0;
			}*/

        //Event wird nicht durch Mausklick ausgelöst
        if (clickedMapPos) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
            break;
        }

        //Grafiken laden wenn nötig oder teleport schon durchführen
        if (progress == 0) {
            //keine Grafik anzeigen
            if (map->mapEventParam[mapPos][2] == 0) {
                map->playerPos = map->mapEventParam[mapPos][0];
                map->playerFace = map->mapEventParam[mapPos][1];
                progress = eventTyp = 0;
                if (map->mapEventParam[mapPos][9] == 1)
                    map->mapEvent[mapPos] = 0;
                eventInProgress = false;

                //Statustext ausgeben wenn vorhanden
                short lineNr = 0;
                if (map->mapEventParam[mapPos][3] > 0)
                    for (short t = map->mapEventParam[mapPos][3]; t < map->mapEventParam[mapPos][3] + map->mapEventParam[mapPos][4]; t++) {
                        statusMessage(languageData->text[t]);
                        lineNr++;
                    }
                break;
            }

            //temp. Bilder löschen
            mediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
            //Bilder laden
            loadEventImage(241, map->mapEventParam[mapPos][2]);
            //Musik laden und starten
            if (!sequenceEvent && map->mapEventParam[mapPos][5] > 0) {
                char musicfile[128];
                sprintf(musicfile, "sound/event%d", map->mapEventParam[mapPos][5]);
                mediaObject->loadSound(1, musicfile);
                mediaObject->playSound(1);
            }
            progress = 1;
        }
        //Grafiken und Text anzeigen, Maus Klicks verarbeiten
        else if (progress == 1) {
            mediaObject->drawImage(240, 0, 0);
            mediaObject->drawImage(241, 8, 7);
            drawEventMessageBackground();

            short lineNr = 0;
            for (short t = map->mapEventParam[mapPos][3]; t < map->mapEventParam[mapPos][3] + map->mapEventParam[mapPos][4]; t++) {
                mediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, languageData->text[t], false);
                lineNr++;
            }

            drawEventButton(languageData->text[39], 59, 188);
            drawEventButton(languageData->text[40], 166, 188);

            if (mouseClicked) {
                //YES wurde geklickt
                if (mousePosX >= 59 && mousePosX <= 154 && mousePosY >= 188 && mousePosY <= 197) {
                    map->playerPos = map->mapEventParam[mapPos][0];
                    map->playerFace = map->mapEventParam[mapPos][1];
                    progress = eventTyp = 0;
                    if (map->mapEventParam[mapPos][9] == 1)
                        map->mapEvent[mapPos] = 0;
                    eventInProgress = false;
                    sequenceEvent = false;
                    mediaObject->stopSound(1);
                    mediaObject->freeSound(1);
                    break;
                }
                //NO wurde geklickt
                if (mousePosX >= 166 && mousePosX <= 261 && mousePosY >= 188 && mousePosY <= 197) {
                    if (map->mapEventParam[mapPos][9] == 1)
                        map->mapEvent[mapPos] = 0;
                    progress = eventTyp = 0;
                    eventInProgress = false;
                    sequenceEvent = false;
                    mediaObject->stopSound(1);
                    mediaObject->freeSound(1);
                    break;
                }
            }
        }

        break;
    case 2:
        /*****
			Typ 0002, message out (when moved in), optional if a character is speaking
			 ""		  message-out stringlinenr linecount(max: 2) charid(optional)
			*****/

        //Event wird nicht durch Mausklick ausgelöst
        if (clickedMapPos) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
            break;
        }

        if (progress == 0) {

            //Character Namen vor Text setzen wenn nötig
            if (languageData != NULL && map != NULL) {
                if (map->mapEventParam[mapPos][2] > 0)
                    sprintf(line, "\"%s\": %s", chars[map->mapEventParam[mapPos][3]]->name, languageData->text[map->mapEventParam[mapPos][0]]);
                else
                    sprintf(line, "%s", languageData->text[map->mapEventParam[mapPos][0]]);
                progress = 1;
            }
        } else if (progress == 1) {
            //Text ausgeben
            statusMessage(line);
            if (map->mapEventParam[mapPos][1] > 1)
                statusMessage(languageData->text[map->mapEventParam[mapPos][0] + 1]);
            progress = eventTyp = 0;
            if (map->mapEventParam[mapPos][9] == 1)
                map->mapEvent[mapPos] = 0;
            eventInProgress = false;
            sequenceEvent = false;
        }
        break;
    case 3:
        /*****
			Typ 0003, changetileid (when clicked in) to tileid(must be avaiable in map anywhere!) and show an optional image with text before with yes-no choice, if yes items can be placed at the player position
				""    changetileid newtileid imageID(optional) stringlinenr(optional, 0-n) linecount(optional) item1(optional) item2(optional) item3(optional)
			*****/

        //wenn kein Click erfolgt dann kein Event
        if (clickedMapPos == 0) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
        } else {
            //prüfen ob ein Bereich definiert wurde in den geklickt werden muss
            if (progress == 0 && map->mapEventParam[clickedMapPos][6] > 0 && (mousePosX >= map->mapEventParam[clickedMapPos][6] && mousePosX <= map->mapEventParam[clickedMapPos][6] + map->mapEventParam[clickedMapPos][8] && mousePosY >= map->mapEventParam[clickedMapPos][7] && mousePosY <= map->mapEventParam[clickedMapPos][7] + map->mapEventParam[clickedMapPos][8])) {

            } else if (progress == 0 && map->mapEventParam[clickedMapPos][6] > 0) {
                progress = eventTyp = clickedMapPos = 0;
                eventInProgress = showGameWindow = false;
                break;
            }

            //Grafiken laden wenn nötig
            if (progress == 0) {
                //keine Grafik anzeigen
                if (map->mapEventParam[clickedMapPos][1] == 0) {
                    showGameWindow = true;
                    progress = 1;
                } else {
                    mediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
                    //Bilder laden
                    loadEventImage(241, map->mapEventParam[clickedMapPos][1]);
                    progress = 1;
                }
            }

            //Grafiken und Text anzeigen, Maus Klicks verarbeiten
            else if (progress == 1) {
                if (map->mapEventParam[clickedMapPos][1] > 0) {
                    mediaObject->drawImage(240, 0, 0);
                    mediaObject->drawImage(241, 8, 7);
                }
                drawEventMessageBackground();
                short lineNr = 0;
                for (short t = map->mapEventParam[clickedMapPos][2]; t < map->mapEventParam[clickedMapPos][2] + map->mapEventParam[clickedMapPos][3]; t++) {
                    mediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, languageData->text[t], false);
                    lineNr++;
                }
                drawEventButton(languageData->text[39], 59, 188);
                drawEventButton(languageData->text[40], 166, 188);

                if (mouseClicked) {
                    //YES wurde geklickt
                    if (mousePosX >= 59 && mousePosX <= 154 && mousePosY >= 188 && mousePosY <= 197) {
                        //Tile ändern
                        map->cell_info[clickedMapPos].wall_code = map->mapEventParam[clickedMapPos][0];

                        //Items ablegen wenn nötig
                        for (short i = 0; i < 2; i++) {
                            if (map->mapEventParam[clickedMapPos][4 + i] > 0) {
                                map->item[map->playerPos][map->item_counter[map->playerPos]] = map->mapEventParam[clickedMapPos][4 + i];
                                map->item_floorpos[map->playerPos][map->item_counter[map->playerPos]] = 1;
                                map->item_counter[map->playerPos]++;
                            }
                        }

                        if (map->mapEventParam[clickedMapPos][9] == 1)
                            map->mapEvent[clickedMapPos] = 0;
                        progress = eventTyp = clickedMapPos = 0;
                        eventInProgress = showGameWindow = false;
                        sequenceEvent = false;
                        break;
                    }
                    //NO wurde geklickt
                    if (mousePosX >= 166 && mousePosX <= 261 && mousePosY >= 188 && mousePosY <= 197) {
                        progress = eventTyp = clickedMapPos = 0;
                        eventInProgress = showGameWindow = false;
                        sequenceEvent = false;
                        break;
                    }
                }
            }
        }
        break;
    case 4:
        /*****
				Type 0004, show an image with text before with yes-no choice
				""		showimage imageID stringlinenr(0-n) linecount do-event-id-after-yes(optional) do-event-id-after-no(optional)  OK-button-instead-yes_no (optional) eventmusic(optional,0=off,1-n), largepic (optional, 0=no,1=yes)
			*****/

        //Event wird nicht durch Mausklick ausgelöst
        if (clickedMapPos) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
            break;
        }

        //Grafiken laden
        if (progress == 0) {
            //temp. Bilder löschen
            mediaObject->loadCPS(240, "original/BORDER.CPS", "original/FOREST.PAL", 0, 0, 184, 122);
            //Bilder laden
            loadEventImage(241, map->mapEventParam[mapPos][0]);
            //Musik laden und starten
            if (!sequenceEvent && map->mapEventParam[mapPos][6] > 0) {
                char musicfile[128];
                sprintf(musicfile, "sound/event%d", map->mapEventParam[mapPos][6]);
                mediaObject->loadSound(1, musicfile);
                mediaObject->playSound(1);
            }

            progress = 1;
        }
        //Grafiken und Text anzeigen, Maus Klicks verarbeiten
        else if (progress == 1) {
            //Hintergund für Bild nur bei "normalen" kleinen Bildern zeichnen, nicht bei Panoramabildern
            if (map->mapEventParam[mapPos][7] == 0) {
                mediaObject->drawImage(240, 0, 0);
                mediaObject->drawImage(241, 8, 7);
            } else
                mediaObject->drawImage(241, 0, 0);

            drawEventMessageBackground();
            short lineNr = 0;
            for (short t = map->mapEventParam[mapPos][1]; t < map->mapEventParam[mapPos][1] + map->mapEventParam[mapPos][2]; t++) {
                mediaObject->drawText(0, 5, 122 + (lineNr * 7), 0xFF, 0xFF, 0xFF, languageData->text[t], false);
                lineNr++;
            }

            //nur OK button
            if (map->mapEventParam[mapPos][5] == 1) {
                drawEventButton(languageData->text[178], 59, 188);
            }
            //YES - NO button
            else {
                drawEventButton(languageData->text[39], 59, 188);
                drawEventButton(languageData->text[40], 166, 188);
            }

            if (mouseClicked) {
                //YES oder OK wurde geklickt
                if (mousePosX >= 59 && mousePosX <= 154 && mousePosY >= 188 && mousePosY <= 197) {
                    progress = eventTyp = 0;
                    if (map->mapEventParam[mapPos][9] == 1)
                        map->mapEvent[mapPos] = 0;

                    //Folge-Event auslösen
                    if (map->mapEventParam[mapPos][3] > 0) {
                        short eventID = map->mapEventParam[mapPos][3];
                        eventTyp = map->mapEvent[eventID];
                        mapPos = eventID;
                        sequenceEvent = true;
                    } else {
                        mediaObject->stopSound(1);
                        mediaObject->freeSound(1);
                        eventInProgress = false;
                        sequenceEvent = false;
                    }

                    break;
                }
                //NO wurde geklickt
                if (map->mapEventParam[mapPos][5] != 1 && mousePosX >= 166 && mousePosX <= 261 && mousePosY >= 188 && mousePosY <= 197) {
                    if (map->mapEventParam[mapPos][9] == 1)
                        map->mapEvent[mapPos] = 0;
                    progress = eventTyp = 0;

                    //Folge-Event auslösen
                    if (map->mapEventParam[mapPos][4] > 0) {
                        short eventID = map->mapEventParam[mapPos][4];
                        eventTyp = map->mapEvent[eventID];
                        mapPos = eventID;
                        sequenceEvent = true;
                    } else {
                        mediaObject->stopSound(1);
                        mediaObject->freeSound(1);
                        eventInProgress = false;
                        sequenceEvent = false;
                    }
                    break;
                }
            }
        }

        break;
    case 5:
        /*****
			Type 0005, change map
			changemap newmapID warpToPos (optional) faceTo (optional)
			*****/

        //Event wird nicht durch Mausklick ausgelöst
        if (clickedMapPos) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
            break;
        }

        mediaObject->stopSound(1);
        //mediaObject->freeSound(1);
        mapChange = true;
        newMapID = map->mapEventParam[mapPos][0];
        warpToPos = map->mapEventParam[mapPos][1];
        faceTo = map->mapEventParam[mapPos][2];
        eventInProgress = false;
        sequenceEvent = false;

        break;
    case 6:
        /*****
			Type 0006, activate door with push-button at the given position
			- keine Parameter
			*****/

        validclick = false;

        //Event wird nur durch Mausklick ausgelöst
        if (clickedMapPos) {
            if (mousePosX >= 120 && mousePosX <= 127 && mousePosY >= 43 && mousePosY <= 52) {
                validclick = true;

                //Tür öffnen
                if (map->cell_info[mapPos].is_door && !map->cell_info[mapPos].door_is_open) {
                    char musicfile[128];
                    sprintf(musicfile, "sound/door1_open", map->mapEventParam[mapPos][6]);
                    mediaObject->loadSound(1, musicfile);
                    mediaObject->playSound(1);
                    map->cell_info[mapPos].door_is_open = true;
                    map->mazeObjects->do_door_open = true;
                    map->mazeObjects->do_door_anim = true;
                }
                //Tür schließen
                else if (map->cell_info[mapPos].is_door && map->cell_info[mapPos].door_is_open) {
                    char musicfile[128];
                    sprintf(musicfile, "sound/door1_close", map->mapEventParam[mapPos][6]);
                    mediaObject->loadSound(1, musicfile);
                    mediaObject->playSound(1);
                    map->cell_info[mapPos].door_is_open = false;
                    map->mazeObjects->do_door_open = false;
                    map->mazeObjects->do_door_anim = true;
                }

                progress = eventTyp = clickedMapPos = 0;
                eventInProgress = showGameWindow = false;
            }
        }
        if (!validclick) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
            break;
        }

        break;
    case 7:

        //wenn kein Click erfolgt dann kein Event
        if (clickedMapPos == 0) {
            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
        } else {
            //prüfen ob ein Bereich definiert wurde in den geklickt werden muss
            if (progress == 0 && map->mapEventParam[clickedMapPos][2] > 0 && (mousePosX >= map->mapEventParam[clickedMapPos][2] && mousePosX <= map->mapEventParam[clickedMapPos][2] + map->mapEventParam[clickedMapPos][4] && mousePosY >= map->mapEventParam[clickedMapPos][3] && mousePosY <= map->mapEventParam[clickedMapPos][3] + map->mapEventParam[clickedMapPos][4])) {

            } else if (progress == 0 && map->mapEventParam[clickedMapPos][2] > 0) {
                progress = eventTyp = clickedMapPos = 0;
                eventInProgress = showGameWindow = false;
                break;
            }

            //Musik/Sound laden und starten
            if (map->mapEventParam[clickedMapPos][1] > 0) {
                char musicfile[128];
                sprintf(musicfile, "sound/event%d", map->mapEventParam[clickedMapPos][1]);
                mediaObject->loadSound(1, musicfile);
                mediaObject->playSound(1);
            }

            //Tile ändern
            map->cell_info[clickedMapPos].wall_code = map->mapEventParam[clickedMapPos][0];

            if (map->mapEventParam[clickedMapPos][9] == 1)
                map->mapEvent[clickedMapPos] = 0;

            progress = eventTyp = clickedMapPos = 0;
            eventInProgress = showGameWindow = false;
        }
        break;

    default:
        break;
    }

    this->mouseClicked = false;
}

//
// Event Image laden
//

void CEvent::loadEventImage(short imageID, short image)
{
    switch (image) {
    case 1:
        //Treppe hinab in Dungeon
        mediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/FOREST.PAL", 0, 96, 160, 96);
        break;
    case 2:
        //alte Frau
        mediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/FOREST.PAL", 0, 0, 160, 96);
        break;
    case 3:
        //Darkmoon Eingang
        mediaObject->loadCPS(imageID, "original/DARKMOON.CPS", "original/FOREST.PAL", 0, 0, 320, 122);
        break;
    case 4:
        //Kleriker - Darkmoon Eingang
        mediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/MEZZ.PAL", 160, 96, 160, 96);
        break;
    case 5:
        //Wachen
        mediaObject->loadCPS(imageID, "original/SOUT1.CPS", "original/MEZZ.PAL", 160, 0, 160, 96);
        break;
    default:
        break;
    }
}

//
// Statustext updaten
//

void CEvent::updateStatusMessage()
{
    mediaObject->drawText(0, 3, 179, 0xFF, 0xFF, 0xFF, statusText[0]);
    mediaObject->drawText(0, 5, 188, 0xFF, 0xFF, 0xFF, statusText[1]);
    time = mediaObject->getMilliSeconds();
    //alle 10 Sekunden Statustexte um 1 Text scrollen
    if (time >= time_change) {
        sprintf(statusText[0], " %s", statusText[1]);
        statusText[1][0] = '\0';
        time_change = time + 10000;
    }
}

//
// Statustext im Statusfenster ausgeben
//

void CEvent::statusMessage(char text[128])
{
    if (statusText[1][0] != '\0')
        sprintf(statusText[0], " %s", statusText[1]);
    sprintf(statusText[1], " %s", text);
    time = mediaObject->getMilliSeconds();
    time_change = time + 20000;
}

//
// Events löschen
//

void CEvent::clear()
{
    eventTyp = -1;
    mapPos = 0;
    clickedMapPos = 0;
    progress = 0;
    showGameWindow = false;
    mapChange = false;
    newMapID = 0;
    mousePosX = mousePosY = 0;
    mouseClicked = false;
    eventInProgress = false;
    sequenceEvent = false;
    //for(int i=240; i<=241; i++)
    //	mediaObject->freeImage(i);
}
