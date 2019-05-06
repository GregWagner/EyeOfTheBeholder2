
/**********************************
Class CCharMenu
***********************************/

#include "charmenu.h"
#include <cstdio>

void CCharMenu::initMenu(MEDIAWrapper* mediaObject, CEvent* mapEvent)
{
    this->mediaObject = mediaObject;
    this->mapEvent = mapEvent;
    menuStyle = 0;
    charID = 0;
    swapping = loadGame = saveGame = false;
    swapFromPos = -1;
    for (short i = 0; i < 6; i++) {
        attackInProgress[i][0] = false;
        attackInProgress[i][1] = false;
        damageTakenTimer[i] = 0;
        damageTakenValue[i][0] = '\0';
        damageDoneTimer[i][0] = 0;
        damageDoneTimer[i][1] = 0;
        damageDoneValue[i][0][0] = '\0';
        damageDoneValue[i][1][0] = '\0';
        attackTimer[i][0] = 0;
        attackTimer[i][1] = 0;
        menuPosID[i] = i;
    }
    attackInterval = 3000;

    //Menu Grafiken laden
    mediaObject->loadCPS(200, "original/PLAYFLD.CPS", "original/SILVER.PAL", 182, 0, 138, 107);
    mediaObject->loadCPS(201, "original/INVENT.CPS", "original/SILVER.PAL", 178, 0, 142, 167);
    mediaObject->loadCPS(202, "original/INVENT.CPS", "original/SILVER.PAL", 178, 0, 142, 167);

    //Character Portrait Grafiken laden, Quelle: CHARGENA.CPS und SILVER.PAL
    short posX = ((character[menuPosID[0]]->imageID % 10)) * 32;
    short posY = character[menuPosID[0]]->imageID / 10 * 32;
    mediaObject->loadCPS(220, "original/CHARGENA.CPS", "original/SILVER.PAL", posX, posY, 31, 32);

    posX = ((character[menuPosID[1]]->imageID % 10)) * 32;
    posY = character[menuPosID[1]]->imageID / 10 * 32;
    mediaObject->loadCPS(221, "original/CHARGENA.CPS", "original/SILVER.PAL", posX, posY, 31, 32);

    posX = ((character[menuPosID[2]]->imageID % 10)) * 32;
    posY = character[menuPosID[2]]->imageID / 10 * 32;
    mediaObject->loadCPS(222, "original/CHARGENA.CPS", "original/SILVER.PAL", posX, posY, 31, 32);

    posX = ((character[menuPosID[3]]->imageID % 10)) * 32;
    posY = character[menuPosID[3]]->imageID / 10 * 32;
    mediaObject->loadCPS(223, "original/CHARGENA.CPS", "original/SILVER.PAL", posX, posY, 31, 32);
}

//
// 4 Fenster Party Ansicht zeichnen
//

void CCharMenu::drawPartyWindow()
{
    mediaObject->drawImage(200, 182, 0);
}

//
// Inventar zeichnen
//

void CCharMenu::drawInventar()
{
    mediaObject->drawImage(201, 182, 0);
    mediaObject->fillRect(219, 14, 32, 19, 108, 108, 136);
}

//
// Stats zeichnen
//

void CCharMenu::drawStats()
{
    mediaObject->drawImage(202, 182, 0);
    mediaObject->fillRect(219, 14, 32, 19, 108, 108, 136);
    mediaObject->fillRect(184, 36, 93, 130, 108, 108, 136);
    mediaObject->fillRect(277, 51, 47, 97, 108, 108, 136);
    mediaObject->fillRect(277, 148, 29, 18, 108, 108, 136);
}

//
// Character Portraits zeichnen
//

void CCharMenu::drawPortrait(short ID, short posX, short posY)
{
    mediaObject->drawImage(220 + ID, posX, posY);

    //wenn Char auf < HP runter ist aber noch lebt Schleier über Bild legen
    if (character[ID]->currenthp < 10 && character[ID]->currenthp > 0)
        mediaObject->drawImage(165, posX, posY, 33, 88, 31, 32);

    //wenn Char tot dann Totenkopf
    if (character[ID]->currenthp == 0) {
        mediaObject->fillRect(posX, posY, 31, 32, 0, 0, 0);
        mediaObject->drawImage(165, posX, posY, 1, 88, 31, 32);
    }
}

//
// Gesundheitsbalken klein zeichnen
//

void CCharMenu::drawSmallBar(short posX, short posY, short currentPos, short maxPos)
{
    mediaObject->fillRect(posX + 1, posY + 0, 40, 1, 52, 52, 80);
    mediaObject->fillRect(posX + 40, posY + 1, 1, 3, 52, 52, 80);
    mediaObject->fillRect(posX + 0, posY + 4, 41, 1, 148, 148, 172);
    mediaObject->fillRect(posX + 0, posY + 0, 1, 4, 148, 148, 172);
    mediaObject->fillRect(posX + 1, posY + 1, 39, 3, 88, 88, 116);
    mediaObject->fillRect(posX + 1, posY + 1, (float)currentPos / (float)maxPos * 39, 3, 0, 168, 0);
}

//
// Gesundheitsbalken gross zeichnen
//

void CCharMenu::drawBigBar(short posX, short posY, short currentPos, short maxPos)
{
    mediaObject->fillRect(posX + 1, posY + 0, 53, 1, 52, 52, 80);
    mediaObject->fillRect(posX + 53, posY + 1, 1, 5, 52, 52, 80);
    mediaObject->fillRect(posX + 0, posY + 6, 54, 1, 148, 148, 172);
    mediaObject->fillRect(posX + 0, posY + 0, 1, 6, 148, 148, 172);
    mediaObject->fillRect(posX + 1, posY + 1, 52, 5, 88, 88, 116);
    mediaObject->fillRect(posX + 1, posY + 1, (float)currentPos / (float)maxPos * 52, 5, 0, 168, 0);
}

//
// Extra Attackbutton beim PPC
//

void CCharMenu::drawAttackButton(short posX, short posY)
{
    mediaObject->fillRect(posX, posY, 7, 7, 255, 0, 0);
    mediaObject->fillRect(posX + 1, posY, 6, 1, 255, 96, 96);
    mediaObject->fillRect(posX + 6, posY, 1, 6, 255, 96, 96);
    mediaObject->fillRect(posX, posY + 6, 7, 1, 183, 6, 6);
    mediaObject->fillRect(posX, posY, 1, 7, 183, 6, 6);
    mediaObject->drawText(1, posX + 1, posY - 2, 255, 255, 255, "A");
}

//
//Punkte-Overlay über Itemsymbol legen
//

void CCharMenu::drawItemOverlay(short posX, short posY)
{
    short x_offset = 0;
    for (short y = posY; y < posY + 16; y++) {
        for (short x = posX; x < posX + 30; x++) {
            if (x % 2)
                mediaObject->fillRect(x + x_offset, y, 1, 1, 0x00, 0x00, 0x00);
        }
        x_offset = x_offset == 1 ? 0 : 1;
    }
}

