#include "monster.h"

void CMonster::init(CMap* gameMap, MEDIAWrapper* mediaObject, CLanguage* languageData, CEvent* mapEvent, short monsterType, short monsterPos, short mapId, short monsterFace, short friendly)
{
    this->mediaObject = mediaObject;
    this->gameMap = gameMap;
    this->mapEvent = mapEvent;
    this->languageData = languageData;
    currenttime = time_change = 0;
    this->monsterType = monsterType;
    this->monsterPos = monsterPos;
    this->staticMapId = mapId;
    this->friendly = true;
    this->isMoving = false;
    moveToPos = -1;
    nrInPlace = 1;
    short countMonsterInPlace = 0;
    for (int m = 0; m < mapId; m++)
        if (gameMap->monsterInfo[m][0] == monsterPos)
            countMonsterInPlace++;
    if (countMonsterInPlace >= 1)
        nrInPlace = 2;
    face = drawFace = monsterFace; //0=N, 1=S, 2=E, 3=W
    aniStep = false;
    attack = false;
    attack_time_change = attackFrame = 0;

    switch (this->monsterType) {
    //Wolf
    case 1:
        hitPoints = 20;
        exp_gain = 16;
        this->friendly = friendly;
        if (!friendly)
            isMoving = true;
        else
            isMoving = false;
        currentPathFindingType = 1;
        if (mediaObject->imageLoaded(this->monsterType))
            break;
        mediaObject->createImage(this->monsterType, 0, 1408, 200);
        mediaObject->fillRect(0, 0, 1408, 200, 255, 255, 255, this->monsterType);
        mediaObject->loadCPS(250, "original/WOLF.CPS", "original/FOREST.PAL", 0, 0, 320, 200, true, true);
        mediaObject->copyImage(250, this->monsterType, 0, 0, 58, 4, 57, 100, 0, 0, 0); //vorn
        mediaObject->copyImage(250, this->monsterType, 255, 0, 244, 4, 65, 109, 0, 0, 0); //hinten
        mediaObject->copyImage(250, this->monsterType, 52, 0, 388, 4, 101, 99, 0, 0, 0); //links 1
        mediaObject->copyImage(250, this->monsterType, 153, 0, 566, 4, 98, 100, 0, 0, 0); //links 2
        mediaObject->copyImage(250, this->monsterType, 0, 100, 764, 10, 55, 100, 0, 0, 0); //Angriff 1
        mediaObject->copyImage(250, this->monsterType, 56, 100, 952, 10, 50, 100, 0, 0, 0); //Angriff 2
        mediaObject->copyScaledImage(250, this->monsterType, 52, 0, 1094, 4, 101, 99, 101, 99, 0, 0, 0, true); //rechts 1
        mediaObject->copyScaledImage(250, this->monsterType, 153, 0, 1282, 4, 98, 100, 98, 100, 0, 0, 0, true); //rechts 2
        break;
    //Kleriker - rot
    case 2:
        hitPoints = 20;
        exp_gain = 16;
        this->friendly = friendly;
        if (!friendly)
            isMoving = true;
        else
            isMoving = false;
        currentPathFindingType = 1;
        if (mediaObject->imageLoaded(this->monsterType))
            break;
        mediaObject->createImage(this->monsterType, 0, 1408, 200);
        mediaObject->fillRect(0, 0, 1408, 200, 255, 255, 255, this->monsterType);
        mediaObject->loadCPS(250, "original/CLERIC1.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true, true);
        mediaObject->copyImage(250, this->monsterType, 0, 0, 58, 4, 58, 95, 0, 0, 0); //vorn
        mediaObject->copyImage(250, this->monsterType, 166, 0, 244, 4, 58, 95, 0, 0, 0); //hinten
        mediaObject->copyImage(250, this->monsterType, 53, 0, 388, 4, 58, 95, 0, 0, 0); //links 1
        mediaObject->copyImage(250, this->monsterType, 107, 0, 566, 4, 58, 95, 0, 0, 0); //links 2
        mediaObject->copyImage(250, this->monsterType, 222, 0, 764, 10, 58, 95, 0, 0, 0); //Angriff 1
        mediaObject->copyImage(250, this->monsterType, 0, 105, 942, 10, 56, 95, 0, 0, 0); //Angriff 2
        mediaObject->copyScaledImage(250, this->monsterType, 53, 0, 1094, 4, 58, 95, 58, 95, 0, 0, 0, true); //rechts 1
        mediaObject->copyScaledImage(250, this->monsterType, 107, 0, 1282, 4, 58, 95, 58, 95, 0, 0, 0, true); //rechts 2
        break;
    default:
        break;
    }
}

//
//updaten
//

bool CMonster::update(long timer, short pos, short destination)
{
    /*
		destination: 0 bis 11
		-1: aktueller Standpunkt 
		0: ganz vorn links
		1: ganz vorn rechts
		2: 1 feld entfernt links
		3: 1 feld entfernt mitte 
		4: 1 feld entfernt rechts
		5: 2 felder entfernt links
		6: 2 felder entfernt mitte 
		7: 2 felder entfernt rechts
		8: ungenutzt
		9: 3 felder entfernt links
		10: 3 felder entfernt mitte
		11: 3 felder entfernt rechts
		12: ungenutzt
	*/

    this->currenttime = timer;
    srand(currenttime);
    if (this->currenttime > time_change) {
        aniStep = !aniStep;
        time_change += rand() % 3000 + 2000;
        if (isMoving)
            updatePos();
    }

    checkPlayerCollision(face, true);
    if (attack && !friendly) {
        doAttack();
    }

    if (hitPoints <= 0)
        this->monsterPos = 1;

    if (this->monsterPos != pos) {
        return false;
    }

    /*
	Player-View N
	N 1
	S 0
	E 6 oder 7
	W 2 oder 3

	Player-View S
	N 0
	S 1
	E 2 oder 3 (links)
	W 6 oder 7 (rechts)

	Player-View E
	N 2 oder 3
	S 6 oder 7
	E 1
	W 0

	Player-View W
	N 6 oder 7
	S 2 oder 3
	E 0
	W 1
	*/
    static short drawViewN[] = { 1, 0, 6, 2 };
    static short drawViewS[] = { 0, 1, 2, 6 };
    static short drawViewE[] = { 2, 6, 1, 0 };
    static short drawViewW[] = { 6, 2, 0, 1 };

    //0=N, 1=S, 2=E, 3=W
    switch (gameMap->playerFace) {
    case 0:
        drawFace = drawViewN[face];
        break;
    case 1:
        drawFace = drawViewS[face];
        break;
    case 2:
        drawFace = drawViewE[face];
        break;
    case 3:
        drawFace = drawViewW[face];
        break;
    default:
        break;
    }
    drawMirror = aniStep;
    if (drawFace == 6 && aniStep) {
        drawMirror = false;
        drawFace = 7;
    }
    if (drawFace == 2 && aniStep) {
        drawMirror = false;
        drawFace = 3;
    }

    mediaObject->fillRect(0, 0, 176, 200, 255, 255, 255, 250);
    if (destination == 3) {
        if (attack) {
            if (nrInPlace <= 1)
                mediaObject->copyScaledImage(this->monsterType, 250, 176 * attackFrame + 20, 0, 0, 0, 176, 200, 176, 200);
            else if (nrInPlace == 2)
                mediaObject->copyScaledImage(this->monsterType, 250, 176 * attackFrame - 20, 0, 0, 0, 176, 200, 176, 200);

        } else {
            mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 0, 0, 176, 200, 176, 200);
        }
    }
    if (destination == 5) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, -36, 10, 176, 200, 117, 133, 0, 0, 0, drawMirror);
    }
    if (destination == 6) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 36, 10, 176, 200, 117, 133, 0, 0, 0, drawMirror);
    }
    if (destination == 7) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 97, 10, 176, 200, 117, 133, 0, 0, 0, drawMirror);
    }
    if (destination == 9) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 01, 30, 176, 200, 65, 57, 0, 0, 0, drawMirror);
    }
    if (destination == 10) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 61, 30, 176, 200, 65, 57, 0, 0, 0, drawMirror);
    }
    if (destination == 11) {
        mediaObject->copyScaledImage(this->monsterType, 250, 176 * drawFace, 0, 120, 30, 176, 200, 65, 57, 0, 0, 0, drawMirror);
    }
    mediaObject->drawImage(250, 0, 0, 0, 0, 176, 200);

    return true;
}

