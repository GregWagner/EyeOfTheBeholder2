
/**********************************
Class CItem
***********************************/
#include "item.h"

//
// Initialisierung
//

void CItem::init(MEDIAWrapper* mediaObject, CEvent* mapEvent, CLanguage* languageData)
{
    this->mediaObject = mediaObject;
    this->mapEvent = mapEvent;
    this->languageData = languageData;

    //Item Icons laden
    mediaObject->loadCPS(210, "original/ITEMICN.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    //grosse Items laden
    mediaObject->loadCPS(211, "original/ITEMS1.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    //kleine Items laden
    mediaObject->loadCPS(212, "original/ITEML1.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    /* 
		Item-Eigenschaften Init 
		=======================
		- largePicID: welches grosses Bild gehört zu welchem Item, erst kommt ITEMS1.CPS (0-30), dann ITEML1.CPS (31-45)
		- itemTyp: 0=nix, 1=Waffe, 2=Schild , 3=Brust, 4=Kopf, 5=Hals, 6=Handgelenk, 7=Gürtel, 8=Fuss, 9=Essen, 10=Schlüssel, 11=Ring
		- itemName: Name des Items
	
	*/

    //Item Bezeichnungen
    for (short i = 0; i < 112; i++)
        sprintf(itemName[i], "%s", languageData->text[43 + i]);

    //0 Mauspfeil
    largePicID[0] = 0;
    itemTyp[0] = 0;

    //1 Schwert
    largePicID[1] = 31;
    itemTyp[1] = 1;

    //2 Dolch
    largePicID[2] = 12;
    itemTyp[2] = 1;

    //3 Helebarde
    largePicID[3] = 41;
    itemTyp[3] = 1;

    //4 Keule
    largePicID[4] = 33;
    itemTyp[4] = 1;

    //5 Keule mit 3 Köpfen
    largePicID[5] = 35;
    itemTyp[5] = 1;

    //6 japan. Axt
    largePicID[6] = 39;
    itemTyp[6] = 1;

    //7 Axt
    largePicID[7] = 39;
    itemTyp[7] = 1;

    //8 Stab
    largePicID[8] = 37;
    itemTyp[8] = 1;

    //9 Axt
    largePicID[9] = 39;
    itemTyp[9] = 1;

    //10 Bogen
    largePicID[10] = 43;
    itemTyp[10] = 1;

    //11 Bogen
    largePicID[11] = 43;
    itemTyp[11] = 1;

    //12 Scimitar
    largePicID[12] = 31;
    itemTyp[12] = 1;

    //13 Helebarde
    largePicID[13] = 41;
    itemTyp[13] = 1;

    //14 Wurfpfeil
    largePicID[14] = 0;
    itemTyp[14] = 1;

    //15 Messer
    largePicID[15] = 12;
    itemTyp[15] = 1;

    //16 Pfeil
    largePicID[16] = 20;
    itemTyp[16] = 1;

    //17 Robe
    largePicID[17] = 32;
    itemTyp[17] = 3;

    //18 Schleuder
    largePicID[18] = 16;
    itemTyp[18] = 1;

    //19 Stein
    largePicID[19] = 8;
    itemTyp[19] = 1;

    //20 Helm
    largePicID[20] = 24;
    itemTyp[20] = 4;

    //21 Schuhe
    largePicID[21] = 5;
    itemTyp[21] = 8;

    //22 Schild
    largePicID[22] = 45;
    itemTyp[22] = 2;

    //23 Schild
    largePicID[23] = 45;
    itemTyp[23] = 2;

    //24 Schild
    largePicID[24] = 45;
    itemTyp[24] = 2;

    //25 Armschienen
    largePicID[25] = 2;
    itemTyp[25] = 6;

    //26 Plattenrüstung
    largePicID[26] = 34;
    itemTyp[26] = 3;

    //27 Kreuz
    largePicID[27] = 18;
    itemTyp[27] = 0;

    //28 Stahlrüstung
    largePicID[28] = 34;
    itemTyp[28] = 3;

    //29 Maschenrüstung
    largePicID[29] = 34;
    itemTyp[29] = 3;

    //30 Kettenrüstung
    largePicID[30] = 34;
    itemTyp[30] = 3;

    //31 Lederrüstung
    largePicID[31] = 34;
    itemTyp[31] = 3;

    //32 Hemd
    largePicID[32] = 32;
    itemTyp[32] = 3;

    //33 Amulett
    largePicID[33] = 24;
    itemTyp[33] = 5;

    //34 Amulett
    largePicID[34] = 24;
    itemTyp[34] = 5;

    //35 Buch
    largePicID[35] = 13;
    itemTyp[35] = 0;

    //36 Rolle
    largePicID[36] = 17;
    itemTyp[36] = 0;

    //37 leer
    largePicID[37] = 38;
    itemTyp[37] = 0;

    //38 Essenspaket
    largePicID[38] = 25;
    itemTyp[38] = 9;

    //39 Essenspaket
    largePicID[39] = 25;
    itemTyp[39] = 9;

    //40 rote Potion
    largePicID[40] = 14;
    itemTyp[40] = 9;

    //41 blaue Potion
    largePicID[41] = 14;
    itemTyp[41] = 9;

    //42 grüne Potion
    largePicID[42] = 14;
    itemTyp[42] = 9;

    //43 gezackter Dolch
    largePicID[43] = 12;
    itemTyp[43] = 1;

    //44 Knochen
    largePicID[44] = 28;
    itemTyp[44] = 0;

    //45 Säbel
    largePicID[45] = 31;
    itemTyp[45] = 0;

    //46 Breitschwert
    largePicID[46] = 31;
    itemTyp[46] = 0;

    //47 Knochenschlüssel
    largePicID[47] = 1;
    itemTyp[47] = 10;

    //48 Stahlschlüssel
    largePicID[48] = 1;
    itemTyp[48] = 10;

    //49 Kupferschlüssel
    largePicID[49] = 1;
    itemTyp[49] = 10;

    //50 Schlüssel
    largePicID[50] = 1;
    itemTyp[50] = 10;

    //51 Smaragdschlüssel
    largePicID[51] = 1;
    itemTyp[51] = 10;

    //52 leer
    largePicID[52] = 38;
    itemTyp[52] = 0;

    //53 Maske
    largePicID[53] = 24;
    itemTyp[53] = 4;

    //54 Zauberstab
    largePicID[54] = 9;
    itemTyp[54] = 1;

    //55 Kreuz
    largePicID[55] = 18;
    itemTyp[55] = 0;

    //56 Pfeilköcher
    largePicID[56] = 4;
    itemTyp[56] = 0;

    //57 Ring
    largePicID[57] = 29;
    itemTyp[57] = 11;

    //58 Zahn
    largePicID[58] = 12;
    itemTyp[58] = 0;

    //59 Kristall
    largePicID[59] = 26;
    itemTyp[59] = 0;

    //60 Schlüssel
    largePicID[60] = 1;
    itemTyp[60] = 10;

    //61 Trompete
    largePicID[61] = 30;
    itemTyp[61] = 0;

    //62 Zauberstock
    largePicID[62] = 6;
    itemTyp[62] = 1;

    //63 Zauberkugel
    largePicID[63] = 10;
    itemTyp[63] = 0;

    //64 Steinchen
    largePicID[64] = 26;
    itemTyp[64] = 0;

    //65 Kreuz
    largePicID[65] = 18;
    itemTyp[65] = 0;

    //66 Amulett
    largePicID[66] = 24;
    itemTyp[66] = 5;

    //67 Schwert
    largePicID[67] = 31;
    itemTyp[67] = 1;

    //68 Zepter
    largePicID[68] = 6;
    itemTyp[68] = 1;

    //69 Augenkristall
    largePicID[69] = 26;
    itemTyp[69] = 0;

    //70 Keule
    largePicID[70] = 35;
    itemTyp[70] = 1;

    //71 Schuhe
    largePicID[71] = 5;
    itemTyp[71] = 8;

    //72 Armschienen
    largePicID[72] = 2;
    itemTyp[72] = 6;

    //73 Schild
    largePicID[73] = 45;
    itemTyp[73] = 2;

    //74 Schild
    largePicID[74] = 45;
    itemTyp[74] = 2;

    //75 Helm
    largePicID[75] = 24;
    itemTyp[75] = 4;

    //76 Haube
    largePicID[76] = 24;
    itemTyp[76] = 4;

    //77 Wurfpfeil
    largePicID[77] = 0;
    itemTyp[77] = 1;

    //78 Stab
    largePicID[78] = 37;
    itemTyp[78] = 1;

    //79 Rüstung
    largePicID[79] = 34;
    itemTyp[79] = 3;

    //80 Ring
    largePicID[80] = 29;
    itemTyp[80] = 11;

    //81 Ring
    largePicID[81] = 29;
    itemTyp[81] = 11;

    //82 Zauberhand
    largePicID[82] = 38;
    itemTyp[82] = 0;

    //83 Zauberhand
    largePicID[83] = 38;
    itemTyp[83] = 0;

    //84 Flammenschwert
    largePicID[84] = 31;
    itemTyp[84] = 1;

    //85 Hand links
    largePicID[85] = 38;
    itemTyp[85] = 1;

    //86 Hand rechts
    largePicID[86] = 38;
    itemTyp[86] = 1;

    //87 Rolle
    largePicID[87] = 17;
    itemTyp[87] = 0;

    //88 Pergament
    largePicID[88] = 13;
    itemTyp[88] = 0;

    //89 Totenkopfschlüssel
    largePicID[89] = 1;
    itemTyp[89] = 10;

    //90 Schlüssel
    largePicID[90] = 1;
    itemTyp[90] = 10;

    //91 Totenkopf
    largePicID[91] = 28;
    itemTyp[91] = 0;

    //92 Knochen
    largePicID[92] = 28;
    itemTyp[92] = 0;

    //93 Kristall
    largePicID[93] = 26;
    itemTyp[93] = 0;

    //94 Robe
    largePicID[94] = 32;
    itemTyp[94] = 3;

    //95 Kristall
    largePicID[95] = 26;
    itemTyp[95] = 0;

    //96 Kristall
    largePicID[96] = 26;
    itemTyp[96] = 0;

    //97 Kristall
    largePicID[97] = 26;
    itemTyp[97] = 0;

    //98 Kristall
    largePicID[98] = 26;
    itemTyp[98] = 0;

    //99 Beutel
    largePicID[99] = 7;
    itemTyp[99] = 0;

    //100 Wurfpfeil
    largePicID[100] = 0;
    itemTyp[100] = 1;

    //101 Smaragdhammer
    largePicID[101] = 36;
    itemTyp[101] = 1;

    //102 Kristallkugel
    largePicID[102] = 10;
    itemTyp[102] = 0;

    //103 Hand
    largePicID[103] = 38;
    itemTyp[103] = 0;

    //104 Schlüssel
    largePicID[104] = 1;
    itemTyp[104] = 10;

    //105 Dolch
    largePicID[105] = 12;
    itemTyp[105] = 1;

    //106 Zepter
    largePicID[106] = 6;
    itemTyp[106] = 1;

    //107 Reif
    largePicID[107] = 22;
    itemTyp[107] = 0;

    //108 Münze
    largePicID[108] = 3;
    itemTyp[108] = 0;

    //109 Münze
    largePicID[109] = 3;
    itemTyp[109] = 0;

    //110 Amulett
    largePicID[110] = 24;
    itemTyp[110] = 4;

    //111 Amulettmünze
    largePicID[111] = 3;
    itemTyp[111] = 0;
}

//
// Item Icons zeichnen
//

void CItem::drawIcon(short itemID, short posX, short posY)
{
    /* - Item Liste
	0 Mauspfeil
	1 Schwert
	2 Dolch
	3 Helebarde
	4 
	5
	6
	7 Axt
	8 Stab
	9 Axt
	10 Bogen
	11 Bogen
	12 Scimitar
	13 Helebarde
	14 Wurfpfeil
	15 Messer
	16 Pfeil
	17 Robe
	18 Schleuder
	19 Stein
	20 Helm
	21 Schuhe
	22 Schild
	23 Schild
	24 Schild
	25 Armschienen
	26 Plattenrüstung
	27 Kreuz
	28 Stahlrüstung
	29 Maschenrüstung
	30 Kettenrüstung
	31 Lederrüstung
	32 Hemd
	33 Amulett
	34 Amulett
	35 Buch
	36 Rolle
	37 
	38 Essenspaket
	39 Essenspaket
	40 rote Potion
	41 blaue Potion
	42 grüne Potion
	43 gezackter Dolch
	44 Knochen
	45 Säbel
	46 Breitschwert
	47 Knochenschlüssel
	48 Stahlschlüssel
	49 Kupferschlüssel
	50 Schlüssel
	51 Smaragdschlüssel
	52
	53 Maske
	54 Stab
	55 Kreuz
	56 Pfeilköcher
	57 Ring
	58 Zahn
	59 Kristall
	60 Schlüssel
	61 Trompete
	62 
	63 
	64
	65 Kreuz
	66 Amulett
	67 Schwert
	68 Zepter
	69
	70
	71 Schuhe
	72 Armschienen
	73 
	74 
	75 Helm
	76 Haube
	77 Pfeil
	78 Stab
	79 
	80 Ring
	81 Ring
	82 Zauberhand
	83 Zauberhand
	84 Flammenschwert
	85 Hand links
	86 Hand rechts
	87 Rolle
	88 Pergament
	89 Totenkopfschlüssel
	90 Schlüssel
	91 Totenkopf
	92 Knochen
	93 Kristall
	94 Robe
	95 Kristall
	96 Kristall
	97 Kristall
	98 Kristall
	99 Beutel
	100 
	101 Smaragdhammer
	102 
	103 Hand
	104 Schlüssel
	105 Dolch
	106 Zepter
	107
	108
	109
	110 Amulett
	111 Amulett
	*/

    short x = (itemID % 20) * 16;
    short y = itemID > 20 ? (itemID / 20 * 16) : 0;
    mediaObject->drawImage(210, posX, posY, x, y, 16, 16);
}