//
//Overlay "zu weit" über Itemsymbol legen
//

void CCharMenu::drawItemOverlayNotReachable(short posX, short posY)
{
    mediaObject->fillRect(posX + 1, posY, 30, 1, 240, 72, 68);
    mediaObject->fillRect(posX + 30, posY, 1, 15, 240, 72, 68);
    mediaObject->fillRect(posX, posY + 15, 31, 1, 112, 8, 32);
    mediaObject->fillRect(posX, posY, 1, 16, 112, 8, 32);
    mediaObject->fillRect(posX + 1, posY + 1, 29, 14, 184, 8, 8);
    mediaObject->drawText(0, posX + 1, posY, 255, 255, 255, languageData->text[180]);
    mediaObject->drawText(0, posX + 1, posY + 6, 255, 255, 255, languageData->text[181]);
}

//
// Maus Klicks verarbeiten
//

void CCharMenu::mouseState(short posX, short posY, bool buttonLeft = true)
{
    mouseItemOld = *mouseItem;

    if (menuStyle == 0) {
        //
        // bei Klick auf Camp Button Camp Menu öffnen
        //

        if (buttonLeft) {
            if (posX >= 290 && posX <= 320 && posY >= 179 && posY <= 198) {
                menuStyle = 3;
            }
        }

        //
        // bei Klick auf ein Char Icon in Inventar Ansicht wechseln
        //
        if (buttonLeft) {
            if (posX >= 185 && posX <= 216 && posY >= 11 && posY <= 43) {
                //Char mit ID 0 wurde gewählt
                charID = 0;
                menuStyle = 1;
            }

            else if (posX >= 257 && posX <= 288 && posY >= 11 && posY <= 43) {
                //Char mit ID 1 wurde gewählt
                charID = 1;
                menuStyle = 1;
            }

            else if (posX >= 185 && posX <= 216 && posY >= 63 && posY <= 95) {
                //Char mit ID 2 wurde gewählt
                charID = 2;
                menuStyle = 1;
            }

            else if (posX >= 257 && posX <= 288 && posY >= 63 && posY <= 95) {
                //Char mit ID 3 wurde gewählt
                charID = 3;
                menuStyle = 1;
            }
        }
        handleInventar4Char(posX, posY, buttonLeft);

    } else if (menuStyle == 1 && buttonLeft) {
        //Klick auf Char Icon wird dieses Menu wieder geschlossen
        if (posX >= 186 && posX <= 218 && posY >= 3 && posY <= 35)
            menuStyle = 0;

        //Klick auf Blattkante wechselt zum Stats Menu
        else if (posX >= 306 && posX <= 320 && posY >= 148 && posY <= 167)
            menuStyle = 2;

        //Klick auf Pfeil rechts wechselt zum nächsten Char
        else if (posX >= 302 && posX <= 320 && posY >= 36 && posY <= 50)
            charID = charID == 3 ? 0 : charID + 1;

        //Klick auf Pfeil links wechselt zum vorigen Char
        else if (posX >= 279 && posX <= 298 && posY >= 36 && posY <= 50)
            charID = charID == 0 ? 3 : charID - 1;

        else
            handleInventarFull(posX, posY, charID);

    } else if (menuStyle == 2 && buttonLeft) {
        //Klick auf Char Icon wird dieses Menu wieder geschlossen
        if (posX >= 186 && posX <= 218 && posY >= 3 && posY <= 35)
            menuStyle = 0;

        //Klick auf Blattkante wechselt zum Inventar Menu
        else if (posX >= 306 && posX <= 320 && posY >= 148 && posY <= 167)
            menuStyle = 1;

        //Klick auf Pfeil rechts wechselt zum nächsten Char
        else if (posX >= 302 && posX <= 320 && posY >= 36 && posY <= 50)
            charID = charID == 3 ? 0 : charID + 1;

        //Klick auf Pfeil links wechselt zum vorigen Char
        else if (posX >= 279 && posX <= 298 && posY >= 36 && posY <= 50)
            charID = charID == 0 ? 3 : charID - 1;
    } else if (menuStyle == 3 && buttonLeft)
        handleCampMenu(posX, posY);
    else if (menuStyle == 4 && buttonLeft)
        handleSaveMenu(posX, posY);
    else if (menuStyle == 5 && buttonLeft)
        handleRestMenu(posX, posY);

    if (this->mouseItem != NULL && mouseItemOld != *this->mouseItem && *this->mouseItem != 0) {
        char temp[256];
        sprintf(temp, "%s %s", item->itemName[*this->mouseItem], languageData->text[42]);
        mapEvent->statusMessage(temp);
    }
}

//
// Inventar im 4-Char-Bild Modus
//