//
// Hilfsfunktionen - prüfen auf Hinderniss
//

bool CMonster::checkCollision(short direction)
{
    //0=N, 1=S, 2=E, 3=W
    switch (direction) {
    case 0:
        if ((monsterPos - 32) < 0)
            return true;
        if (gameMap->isSolidWall(0, monsterPos - 32))
            return true;
        break;
    case 1:
        if (gameMap->isSolidWall(0, monsterPos + 32))
            return true;
        break;
    case 2:
        if (gameMap->isSolidWall(0, monsterPos + 1))
            return true;
        break;
    case 3:
        if ((monsterPos - 1) < 0)
            return true;
        if (gameMap->isSolidWall(0, monsterPos - 1))
            return true;
        break;
    default:
        break;
    }
    return false;
}

bool CMonster::checkPlayerCollision(short direction, bool doAttack = false)
{
    //0=N, 1=S, 2=E, 3=W
    switch (direction) {
    case 0:
        if (gameMap->playerPos == (monsterPos - 32)) {
            if (doAttack) {
                attack = true;
            }
            return true;
        }
        break;
    case 1:
        if (gameMap->playerPos == (monsterPos + 32)) {
            if (doAttack) {
                attack = true;
            }
            return true;
        }
        break;
    case 2:
        if (gameMap->playerPos == (monsterPos + 1)) {
            if (doAttack) {
                attack = true;
            }
            return true;
        }
        break;
    case 3:
        if (gameMap->playerPos == (monsterPos - 1)) {
            if (doAttack) {
                attack = true;
            }
            return true;
        }
        break;
    default:
        break;
    }
    attack = false;
    return false;
}