void CCharMenu::handleInventar4Char(short posX, short posY, bool buttonLeft)
{
    //
    // Klick auf Hand nimmt Item auf
    //

    short memory;
    short ppc_offset = 0;

//Platz für Extra Attack-Button beim Pocket PC
#ifdef WINCE
    ppc_offset = 7;
#endif

    //Char 0
    if (posX >= 217 && posX <= 246 && posY >= 12 && posY <= 26) {
        if (buttonLeft && posX <= 246 - ppc_offset) {
            if (character[menuPosID[0]]->itemHand[0] != 85) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[0]]->itemHand[0];
                if (memory > 0)
                    character[menuPosID[0]]->itemHand[0] = memory;
                else
                    character[menuPosID[0]]->itemHand[0] = 85;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[0]]->itemHand[0];
                    character[menuPosID[0]]->itemHand[0] = *this->mouseItem;
                    if (memory == 85)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[0]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[0]][0] && item->itemTyp[character[menuPosID[0]]->itemHand[0]] == 1) {
                attackTimer[menuPosID[0]][0] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[0]][0] = true;
                if (character[menuPosID[0]]->charID <= 1)
                    character[menuPosID[0]]->attackNow(0);
            } else if (!attackInProgress[menuPosID[0]][0] && item->itemTyp[character[menuPosID[0]]->itemHand[0]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }
    if (posX >= 217 && posX <= 246 && posY >= 28 && posY <= 42) {
        if (buttonLeft && posX <= 246 - ppc_offset) {
            if (character[menuPosID[0]]->itemHand[1] != 86) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[0]]->itemHand[1];
                if (memory > 0)
                    character[menuPosID[0]]->itemHand[1] = memory;
                else
                    character[menuPosID[0]]->itemHand[1] = 86;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[0]]->itemHand[1];
                    character[menuPosID[0]]->itemHand[1] = *this->mouseItem;
                    if (memory == 86)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[0]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[0]][1] && item->itemTyp[character[menuPosID[0]]->itemHand[1]] == 1) {
                attackTimer[menuPosID[0]][1] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[0]][1] = true;
                if (character[menuPosID[0]]->charID <= 1)
                    character[menuPosID[0]]->attackNow(1);
            } else if (!attackInProgress[menuPosID[0]][1] && item->itemTyp[character[menuPosID[0]]->itemHand[1]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }

    //Char 1
    if (posX >= 289 && posX <= 318 && posY >= 12 && posY <= 26) {
        if (buttonLeft && posX <= 318 - ppc_offset) {
            if (character[menuPosID[1]]->itemHand[0] != 85) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[1]]->itemHand[0];
                if (memory > 0)
                    character[menuPosID[1]]->itemHand[0] = memory;
                else
                    character[menuPosID[1]]->itemHand[0] = 85;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[1]]->itemHand[0];
                    character[menuPosID[1]]->itemHand[0] = *this->mouseItem;
                    if (memory == 85)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[1]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[1]][0] && item->itemTyp[character[menuPosID[1]]->itemHand[0]] == 1) {
                attackTimer[menuPosID[1]][0] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[1]][0] = true;
                if (character[menuPosID[1]]->charID <= 1)
                    character[menuPosID[1]]->attackNow(0);
            } else if (!attackInProgress[menuPosID[1]][0] && item->itemTyp[character[menuPosID[1]]->itemHand[0]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }
    if (posX >= 289 && posX <= 318 && posY >= 28 && posY <= 42) {
        if (buttonLeft && posX <= 318 - ppc_offset) {
            if (character[menuPosID[1]]->itemHand[1] != 86) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[1]]->itemHand[1];
                if (memory > 0)
                    character[menuPosID[1]]->itemHand[1] = memory;
                else
                    character[menuPosID[1]]->itemHand[1] = 86;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[1]]->itemHand[1];
                    character[menuPosID[1]]->itemHand[1] = *this->mouseItem;
                    if (memory == 86)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[1]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[1]][1] && item->itemTyp[character[menuPosID[1]]->itemHand[1]] == 1) {
                attackTimer[menuPosID[1]][1] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[1]][1] = true;
                if (character[menuPosID[1]]->charID <= 1)
                    character[menuPosID[1]]->attackNow(1);
            } else if (!attackInProgress[menuPosID[1]][1] && item->itemTyp[character[menuPosID[1]]->itemHand[1]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }

    //Char 2
    if (posX >= 217 && posX <= 246 && posY >= 64 && posY <= 78) {
        if (buttonLeft && posX <= 246 - ppc_offset) {
            if (character[menuPosID[2]]->itemHand[0] != 85) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[2]]->itemHand[0];
                if (memory > 0)
                    character[menuPosID[2]]->itemHand[0] = memory;
                else
                    character[menuPosID[2]]->itemHand[0] = 85;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[2]]->itemHand[0];
                    character[menuPosID[2]]->itemHand[0] = *this->mouseItem;
                    if (memory == 85)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[2]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[2]][0] && item->itemTyp[character[menuPosID[2]]->itemHand[0]] == 1) {
                attackTimer[menuPosID[2]][0] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[2]][0] = true;
                if (character[menuPosID[2]]->charID <= 1)
                    character[menuPosID[2]]->attackNow(0);
            } else if (!attackInProgress[menuPosID[2]][0] && item->itemTyp[character[menuPosID[2]]->itemHand[0]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }
    if (posX >= 217 && posX <= 246 && posY >= 80 && posY <= 94) {
        if (buttonLeft && posX <= 246 - ppc_offset) {
            if (character[menuPosID[2]]->itemHand[1] != 86) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[2]]->itemHand[1];
                if (memory > 0)
                    character[menuPosID[2]]->itemHand[1] = memory;
                else
                    character[menuPosID[2]]->itemHand[1] = 86;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[2]]->itemHand[1];
                    character[menuPosID[2]]->itemHand[1] = *this->mouseItem;
                    if (memory == 86)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[2]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[2]][1] && item->itemTyp[character[menuPosID[2]]->itemHand[1]] == 1) {
                attackTimer[menuPosID[2]][1] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[2]][1] = true;
                if (character[menuPosID[2]]->charID <= 1)
                    character[menuPosID[2]]->attackNow(1);
            } else if (!attackInProgress[menuPosID[2]][1] && item->itemTyp[character[menuPosID[2]]->itemHand[1]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }

    //Char 3
    if (posX >= 289 && posX <= 318 && posY >= 64 && posY <= 78) {
        if (buttonLeft && posX <= 318 - ppc_offset) {
            if (character[menuPosID[3]]->itemHand[0] != 85) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[3]]->itemHand[0];
                if (memory > 0)
                    character[menuPosID[3]]->itemHand[0] = memory;
                else
                    character[menuPosID[3]]->itemHand[0] = 85;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[3]]->itemHand[0];
                    character[menuPosID[3]]->itemHand[0] = *this->mouseItem;
                    if (memory == 85)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[3]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[3]][0] && item->itemTyp[character[menuPosID[3]]->itemHand[0]] == 1) {
                attackTimer[menuPosID[3]][0] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[3]][0] = true;
                if (character[menuPosID[3]]->charID <= 1)
                    character[menuPosID[3]]->attackNow(0);
            } else if (!attackInProgress[menuPosID[3]][0] && item->itemTyp[character[menuPosID[3]]->itemHand[0]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }
    if (posX >= 289 && posX <= 318 && posY >= 80 && posY <= 94) {
        if (buttonLeft && posX <= 318 - ppc_offset) {
            if (character[menuPosID[3]]->itemHand[1] != 86) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[3]]->itemHand[1];
                if (memory > 0)
                    character[menuPosID[3]]->itemHand[1] = memory;
                else
                    character[menuPosID[3]]->itemHand[1] = 86;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[3]]->itemHand[1];
                    character[menuPosID[3]]->itemHand[1] = *this->mouseItem;
                    if (memory == 86)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        } else if (character[menuPosID[3]]->currenthp > 10) {
            if (!attackInProgress[menuPosID[3]][1] && item->itemTyp[character[menuPosID[3]]->itemHand[1]] == 1) {
                attackTimer[menuPosID[3]][1] = mediaObject->getMilliSeconds() + attackInterval;
                attackInProgress[menuPosID[3]][1] = true;
                if (character[menuPosID[3]]->charID <= 1)
                    character[menuPosID[3]]->attackNow(1);
            } else if (!attackInProgress[menuPosID[3]][1] && item->itemTyp[character[menuPosID[3]]->itemHand[1]] != 1)
                mapEvent->statusMessage(languageData->text[165]);
        }
    }

    //Character Swapping - Char 1
    bool swapNewClick = false;
    if (posX >= 185 && posX <= 247 && posY >= 2 && posY <= 9) {
        if (swapping) {
            short pos1 = menuPosID[swapFromPos];
            short pos2 = menuPosID[0];
            menuPosID[0] = pos1;
            menuPosID[swapFromPos] = pos2;
        }
        if (!swapping) {
            swapping = swapNewClick = true;
            swapFromPos = 0;
        }
    }

    //Character Swapping - Char 1
    if (posX >= 257 && posX <= 319 && posY >= 2 && posY <= 9) {
        if (swapping) {
            short pos1 = menuPosID[swapFromPos];
            short pos2 = menuPosID[1];
            menuPosID[1] = pos1;
            menuPosID[swapFromPos] = pos2;
        }
        if (!swapping) {
            swapping = swapNewClick = true;
            swapFromPos = 1;
        }
    }

    //Character Swapping - Char 2
    if (posX >= 185 && posX <= 247 && posY >= 54 && posY <= 61) {
        if (swapping) {
            short pos1 = menuPosID[swapFromPos];
            short pos2 = menuPosID[2];
            menuPosID[2] = pos1;
            menuPosID[swapFromPos] = pos2;
        }
        if (!swapping) {
            swapping = swapNewClick = true;
            swapFromPos = 2;
        }
    }

    //Character Swapping - Char 3
    if (posX >= 257 && posX <= 319 && posY >= 54 && posY <= 61) {
        if (swapping) {
            short pos1 = menuPosID[swapFromPos];
            short pos2 = menuPosID[3];
            menuPosID[3] = pos1;
            menuPosID[swapFromPos] = pos2;
        }
        if (!swapping) {
            swapping = swapNewClick = true;
            swapFromPos = 3;
        }
    }

    character[menuPosID[0]]->charID = 0;
    character[menuPosID[1]]->charID = 1;
    character[menuPosID[2]]->charID = 2;
    character[menuPosID[3]]->charID = 3;

    if (!swapNewClick) {
        swapping = false;
        swapFromPos = -1;
    }
}

//
// Inventar
//

void CCharMenu::handleInventarFull(short posX, short posY, short charID)
{

    short memory;

    short row, col;
    row = 0;
    col = 0;

    //14 Platz-Inventar
    for (short i = 0; i < 14; i++) {
        if (posX >= 185 + col * 18 && posX <= 202 + col * 18 && posY >= 39 + row * 18 && posY <= 56 + row * 18) {
            if (character[menuPosID[charID]]->itemInventar[i] != 52) {
                memory = *this->mouseItem;
                *this->mouseItem = character[menuPosID[charID]]->itemInventar[i];
                if (memory > 0)
                    character[menuPosID[charID]]->itemInventar[i] = memory;
                else
                    character[menuPosID[charID]]->itemInventar[i] = 52;
            } else {
                if (*this->mouseItem != 0) {
                    memory = character[menuPosID[charID]]->itemInventar[i];
                    character[menuPosID[charID]]->itemInventar[i] = *this->mouseItem;
                    if (memory == 52)
                        *this->mouseItem = 0;
                    else
                        *this->mouseItem = memory;
                }
            }
        }
        col++;
        if (col == 2) {
            row++;
            col = 0;
        }
    }

    //Kopf
    if (posX >= 302 && posX <= 319 && posY >= 54 && posY <= 71) {
        if (character[menuPosID[charID]]->itemHead != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemHead;
            if (memory > 0)
                character[menuPosID[charID]]->itemHead = memory;
            else
                character[menuPosID[charID]]->itemHead = 52;
        } else {
            if (*this->mouseItem != 0 && this->item->itemTyp[*this->mouseItem] == 4) {
                memory = character[menuPosID[charID]]->itemHead;
                character[menuPosID[charID]]->itemHead = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Hals
    if (posX >= 291 && posX <= 308 && posY >= 74 && posY <= 91) {
        if (character[menuPosID[charID]]->itemNeck != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemNeck;
            if (memory > 0)
                character[menuPosID[charID]]->itemNeck = memory;
            else
                character[menuPosID[charID]]->itemNeck = 52;
        } else {
            if (*this->mouseItem != 0 && this->item->itemTyp[*this->mouseItem] == 5) {
                memory = character[menuPosID[charID]]->itemNeck;
                character[menuPosID[charID]]->itemNeck = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Brust
    if (posX >= 228 && posX <= 245 && posY >= 75 && posY <= 92) {
        if (character[menuPosID[charID]]->itemTorso != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemTorso;
            if (memory > 0)
                character[menuPosID[charID]]->itemTorso = memory;
            else
                character[menuPosID[charID]]->itemTorso = 52;
        } else {
            if (*this->mouseItem != 0 && this->item->itemTyp[*this->mouseItem] == 3) {
                memory = character[menuPosID[charID]]->itemTorso;
                character[menuPosID[charID]]->itemTorso = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Handgelenk
    if (posX >= 229 && posX <= 246 && posY >= 95 && posY <= 112) {
        if (character[menuPosID[charID]]->itemWrist != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemWrist;
            if (memory > 0)
                character[menuPosID[charID]]->itemWrist = memory;
            else
                character[menuPosID[charID]]->itemWrist = 52;
        } else {
            if (*this->mouseItem != 0 && this->item->itemTyp[*this->mouseItem] == 6) {
                memory = character[menuPosID[charID]]->itemWrist;
                character[menuPosID[charID]]->itemWrist = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Fuss
    if (posX >= 281 && posX <= 298 && posY >= 136 && posY <= 153) {
        if (character[menuPosID[charID]]->itemFoot != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemFoot;
            if (memory > 0)
                character[menuPosID[charID]]->itemFoot = memory;
            else
                character[menuPosID[charID]]->itemFoot = 52;
        } else {
            if (*this->mouseItem != 0 && this->item->itemTyp[*this->mouseItem] == 8) {
                memory = character[menuPosID[charID]]->itemFoot;
                character[menuPosID[charID]]->itemFoot = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Gürtel 1
    if (posX >= 304 && posX <= 320 && posY >= 93 && posY <= 110) {
        if (character[menuPosID[charID]]->itemBelt[0] != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemBelt[0];
            if (memory > 0)
                character[menuPosID[charID]]->itemBelt[0] = memory;
            else
                character[menuPosID[charID]]->itemBelt[0] = 52;
        } else {
            if (*this->mouseItem != 0) {
                memory = character[menuPosID[charID]]->itemBelt[0];
                character[menuPosID[charID]]->itemBelt[0] = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Gürtel 2
    if (posX >= 304 && posX <= 320 && posY >= 111 && posY <= 128) {
        if (character[menuPosID[charID]]->itemBelt[1] != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemBelt[1];
            if (memory > 0)
                character[menuPosID[charID]]->itemBelt[1] = memory;
            else
                character[menuPosID[charID]]->itemBelt[1] = 52;
        } else {
            if (*this->mouseItem != 0) {
                memory = character[menuPosID[charID]]->itemBelt[1];
                character[menuPosID[charID]]->itemBelt[1] = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Gürtel 3
    if (posX >= 304 && posX <= 320 && posY >= 129 && posY <= 146) {
        if (character[menuPosID[charID]]->itemBelt[2] != 52) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemBelt[2];
            if (memory > 0)
                character[menuPosID[charID]]->itemBelt[2] = memory;
            else
                character[menuPosID[charID]]->itemBelt[2] = 52;
        } else {
            if (*this->mouseItem != 0) {
                memory = character[menuPosID[charID]]->itemBelt[2];
                character[menuPosID[charID]]->itemBelt[2] = *this->mouseItem;
                if (memory == 52)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Hand links
    if (posX >= 234 && posX <= 251 && posY >= 115 && posY <= 132) {
        if (character[menuPosID[charID]]->itemHand[0] != 85) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemHand[0];
            if (memory > 0)
                character[menuPosID[charID]]->itemHand[0] = memory;
            else
                character[menuPosID[charID]]->itemHand[0] = 85;
        } else {
            if (*this->mouseItem != 0) {
                memory = character[menuPosID[charID]]->itemHand[0];
                character[menuPosID[charID]]->itemHand[0] = *this->mouseItem;
                if (memory == 85)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }

    //Hand rechts
    if (posX >= 282 && posX <= 299 && posY >= 115 && posY <= 132) {
        if (character[menuPosID[charID]]->itemHand[1] != 86) {
            memory = *this->mouseItem;
            *this->mouseItem = character[menuPosID[charID]]->itemHand[1];
            if (memory > 0)
                character[menuPosID[charID]]->itemHand[1] = memory;
            else
                character[menuPosID[charID]]->itemHand[1] = 86;
        } else {
            if (*this->mouseItem != 0) {
                memory = character[menuPosID[charID]]->itemHand[1];
                character[menuPosID[charID]]->itemHand[1] = *this->mouseItem;
                if (memory == 86)
                    *this->mouseItem = 0;
                else
                    *this->mouseItem = memory;
            }
        }
    }
}

//
// neu zeichnen usw.
//

void CCharMenu::update()
{
    // Menustyles
    // 0 = 4 Fenster Char Ansicht
    // 1 = Inventar Ansicht
    // 2 = Stats Ansicht
    // 3 = Camp Menu
    // 4 = Save/Load/Quit Menu
    // 5 = Rest Menu

    if (menuStyle == 0 || menuStyle > 2) {
        //Hintergrund Inventar Bild
        drawPartyWindow();
        //Char Bild
        drawPortrait(menuPosID[0], 185, 11);
        //Char Name
        if (!swapping || (swapping && swapFromPos != 0))
            mediaObject->drawText(0, 186, 1, 0x00, 0x00, 0x00, character[menuPosID[0]]->name);
        if (swapping && swapFromPos == 0)
            mediaObject->drawText(0, 186, 1, 0xFF, 0xFF, 0xFF, languageData->text[166]);
        // String "HP"
        mediaObject->drawText(0, 186, 42, 0x00, 0x00, 0x00, languageData->text[0]);
        // Gesundheitsbalken
        drawSmallBar(198, 45, character[menuPosID[0]]->currenthp, character[menuPosID[0]]->hp);

        drawPortrait(menuPosID[1], 257, 11);
        if (!swapping || (swapping && swapFromPos != 1))
            mediaObject->drawText(0, 258, 1, 0x00, 0x00, 0x00, character[menuPosID[1]]->name);
        if (swapping && swapFromPos == 1)
            mediaObject->drawText(0, 258, 1, 0xFF, 0xFF, 0xFF, languageData->text[166]);
        mediaObject->drawText(0, 258, 42, 0x00, 0x00, 0x00, languageData->text[0]);
        drawSmallBar(270, 45, character[menuPosID[1]]->currenthp, character[menuPosID[1]]->hp);

        drawPortrait(menuPosID[2], 185, 63);
        if (!swapping || (swapping && swapFromPos != 2))
            mediaObject->drawText(0, 186, 53, 0x00, 0x00, 0x00, character[menuPosID[2]]->name);
        if (swapping && swapFromPos == 2)
            mediaObject->drawText(0, 186, 53, 0xFF, 0xFF, 0xFF, languageData->text[166]);
        mediaObject->drawText(0, 186, 94, 0x00, 0x00, 0x00, languageData->text[0]);
        drawSmallBar(198, 97, character[menuPosID[2]]->currenthp, character[menuPosID[2]]->hp);

        drawPortrait(menuPosID[3], 257, 63);
        if (!swapping || (swapping && swapFromPos != 3))
            mediaObject->drawText(0, 258, 53, 0x00, 0x00, 0x00, character[menuPosID[3]]->name);
        if (swapping && swapFromPos == 3)
            mediaObject->drawText(0, 258, 53, 0xFF, 0xFF, 0xFF, languageData->text[166]);
        mediaObject->drawText(0, 258, 94, 0x00, 0x00, 0x00, languageData->text[0]);
        drawSmallBar(270, 97, character[menuPosID[3]]->currenthp, character[menuPosID[3]]->hp);

        //Items in den Händen der Chars
        item->drawIcon(character[menuPosID[0]]->itemHand[0], 223, 11);
        item->drawIcon(character[menuPosID[0]]->itemHand[1], 223, 27);
        item->drawIcon(character[menuPosID[1]]->itemHand[0], 295, 11);
        item->drawIcon(character[menuPosID[1]]->itemHand[1], 295, 27);
        item->drawIcon(character[menuPosID[2]]->itemHand[0], 223, 63);
        item->drawIcon(character[menuPosID[2]]->itemHand[1], 223, 79);
        item->drawIcon(character[menuPosID[3]]->itemHand[0], 295, 63);
        item->drawIcon(character[menuPosID[3]]->itemHand[1], 295, 79);

        //Schleiher über Items legen wenn diese als Waffe gerade gebraucht wurden
        if (attackInProgress[menuPosID[0]][0] || character[menuPosID[0]]->currenthp <= 10)
            drawItemOverlay(217, 11);
        if (attackInProgress[menuPosID[0]][1] || character[menuPosID[0]]->currenthp <= 10)
            drawItemOverlay(217, 27);
        if (attackInProgress[menuPosID[1]][0] || character[menuPosID[1]]->currenthp <= 10)
            drawItemOverlay(289, 11);
        if (attackInProgress[menuPosID[1]][1] || character[menuPosID[1]]->currenthp <= 10)
            drawItemOverlay(289, 27);
        if (attackInProgress[menuPosID[2]][0] || character[menuPosID[2]]->currenthp <= 10)
            drawItemOverlayNotReachable(216, 63);
        if (attackInProgress[menuPosID[2]][1] || character[menuPosID[2]]->currenthp <= 10)
            drawItemOverlayNotReachable(216, 79);
        if (attackInProgress[menuPosID[3]][0] || character[menuPosID[3]]->currenthp <= 10)
            drawItemOverlayNotReachable(288, 63);
        if (attackInProgress[menuPosID[3]][1] || character[menuPosID[3]]->currenthp <= 10)
            drawItemOverlayNotReachable(288, 79);

//beim PocketPC Extra-Attack Button einblenden
#ifdef WINCE
        drawAttackButton(240, 11);
        drawAttackButton(240, 27);
        drawAttackButton(312, 11);
        drawAttackButton(312, 27);
        drawAttackButton(240, 63);
        drawAttackButton(240, 79);
        drawAttackButton(312, 63);
        drawAttackButton(312, 79);
#endif

        //wenn der Char gerade Schaden nimmt Schadensanzeige

        for (short i = 0; i < 4; i++) {
            if (character[menuPosID[i]]->damageTaken) {
                damageTakenTimer[menuPosID[i]] = mediaObject->getMilliSeconds() + 1000;
                sprintf(damageTakenValue[menuPosID[i]], "%i", character[menuPosID[i]]->damageTakenValue);
                character[menuPosID[i]]->damageTaken = false;
            }
            if (mediaObject->getMilliSeconds() < damageTakenTimer[menuPosID[i]]) {
                switch (i) {
                case 0:
                    mediaObject->drawImage(166, 197, 34, 129, 74, 40, 18);
                    mediaObject->drawText(0, 210, 41, 0xFF, 0xFF, 0xFF, damageTakenValue[menuPosID[i]]);
                    break;
                case 1:
                    mediaObject->drawImage(166, 269, 34, 129, 74, 40, 18);
                    mediaObject->drawText(0, 282, 41, 0xFF, 0xFF, 0xFF, damageTakenValue[menuPosID[i]]);
                    break;
                case 2:
                    mediaObject->drawImage(166, 197, 86, 129, 74, 40, 18);
                    mediaObject->drawText(0, 210, 93, 0xFF, 0xFF, 0xFF, damageTakenValue[menuPosID[i]]);
                    break;
                case 3:
                    mediaObject->drawImage(166, 269, 86, 129, 74, 40, 18);
                    mediaObject->drawText(0, 282, 93, 0xFF, 0xFF, 0xFF, damageTakenValue[menuPosID[i]]);
                    break;
                default:
                    break;
                }
            }
        }

        //wenn der Char gerade Schaden verteilt Schadensanzeige

        for (short i = 0; i < 4; i++) {
            short x_offset = 0;

            if (character[menuPosID[i]]->charID > 1)
                break;

            if (character[menuPosID[i]]->damageDone) {
                if (attackInProgress[menuPosID[i]][0]) {
                    damageDoneTimer[menuPosID[i]][0] = mediaObject->getMilliSeconds() + 1000;
                    if (character[menuPosID[i]]->damageDoneValue[0] == 0) {
                        sprintf(damageDoneValue[menuPosID[i]][0], "%s", languageData->text[179]);
                        x_offset = 8;
                    } else
                        sprintf(damageDoneValue[menuPosID[i]][0], "%i", character[menuPosID[i]]->damageDoneValue[0]);
                    character[menuPosID[i]]->damageDone[0] = false;
                }
                if (attackInProgress[menuPosID[i]][1]) {
                    damageDoneTimer[menuPosID[i]][1] = mediaObject->getMilliSeconds() + 1000;
                    if (character[menuPosID[i]]->damageDoneValue[1] == 0) {
                        sprintf(damageDoneValue[menuPosID[i]][1], "%s", languageData->text[179]);
                        x_offset = 8;
                    } else
                        sprintf(damageDoneValue[menuPosID[i]][1], "%i", character[menuPosID[i]]->damageDoneValue[1]);
                    character[menuPosID[i]]->damageDone[1] = false;
                }
            }
            if (mediaObject->getMilliSeconds() < damageDoneTimer[menuPosID[i]][0]) {
                switch (i) {
                case 0:
                    if (attackInProgress[menuPosID[i]][0]) {
                        mediaObject->drawImage(166, 216, 11, 128, 96, 32, 15);
                        mediaObject->drawText(0, 228 - x_offset, 14, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][0]);
                    }
                    break;
                case 1:
                    if (attackInProgress[menuPosID[i]][0]) {
                        mediaObject->drawImage(166, 287, 11, 128, 96, 32, 15);
                        mediaObject->drawText(0, 299 - x_offset, 14, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][0]);
                    }
                    break;
                case 2:
                    if (attackInProgress[menuPosID[i]][0]) {
                        mediaObject->drawImage(166, 216, 63, 128, 96, 32, 15);
                        mediaObject->drawText(0, 228 - x_offset, 66, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][0]);
                    }
                    break;
                case 3:
                    if (attackInProgress[menuPosID[i]][0]) {
                        mediaObject->drawImage(166, 287, 63, 128, 96, 32, 15);
                        mediaObject->drawText(0, 299 - x_offset, 66, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][0]);
                    }
                    break;
                default:
                    break;
                }
            }
            if (mediaObject->getMilliSeconds() < damageDoneTimer[menuPosID[i]][1]) {
                switch (i) {
                case 0:
                    if (attackInProgress[menuPosID[i]][1]) {
                        mediaObject->drawImage(166, 216, 27, 128, 96, 32, 15);
                        mediaObject->drawText(0, 228 - x_offset, 30, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][1]);
                    }
                    break;
                case 1:
                    if (attackInProgress[menuPosID[i]][1]) {
                        mediaObject->drawImage(166, 287, 27, 128, 96, 32, 15);
                        mediaObject->drawText(0, 299 - x_offset, 30, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][1]);
                    }
                    break;
                case 2:
                    if (attackInProgress[menuPosID[i]][1]) {
                        mediaObject->drawImage(166, 216, 79, 128, 96, 32, 15);
                        mediaObject->drawText(0, 228 - x_offset, 82, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][1]);
                    }
                    break;
                case 3:
                    if (attackInProgress[menuPosID[i]][1]) {
                        mediaObject->drawImage(166, 287, 79, 128, 96, 32, 15);
                        mediaObject->drawText(0, 299 - x_offset, 82, 0xFF, 0xFF, 0xFF, damageDoneValue[menuPosID[i]][1]);
                    }
                    break;
                default:
                    break;
                }
            }
        }
    } else if (menuStyle == 1) {
        drawInventar();
        //Char Portait
        drawPortrait(menuPosID[charID], 186, 3);
        //Char Name
        mediaObject->drawText(0, 223, 4, 0xFF, 0xFF, 0xFF, character[menuPosID[charID]]->name);
        //String "HP"
        mediaObject->drawText(0, 223, 14, 0x00, 0x00, 0x00, languageData->text[0]);
        // Gesundheitsbalken
        drawBigBar(249, 15, character[menuPosID[charID]]->currenthp, character[menuPosID[charID]]->hp);
        //String "FOOD"
        mediaObject->drawText(0, 223, 22, 0x00, 0x00, 0x00, languageData->text[1]);
        // Nahrungsbalken
        drawBigBar(249, 24, character[menuPosID[charID]]->currentfood, character[menuPosID[charID]]->food);
        //Pfeile im Inventar
        char arrows[3];
        character[menuPosID[charID]]->arrows = 0;
        for (short i = 0; i < 14; i++)
            if (character[menuPosID[charID]]->itemInventar[i] == 16)
                character[menuPosID[charID]]->arrows++;
        if (character[menuPosID[charID]]->itemHand[0] == 16)
            character[menuPosID[charID]]->arrows++;
        if (character[menuPosID[charID]]->itemHand[1] == 16)
            character[menuPosID[charID]]->arrows++;
        for (short i = 0; i < 3; i++)
            if (character[menuPosID[charID]]->itemBelt[i] == 16)
                character[menuPosID[charID]]->arrows++;
        sprintf(arrows, "%d", character[menuPosID[charID]]->arrows);
        mediaObject->drawText(0, 233, 62, 0xFF, 0xFF, 0xFF, arrows);
        //Items im Inventar zeichnen
        short row, col;
        row = 0;
        col = 0;
        for (short i = 0; i < 14; i++) {
            item->drawIcon(character[menuPosID[charID]]->itemInventar[i], 185 + col * 18, 40 + row * 18);
            col++;
            if (col == 2) {
                row++;
                col = 0;
            }
        }
        item->drawIcon(character[menuPosID[charID]]->itemHead, 302, 55);
        item->drawIcon(character[menuPosID[charID]]->itemNeck, 291, 75);
        item->drawIcon(character[menuPosID[charID]]->itemTorso, 228, 76);
        item->drawIcon(character[menuPosID[charID]]->itemBelt[0], 304, 94);
        item->drawIcon(character[menuPosID[charID]]->itemBelt[1], 304, 112);
        item->drawIcon(character[menuPosID[charID]]->itemBelt[2], 304, 130);
        item->drawIcon(character[menuPosID[charID]]->itemFoot, 281, 138);
        item->drawIcon(character[menuPosID[charID]]->itemWrist, 229, 96);
        item->drawIcon(character[menuPosID[charID]]->itemHand[0] == 85 ? 52 : character[menuPosID[charID]]->itemHand[0], 234, 116);
        item->drawIcon(character[menuPosID[charID]]->itemHand[1] == 86 ? 52 : character[menuPosID[charID]]->itemHand[1], 282, 116);

    } else if (menuStyle == 2) {
        char temp[8];
        drawStats();
        //Char Portait
        drawPortrait(menuPosID[charID], 186, 3);
        //Char Name
        mediaObject->drawText(0, 223, 4, 0xFF, 0xFF, 0xFF, character[menuPosID[charID]]->name);
        //String "HP"
        mediaObject->drawText(0, 223, 14, 0x00, 0x00, 0x00, languageData->text[0]);
        // Gesundheitsbalken
        drawBigBar(249, 15, character[menuPosID[charID]]->currenthp, character[menuPosID[charID]]->hp);
        //String "FOOD"
        mediaObject->drawText(0, 223, 22, 0x00, 0x00, 0x00, languageData->text[1]);
        // Nahrungsbalken
        drawBigBar(249, 24, character[menuPosID[charID]]->currentfood, character[menuPosID[charID]]->food);
        //String "CHARACTER INFO"
        mediaObject->drawText(0, 187, 39, 0xFF, 0xFF, 0xFF, languageData->text[2]);
        //Char Klasse
        mediaObject->drawText(0, 187, 51, 0x00, 0x00, 0x00, character[menuPosID[charID]]->charclass);
        //Char typ
        mediaObject->drawText(0, 187, 59, 0x00, 0x00, 0x00, character[menuPosID[charID]]->social);
        //Char Rasse
        mediaObject->drawText(0, 187, 66, 0x00, 0x00, 0x00, character[menuPosID[charID]]->race);
        //Char Geschlecht
        mediaObject->drawText(0, 225, 66, 0x00, 0x00, 0x00, character[menuPosID[charID]]->gender);
        //String "STRENGTH"
        mediaObject->drawText(0, 187, 78, 0x00, 0x00, 0x00, languageData->text[3]);
        //Char Stärke
        sprintf(temp, "%d/%d", character[menuPosID[charID]]->strength, character[menuPosID[charID]]->maxstrength);
        mediaObject->drawText(0, 279, 78, 0xFF, 0xFF, 0xFF, temp);
        //String "INT"
        mediaObject->drawText(0, 187, 86, 0x00, 0x00, 0x00, languageData->text[4]);
        //Char Int
        sprintf(temp, "%d", character[menuPosID[charID]]->intelligence);
        mediaObject->drawText(0, 279, 86, 0xFF, 0xFF, 0xFF, temp);
        //String "WISDOM"
        mediaObject->drawText(0, 187, 94, 0x00, 0x00, 0x00, languageData->text[5]);
        //Char Wisdom
        sprintf(temp, "%d", character[menuPosID[charID]]->wisdom);
        mediaObject->drawText(0, 279, 94, 0xFF, 0xFF, 0xFF, temp);
        //String "DEX"
        mediaObject->drawText(0, 187, 102, 0x00, 0x00, 0x00, languageData->text[6]);
        //Char Dex
        sprintf(temp, "%d", character[menuPosID[charID]]->dext);
        mediaObject->drawText(0, 279, 102, 0xFF, 0xFF, 0xFF, temp);
        //String "CONST"
        mediaObject->drawText(0, 187, 110, 0x00, 0x00, 0x00, languageData->text[7]);
        //Char Const
        sprintf(temp, "%d", character[menuPosID[charID]]->constitution);
        mediaObject->drawText(0, 279, 110, 0xFF, 0xFF, 0xFF, temp);
        //String "ChARISMA"
        mediaObject->drawText(0, 187, 118, 0x00, 0x00, 0x00, languageData->text[8]);
        //Char Charisma
        sprintf(temp, "%d", character[menuPosID[charID]]->charisma);
        mediaObject->drawText(0, 279, 118, 0xFF, 0xFF, 0xFF, temp);
        //String "ARMOR CLASS"
        mediaObject->drawText(0, 187, 126, 0x00, 0x00, 0x00, languageData->text[9]);
        //Char armor class
        sprintf(temp, "%d", character[menuPosID[charID]]->armorclass);
        mediaObject->drawText(0, 279, 126, 0xFF, 0xFF, 0xFF, temp);
        //String "EXP"
        mediaObject->drawText(0, 243, 138, 0x00, 0x00, 0x00, languageData->text[10]);
        //String "LVL"
        mediaObject->drawText(0, 279, 138, 0x00, 0x00, 0x00, languageData->text[11]);
        //Char Klasse
        mediaObject->drawText(0, 187, 146, 0x00, 0x00, 0x00, character[menuPosID[charID]]->charclass);
        //Char Exp
        sprintf(temp, "%d", character[menuPosID[charID]]->exp);
        mediaObject->drawText(0, 237, 146, 0xFF, 0xFF, 0xFF, temp);
        //Char LVL
        sprintf(temp, "%d", character[menuPosID[charID]]->lvl);
        mediaObject->drawText(0, 279, 146, 0xFF, 0xFF, 0xFF, temp);
    }
    if (menuStyle == 3) {
        drawCampMenu();
    }
    if (menuStyle == 4) {
        drawSaveMenu();
    }
    if (menuStyle == 5) {
        drawRestMenu();
    }

    //Timer um Endlosangriffe zu verhindern
    long time = mediaObject->getMilliSeconds();
    for (short a = 0; a < 6; a++) {
        for (short h = 0; h < 2; h++) {
            if (attackInProgress[a][h] && time > attackTimer[a][h]) {
                attackInProgress[a][h] = false;
            }
        }
    }
}

//
// Camp Menu
//

void CCharMenu::drawCampMenu()
{
    //Hintergrund zeichnen
    mediaObject->fillRect(0, 0, 175, 144, 108, 108, 136);
    mediaObject->fillRect(0, 0, 175, 2, 148, 148, 172);
    mediaObject->fillRect(174, 0, 2, 144, 148, 148, 172);
    mediaObject->fillRect(0, 142, 175, 2, 52, 52, 80);
    mediaObject->fillRect(0, 0, 2, 144, 52, 52, 80);

    //CAMP
    mediaObject->drawText(6, 4, 4, 0, 0, 0, this->languageData->text[183]);

    //REST
    drawCampButton(12, 20, 158, this->languageData->text[184]);

    //OPTIONS
    drawCampButton(12, 37, 158, this->languageData->text[185]);

    //EXIT
    drawCampButton(128, 122, 40, this->languageData->text[189]);
}
void CCharMenu::handleCampMenu(short posX, short posY)
{
    //Exit Button
    if (posX > 128 && posY > 122 && posX < 168 && posY < 136)
        menuStyle = 0;

    //REST Button
    if (posX > 12 && posY > 20 && posX < 170 && posY < 38) {
        //prüfen ob sich Monster in der Nähe befinden (7x6 Felder Umgebung), wenn nein kann gerastet werden
        bool monsterFound = false;
        short player_line = gameMap->playerPos / 33;
        short line_count = 0, field_count = 0;
        //printf("Playerpos: %d\n", gameMap->playerPos);
        for (short map_line = ((gameMap->playerPos / 33) - 3); map_line <= ((gameMap->playerPos / 33) + 3); map_line++) {
            if (map_line < 0)
                map_line = 0;
            if (map_line > 32)
                map_line = 32;
            field_count = 0;
            for (short field_nr = (gameMap->playerPos - ((player_line - map_line) * 33) - 3); field_nr <= (gameMap->playerPos - ((player_line - map_line) * 33) + 4); field_nr++) {
                if ((field_nr / 33) != map_line)
                    field_nr--;

                //printf("%d | ", field_nr);

                //prüfen obs in diesem Feld ein Monster gibt
                for (short m = 0; m < gameMap->monsterCount; m++) {
                    if (this->gameMap->monsterInfo[m][0] == field_nr)
                        monsterFound = true;
                }

                field_count++;
                if (field_count == 7)
                    break;
            }
            //printf("\n");
            line_count++;
            if (line_count == 6)
                break;
        }

        if (!monsterFound) {
            timer = this->mediaObject->getMilliSeconds() + 3000;
            menuStyle = 5;
        } else {
            mapEvent->statusMessage(this->languageData->text[194]);
            menuStyle = 0;
        }
    }

    //OPTIONS Button
    if (posX > 12 && posY > 37 && posX < 170 && posY < 55)
        menuStyle = 4;
}

//
// Save/Load/Quit Menu
//

void CCharMenu::drawSaveMenu()
{
    //Hintergrund zeichnen
    mediaObject->fillRect(0, 0, 175, 144, 108, 108, 136);
    mediaObject->fillRect(0, 0, 175, 2, 148, 148, 172);
    mediaObject->fillRect(174, 0, 2, 144, 148, 148, 172);
    mediaObject->fillRect(0, 142, 175, 2, 52, 52, 80);
    mediaObject->fillRect(0, 0, 2, 144, 52, 52, 80);

    //Game Options
    mediaObject->drawText(6, 4, 4, 0, 0, 0, this->languageData->text[185]);

    //LOAD
    drawCampButton(12, 20, 158, this->languageData->text[186]);

    //SAVE
    drawCampButton(12, 37, 158, this->languageData->text[187]);

    //QUIT
    drawCampButton(12, 54, 158, this->languageData->text[188]);

    //EXIT
    drawCampButton(128, 122, 40, this->languageData->text[189]);
}
void CCharMenu::handleSaveMenu(short posX, short posY)
{
    //LOAD Button
    if (posX > 12 && posY > 20 && posX < 168 && posY < 34) {
        loadGame = true;
        menuStyle = 0;
    }

    //SAVE Button
    if (posX > 12 && posY > 37 && posX < 168 && posY < 51) {
        saveGame = true;
        menuStyle = 0;
    }

    //QUIT Button
    if (posX > 12 && posY > 54 && posX < 168 && posY < 68)
        mediaObject->quit();

    //Exit Button
    if (posX > 128 && posY > 122 && posX < 168 && posY < 136)
        menuStyle = 3;
}

//
// Rest Menu
//

void CCharMenu::drawRestMenu()
{
    //Hintergrund zeichnen
    mediaObject->fillRect(0, 0, 175, 144, 108, 108, 136);
    mediaObject->fillRect(0, 0, 175, 2, 148, 148, 172);
    mediaObject->fillRect(174, 0, 2, 144, 148, 148, 172);
    mediaObject->fillRect(0, 142, 175, 2, 52, 52, 80);
    mediaObject->fillRect(0, 0, 2, 144, 52, 52, 80);

    //Resting Party Text
    mediaObject->drawText(6, 4, 4, 0, 0, 0, this->languageData->text[190]);

    mediaObject->fillRect(8, 20, 160, 56, 108, 108, 136);
    mediaObject->fillRect(8, 20, 160, 2, 148, 148, 172);
    mediaObject->fillRect(8 + 159, 20, 2, 56, 148, 148, 172);
    mediaObject->fillRect(8, 20 + 54, 159, 2, 52, 52, 80);
    mediaObject->fillRect(8, 20, 2, 56, 52, 52, 80);
    mediaObject->drawText(4, 20, 27, 0, 0, 0, this->languageData->text[191]);
    mediaObject->drawText(4, 20, 37, 0, 0, 0, this->languageData->text[192]);
    mediaObject->drawText(4, 20, 47, 0, 0, 0, this->languageData->text[193]);

    //Chars erholen
    for (short i = 0; i < 4; i++)
        if (character[i] != NULL)
            character[i]->currenthp = character[i]->hp;

    //Ausblenden nach 2 sek.
    if (mediaObject->getMilliSeconds() > timer)
        menuStyle = 0;
}
void CCharMenu::handleRestMenu(short posX, short posY)
{
}

//
// Button für Campstyle-Menus
//

void CCharMenu::drawCampButton(short posX, short posY, short width, char text[64])
{
    //Hintergrund zeichnen
    mediaObject->fillRect(posX, posY, width, 14, 108, 108, 136);
    mediaObject->fillRect(posX, posY, width, 2, 148, 148, 172);
    mediaObject->fillRect(posX + width - 1, posY, 2, 14, 148, 148, 172);
    mediaObject->fillRect(posX, posY + 12, width, 2, 52, 52, 80);
    mediaObject->fillRect(posX, posY, 2, 14, 52, 52, 80);

    //Text zeichnen inkl. MouseOver Effekt
    if (mouseX > posX && mouseY > posY && mouseX < posX + width && mouseY < posY + 14)
        mediaObject->drawText(5, posX + 4, posY + 1, 255, 255, 255, text);
    else
        mediaObject->drawText(4, posX + 4, posY + 1, 255, 255, 255, text);
}