//
// prüfen ob sich Spieler in direktem Sichtfeld befindet
//

bool CMonster::checkPlayerInLine(short direction, bool setMoveToPos)
{
    //0=N, 1=S, 2=E, 3=W
    switch (direction) {
    case 0:
        for (short l = 1; l < 32; l++) {
            if (monsterPos - (32 * l) < 0)
                return false;
            if (gameMap->isSolidWall(0, monsterPos - 32))
                return false;
            if (monsterPos - (32 * l) == gameMap->playerPos) {
                if (setMoveToPos) {
                    moveToPos = gameMap->playerPos;
                    face = 0;
                }
                return true;
            }
        }
        break;
    case 1:

        for (short l = 1; l < 32; l++) {
            if (gameMap->isSolidWall(0, monsterPos + 32))
                return false;
            if (monsterPos + (32 * l) == gameMap->playerPos) {
                if (setMoveToPos) {
                    moveToPos = gameMap->playerPos;
                    face = 1;
                }
                return true;
            }
        }
        break;
    case 2:
        for (short l = 1; l < 32; l++) {
            if (gameMap->isSolidWall(0, monsterPos + 1))
                return false;
            if (monsterPos + (1 * l) == gameMap->playerPos) {
                if (setMoveToPos) {
                    moveToPos = gameMap->playerPos;
                    face = 2;
                }
                return true;
            }
        }
        break;
    case 3:
        for (short l = 1; l < 32; l++) {
            if (monsterPos - (1 * l) < 0)
                return false;
            if (gameMap->isSolidWall(0, monsterPos - 1))
                return false;
            if (monsterPos - (1 * l) == gameMap->playerPos) {
                if (setMoveToPos) {
                    moveToPos = gameMap->playerPos;
                    face = 3;
                }
                return true;
            }
        }
        break;
    default:
        break;
    }
    return false;
}

//
// Angriff ausführen
//
void CMonster::doAttack()
{
    short damage = 0;

    if (currenttime > attack_time_change && attackFrame == 0) {
        attackFrame = 4;
        srand(currenttime);
        attack_time_change = currenttime + 500;
    }

    else if (currenttime > attack_time_change && attackFrame == 4) {
        mediaObject->playSound(2);
        attackFrame = 5;
        srand(currenttime);
        attack_time_change = currenttime + 500;
    }

    else if (currenttime > attack_time_change && attackFrame == 5) {

        attackFrame = 0;

        //
        // Angriff ausführen
        //

        srand(currenttime);
        switch (monsterType) {
        case 1:
            //Wolf, Schaden zwischen 4 und 7
            damage = rand() % 4 + 3;
            break;
        case 2:
            //Kleriker - rot, Schaden zwischen 5 und 9
            damage = rand() % 5 + 4;
            break;
        default:
            break;
        }

        //abhängig von Blickwinkel des Monsters und der Spielerposition + Spieler-Blickwinkel anzugreifende character bestimmen
        short charToAttack1 = 0;
        short charToAttack2 = 1;

        // 0=N /\, 1=S \/, 2=E >, 3=W <
        switch (this->face) {
        case 0:
            switch (gameMap->playerFace) {
            case 0:
                charToAttack1 = 2;
                charToAttack2 = 3;
                break;
            case 1:
                charToAttack1 = 0;
                charToAttack2 = 1;
                break;
            case 2:
                charToAttack1 = 1;
                charToAttack2 = 3;
                break;
            case 3:
                charToAttack1 = 0;
                charToAttack2 = 2;
                break;
            default:
                break;
            }
            break;
        case 1:
            switch (gameMap->playerFace) {
            case 0:
                charToAttack1 = 0;
                charToAttack2 = 1;
                break;
            case 1:
                charToAttack1 = 2;
                charToAttack2 = 3;
                break;
            case 2:
                charToAttack1 = 0;
                charToAttack2 = 2;
                break;
            case 3:
                charToAttack1 = 1;
                charToAttack2 = 3;
                break;
            default:
                break;
            }
            break;
        case 2:
            switch (gameMap->playerFace) {
            case 0:
                charToAttack1 = 0;
                charToAttack2 = 2;
                break;
            case 1:
                charToAttack1 = 1;
                charToAttack2 = 3;
                break;
            case 2:
                charToAttack1 = 2;
                charToAttack2 = 3;
                break;
            case 3:
                charToAttack1 = 0;
                charToAttack2 = 1;
                break;
            default:
                break;
            }
            break;
        case 3:
            switch (gameMap->playerFace) {
            case 0:
                charToAttack1 = 1;
                charToAttack2 = 3;
                break;
            case 1:
                charToAttack1 = 0;
                charToAttack2 = 2;
                break;
            case 2:
                charToAttack1 = 0;
                charToAttack2 = 1;
                break;
            case 3:
                charToAttack1 = 2;
                charToAttack2 = 3;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }

        //einen von zwei Characteren auf Schaden reagieren lassen
        short randnr = rand() % 2 + 1;
        if (randnr == 1) {
            for (short c = 0; c < 4; c++) {
                if (character[c]->charID == charToAttack1) {
                    character[c]->doDamage(damage);
                }
            }
        }
        if (randnr == 2) {
            for (short c = 0; c < 4; c++) {
                if (character[c]->charID == charToAttack2) {
                    character[c]->doDamage(damage);
                }
            }
        }

        srand(currenttime);
        attack_time_change = currenttime + rand() % 2000 + 2000;
    }
}

//
// Schaden hinzufügen (diesem Monster)
//

void CMonster::doDamage(short damage)
{
    friendly = false;
    isMoving = true;

    if (damage < 0)
        damage = 0;
    if (hitPoints > 0)
        hitPoints -= damage;
    if (hitPoints <= 0) {
        hitPoints = 0;

        //Chars Erfahrung hinzugeben
        for (short i = 0; i < 4; i++)
            if (character[i] != NULL)
                character[i]->exp += this->exp_gain;
    }
}

//
// Position und Movement updaten
//

void CMonster::updatePos()
{
    switch (this->monsterType) {
    //Wolf
    case 1:
        //Angriff bei Spielerkontakt, ansonsten normales umherwandern
        if (!checkPlayerCollision(face, true)) {
            attack = false;
            doPathFinding(currentPathFindingType);
        }
        break;
    //Kleriker - rot
    case 2:
        //Angriff bei Spielerkontakt, ansonsten normales umherwandern
        if (!checkPlayerCollision(face, true)) {
            attack = false;
            doPathFinding(currentPathFindingType);
        }
        break;
    default:
        break;
    }
}

//
// die verschiedenen Arten der Pfadfindung
//

void CMonster::doPathFinding(short pathType)
{
    switch (pathType) {
        //
        // Typ 1 -	einfaches Laufen und Neuausrichtung bei Kollision - kein Focus auf bestimmten Punkt,
        //			bei Sichtkontakt Verfolgung des Spielers durch Wechsel zu pathtype 2
        //
        //0=N, 1=S, 2=E, 3=W

    case 1:

        //prüfen ob in direkter Linie - ohne Wand dazwischen - der Spieler erreichbar ist
        for (short dir = 0; dir < 4; dir++)
            if (checkPlayerInLine(dir, true)) {
                //wenn ja wird ab jetzt gnadenlos verfolgt
                pathType = 2;
            }

        //einfaches laufen
        switch (face) {
        case 0:
            if (checkCollision(face)) {
                if (!checkCollision(2)) {
                    if (checkPlayerCollision(2, true)) {
                        face = 2;
                    } else {
                        if (countMonster(monsterPos + 1) == 0) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 1) == 1) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 2;
                        }
                        face = 2;
                    }
                } else if (!checkCollision(3)) {
                    if (checkPlayerCollision(3, true)) {
                        face = 3;
                    } else {
                        if (countMonster(monsterPos - 1) == 0) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 1) == 1) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 2;
                        }
                        face = 3;
                    }
                } else if (!checkCollision(1)) {
                    if (checkPlayerCollision(1, true)) {
                        face = 1;
                    } else {
                        if (countMonster(monsterPos + 32) == 0) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 32) == 1) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 2;
                        }
                        face = 1;
                    }
                }
            } else {
                if (checkPlayerCollision(0, true)) {
                    face = 0;
                } else {
                    if (countMonster(monsterPos - 32) == 0) {
                        monsterPos -= 32;
                        nrInPlace = 1;
                    } else if (countMonster(monsterPos - 32) == 1) {
                        monsterPos -= 32;
                        nrInPlace = 2;
                    }
                }
            }
            break;
        case 1:
            if (checkCollision(face)) {
                if (!checkCollision(2)) {
                    if (checkPlayerCollision(2, true)) {
                        face = 2;
                    } else {
                        if (countMonster(monsterPos + 1) == 0) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 1) == 1) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 2;
                        }
                        face = 2;
                    }
                } else if (!checkCollision(3)) {
                    if (checkPlayerCollision(3, true)) {
                        face = 3;
                    } else {
                        if (countMonster(monsterPos - 1) == 0) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 1) == 1) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 2;
                        }
                        face = 3;
                    }
                } else if (!checkCollision(0)) {
                    if (checkPlayerCollision(0, true)) {
                        face = 0;
                    } else {
                        if (countMonster(monsterPos - 32) == 0) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 32) == 1) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 2;
                        }
                        face = 0;
                    }
                }
            } else {
                if (checkPlayerCollision(1, true)) {
                    face = 1;
                } else {
                    if (countMonster(monsterPos - 32) == 0) {
                        monsterPos += 32;
                        nrInPlace = 1;
                    } else if (countMonster(monsterPos + 32) == 1) {
                        monsterPos += 3;
                        nrInPlace = 2;
                    }
                }
            }
            break;
        case 2:
            if (checkCollision(face)) {
                if (!checkCollision(0)) {
                    if (checkPlayerCollision(0, true)) {
                        face = 0;
                    } else {
                        if (countMonster(monsterPos - 32) == 0) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 32) == 1) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 2;
                        }
                        face = 0;
                    }
                } else if (!checkCollision(1)) {
                    if (checkPlayerCollision(1, true)) {
                        face = 1;
                    } else {
                        if (countMonster(monsterPos + 32) == 0) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 32) == 1) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 2;
                        }
                        face = 1;
                    }
                } else if (!checkCollision(3)) {
                    if (checkPlayerCollision(3, true)) {
                        face = 3;
                    } else {
                        if (countMonster(monsterPos - 1) == 0) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 1) == 1) {
                            monsterPos -= 1;
                            face = 3;
                            nrInPlace = 2;
                        }
                        face = 3;
                    }
                }
            } else {
                if (checkPlayerCollision(2, true)) {
                    face = 2;
                } else {
                    if (countMonster(monsterPos + 1) == 0) {
                        monsterPos += 1;
                        nrInPlace = 1;
                    } else if (countMonster(monsterPos + 1) == 1) {
                        monsterPos += 1;
                        nrInPlace = 2;
                    }
                }
            }
            break;
        case 3:
            if (checkCollision(face)) {
                if (!checkCollision(0)) {
                    if (checkPlayerCollision(0, true)) {
                        face = 0;
                    } else {
                        if (countMonster(monsterPos - 32) == 0) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos - 32) == 1) {
                            monsterPos -= 32;
                            face = 0;
                            nrInPlace = 2;
                        }
                        face = 0;
                    }
                } else if (!checkCollision(1)) {
                    if (checkPlayerCollision(1, true)) {
                        face = 1;
                    } else {
                        if (countMonster(monsterPos + 32) == 0) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 32) == 1) {
                            monsterPos += 32;
                            face = 1;
                            nrInPlace = 2;
                        }
                        face = 1;
                    }
                } else if (!checkCollision(2)) {
                    if (checkPlayerCollision(2, true)) {
                        face = 2;
                    } else {
                        if (countMonster(monsterPos + 1) == 0) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 1;
                        } else if (countMonster(monsterPos + 1) == 1) {
                            monsterPos += 1;
                            face = 2;
                            nrInPlace = 2;
                        }
                        face = 2;
                    }
                }
            } else {
                if (checkPlayerCollision(3, true)) {
                    face = 3;
                } else {
                    if (countMonster(monsterPos - 1) == 0) {
                        monsterPos -= 1;
                        nrInPlace = 1;
                    } else if (countMonster(monsterPos - 1) == 1) {
                        monsterPos -= 1;
                        nrInPlace = 2;
                    }
                }
            }
            break;
        default:
            break;
        }
        break;
    case 2:
        //
        // Typ 2 -	Spieler wird verfolgt
        //
        //0=N, 1=S, 2=E, 3=W

        //exisiert ein gesetzter Zielpunkt dann dahin wandern und wenn nötig neu setzen
        for (short dir = 0; dir < 4; dir++)
            checkPlayerInLine(dir, true);
        if (moveToPos != -1 && monsterPos != moveToPos) {
            switch (face) {
            case 0:
                if (countMonster(monsterPos - 32) == 0) {
                    monsterPos -= 32;
                    nrInPlace = 1;
                } else if (countMonster(monsterPos - 32) == 1) {
                    monsterPos -= 32;
                    nrInPlace = 2;
                }
                break;
            case 1:
                if (countMonster(monsterPos + 32) == 0) {
                    monsterPos += 32;
                    nrInPlace = 1;
                } else if (countMonster(monsterPos + 32) == 1) {
                    monsterPos += 32;
                    nrInPlace = 2;
                }
                break;
            case 2:
                if (countMonster(monsterPos + 1) == 0) {
                    monsterPos += 1;
                    nrInPlace = 1;
                } else if (countMonster(monsterPos + 1) == 1) {
                    monsterPos += 1;
                    nrInPlace = 2;
                }
                break;
            case 3:
                if (countMonster(monsterPos - 1) == 0) {
                    monsterPos -= 1;
                    nrInPlace = 1;
                } else if (countMonster(monsterPos - 1) == 1) {
                    monsterPos -= 1;
                    nrInPlace = 2;
                }
                break;
            default:
                break;
            }
        } else {
            //Spieler nicht mehr im Sichtfeld - dann zurück zu pathType 1
            moveToPos = -1;
            pathType = 1;
        }

        break;
    default:
        break;
    }
    gameMap->monsterInfo[staticMapId][0] = monsterPos;
}

//
// prüfen wieviele Monster sich in der Position befinden
//
short CMonster::countMonster(short mapPos)
{
    short count = 0;

    for (short m = 0; m < gameMap->monsterCount; m++) {
        if (gameMap->monsterInfo[m][0] == mapPos)
            count++;
    }

    return count;
}
