#include "item.h"
#include <cstdio>

void Item::init(MediaWrapper* mediaObject, Language* languageData)
{
    mMediaObject = mediaObject;

    // get item icons
    mediaObject->loadCPS(210, "original/ITEMICN.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    // load large items
    mediaObject->loadCPS(211, "original/ITEMS1.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    // load small items
    mediaObject->loadCPS(212, "original/ITEML1.CPS", "original/SILVER.PAL", 0, 0, 320, 200, true);

    /* 
		Item Features Init
		==================
		- largePicID: Which big picture belongs to which item, only comes ITEMS1. CPS (0-30), then ITEML1. CPS (31-45)
		- itemTyp: 0=Nothing, 1=Weapon, 2=Shield(?), 3=Chest, 4=Head, 5=Neck, 6=Wrist, 7=Belt,
                   8=Foot, 9=To Eat, 10=Key, 11=Ring
		- itemName: Name of the item
	*/

    // Item designations
    for (short i = 0; i < 112; i++) {
        sprintf_s(mItemName[i], "%s", languageData->mText[43 + i]);
    }

    // 0 Mauspfeil
    mLargePicID[0] = 0;
    mItemTyp[0] = 0;

    // 1 Sword
    mLargePicID[1] = 31;
    mItemTyp[1] = 1;

    // 2 Dagger
    mLargePicID[2] = 12;
    mItemTyp[2] = 1;

    //3 Helebarde
    mLargePicID[3] = 41;
    mItemTyp[3] = 1;

    // 4 Club
    mLargePicID[4] = 33;
    mItemTyp[4] = 1;

    // 5 Kace with 3 heads
    mLargePicID[5] = 35;
    mItemTyp[5] = 1;

    // 6 Japan Axe
    mLargePicID[6] = 39;
    mItemTyp[6] = 1;

    // 7 Axe
    mLargePicID[7] = 39;
    mItemTyp[7] = 1;

    // 8 Bar
    mLargePicID[8] = 37;
    mItemTyp[8] = 1;

    // 9 Axe
    mLargePicID[9] = 39;
    mItemTyp[9] = 1;

    // 10 Curve
    mLargePicID[10] = 43;
    mItemTyp[10] = 1;

    // 11 Curve
    mLargePicID[11] = 43;
    mItemTyp[11] = 1;

    // 12 Scimitar
    mLargePicID[12] = 31;
    mItemTyp[12] = 1;

    //13 Helebarde
    mLargePicID[13] = 41;
    mItemTyp[13] = 1;

    //14 Wurfpfeil
    mLargePicID[14] = 0;
    mItemTyp[14] = 1;

    //15 Messer
    mLargePicID[15] = 12;
    mItemTyp[15] = 1;

    //16 Pfeil
    mLargePicID[16] = 20;
    mItemTyp[16] = 1;

    //17 Robe
    mLargePicID[17] = 32;
    mItemTyp[17] = 3;

    //18 Schleuder
    mLargePicID[18] = 16;
    mItemTyp[18] = 1;

    //19 Stein
    mLargePicID[19] = 8;
    mItemTyp[19] = 1;

    //20 Helm
    mLargePicID[20] = 24;
    mItemTyp[20] = 4;

    //21 Schuhe
    mLargePicID[21] = 5;
    mItemTyp[21] = 8;

    //22 Schild
    mLargePicID[22] = 45;
    mItemTyp[22] = 2;

    //23 Schild
    mLargePicID[23] = 45;
    mItemTyp[23] = 2;

    //24 Schild
    mLargePicID[24] = 45;
    mItemTyp[24] = 2;

    //25 Armschienen
    mLargePicID[25] = 2;
    mItemTyp[25] = 6;

    //26 Plattenrüstung
    mLargePicID[26] = 34;
    mItemTyp[26] = 3;

    //27 Kreuz
    mLargePicID[27] = 18;
    mItemTyp[27] = 0;

    //28 Stahlrüstung
    mLargePicID[28] = 34;
    mItemTyp[28] = 3;

    //29 Maschenrüstung
    mLargePicID[29] = 34;
    mItemTyp[29] = 3;

    //30 Kettenrüstung
    mLargePicID[30] = 34;
    mItemTyp[30] = 3;

    //31 Lederrüstung
    mLargePicID[31] = 34;
    mItemTyp[31] = 3;

    //32 Hemd
    mLargePicID[32] = 32;
    mItemTyp[32] = 3;

    //33 Amulett
    mLargePicID[33] = 24;
    mItemTyp[33] = 5;

    //34 Amulett
    mLargePicID[34] = 24;
    mItemTyp[34] = 5;

    //35 Buch
    mLargePicID[35] = 13;
    mItemTyp[35] = 0;

    //36 Rolle
    mLargePicID[36] = 17;
    mItemTyp[36] = 0;

    //37 leer
    mLargePicID[37] = 38;
    mItemTyp[37] = 0;

    //38 Essenspaket
    mLargePicID[38] = 25;
    mItemTyp[38] = 9;

    //39 Essenspaket
    mLargePicID[39] = 25;
    mItemTyp[39] = 9;

    //40 rote Potion
    mLargePicID[40] = 14;
    mItemTyp[40] = 9;

    //41 blaue Potion
    mLargePicID[41] = 14;
    mItemTyp[41] = 9;

    //42 grüne Potion
    mLargePicID[42] = 14;
    mItemTyp[42] = 9;

    //43 gezackter Dolch
    mLargePicID[43] = 12;
    mItemTyp[43] = 1;

    //44 Knochen
    mLargePicID[44] = 28;
    mItemTyp[44] = 0;

    //45 Säbel
    mLargePicID[45] = 31;
    mItemTyp[45] = 0;

    //46 Breitschwert
    mLargePicID[46] = 31;
    mItemTyp[46] = 0;

    //47 Knochenschlüssel
    mLargePicID[47] = 1;
    mItemTyp[47] = 10;

    //48 Stahlschlüssel
    mLargePicID[48] = 1;
    mItemTyp[48] = 10;

    //49 Kupferschlüssel
    mLargePicID[49] = 1;
    mItemTyp[49] = 10;

    //50 Schlüssel
    mLargePicID[50] = 1;
    mItemTyp[50] = 10;

    //51 Smaragdschlüssel
    mLargePicID[51] = 1;
    mItemTyp[51] = 10;

    //52 leer
    mLargePicID[52] = 38;
    mItemTyp[52] = 0;

    //53 Maske
    mLargePicID[53] = 24;
    mItemTyp[53] = 4;

    //54 Zauberstab
    mLargePicID[54] = 9;
    mItemTyp[54] = 1;

    //55 Kreuz
    mLargePicID[55] = 18;
    mItemTyp[55] = 0;

    //56 Pfeilköcher
    mLargePicID[56] = 4;
    mItemTyp[56] = 0;

    //57 Ring
    mLargePicID[57] = 29;
    mItemTyp[57] = 11;

    //58 Zahn
    mLargePicID[58] = 12;
    mItemTyp[58] = 0;

    //59 Kristall
    mLargePicID[59] = 26;
    mItemTyp[59] = 0;

    //60 Schlüssel
    mLargePicID[60] = 1;
    mItemTyp[60] = 10;

    //61 Trompete
    mLargePicID[61] = 30;
    mItemTyp[61] = 0;

    //62 Zauberstock
    mLargePicID[62] = 6;
    mItemTyp[62] = 1;

    //63 Zauberkugel
    mLargePicID[63] = 10;
    mItemTyp[63] = 0;

    //64 Steinchen
    mLargePicID[64] = 26;
    mItemTyp[64] = 0;

    //65 Kreuz
    mLargePicID[65] = 18;
    mItemTyp[65] = 0;

    //66 Amulett
    mLargePicID[66] = 24;
    mItemTyp[66] = 5;

    //67 Schwert
    mLargePicID[67] = 31;
    mItemTyp[67] = 1;

    //68 Zepter
    mLargePicID[68] = 6;
    mItemTyp[68] = 1;

    //69 Augenkristall
    mLargePicID[69] = 26;
    mItemTyp[69] = 0;

    //70 Keule
    mLargePicID[70] = 35;
    mItemTyp[70] = 1;

    //71 Schuhe
    mLargePicID[71] = 5;
    mItemTyp[71] = 8;

    //72 Armschienen
    mLargePicID[72] = 2;
    mItemTyp[72] = 6;

    //73 Schild
    mLargePicID[73] = 45;
    mItemTyp[73] = 2;

    //74 Schild
    mLargePicID[74] = 45;
    mItemTyp[74] = 2;

    //75 Helm
    mLargePicID[75] = 24;
    mItemTyp[75] = 4;

    //76 Haube
    mLargePicID[76] = 24;
    mItemTyp[76] = 4;

    //77 Wurfpfeil
    mLargePicID[77] = 0;
    mItemTyp[77] = 1;

    //78 Stab
    mLargePicID[78] = 37;
    mItemTyp[78] = 1;

    //79 Rüstung
    mLargePicID[79] = 34;
    mItemTyp[79] = 3;

    //80 Ring
    mLargePicID[80] = 29;
    mItemTyp[80] = 11;

    //81 Ring
    mLargePicID[81] = 29;
    mItemTyp[81] = 11;

    //82 Zauberhand
    mLargePicID[82] = 38;
    mItemTyp[82] = 0;

    //83 Zauberhand
    mLargePicID[83] = 38;
    mItemTyp[83] = 0;

    //84 Flammenschwert
    mLargePicID[84] = 31;
    mItemTyp[84] = 1;

    //85 Hand links
    mLargePicID[85] = 38;
    mItemTyp[85] = 1;

    //86 Hand rechts
    mLargePicID[86] = 38;
    mItemTyp[86] = 1;

    //87 Rolle
    mLargePicID[87] = 17;
    mItemTyp[87] = 0;

    //88 Pergament
    mLargePicID[88] = 13;
    mItemTyp[88] = 0;

    //89 Totenkopfschlüssel
    mLargePicID[89] = 1;
    mItemTyp[89] = 10;

    //90 Schlüssel
    mLargePicID[90] = 1;
    mItemTyp[90] = 10;

    //91 Totenkopf
    mLargePicID[91] = 28;
    mItemTyp[91] = 0;

    //92 Knochen
    mLargePicID[92] = 28;
    mItemTyp[92] = 0;

    //93 Kristall
    mLargePicID[93] = 26;
    mItemTyp[93] = 0;

    //94 Robe
    mLargePicID[94] = 32;
    mItemTyp[94] = 3;

    //95 Kristall
    mLargePicID[95] = 26;
    mItemTyp[95] = 0;

    //96 Kristall
    mLargePicID[96] = 26;
    mItemTyp[96] = 0;

    //97 Kristall
    mLargePicID[97] = 26;
    mItemTyp[97] = 0;

    //98 Kristall
    mLargePicID[98] = 26;
    mItemTyp[98] = 0;

    //99 Beutel
    mLargePicID[99] = 7;
    mItemTyp[99] = 0;

    //100 Wurfpfeil
    mLargePicID[100] = 0;
    mItemTyp[100] = 1;

    //101 Smaragdhammer
    mLargePicID[101] = 36;
    mItemTyp[101] = 1;

    //102 Kristallkugel
    mLargePicID[102] = 10;
    mItemTyp[102] = 0;

    //103 Hand
    mLargePicID[103] = 38;
    mItemTyp[103] = 0;

    //104 Schlüssel
    mLargePicID[104] = 1;
    mItemTyp[104] = 10;

    //105 Dolch
    mLargePicID[105] = 12;
    mItemTyp[105] = 1;

    //106 Zepter
    mLargePicID[106] = 6;
    mItemTyp[106] = 1;

    //107 Reif
    mLargePicID[107] = 22;
    mItemTyp[107] = 0;

    //108 Münze
    mLargePicID[108] = 3;
    mItemTyp[108] = 0;

    //109 Münze
    mLargePicID[109] = 3;
    mItemTyp[109] = 0;

    //110 Amulett
    mLargePicID[110] = 24;
    mItemTyp[110] = 4;

    //111 Amulettmünze
    mLargePicID[111] = 3;
    mItemTyp[111] = 0;
}

// Item Icons zeichnen
void Item::drawIcon(short itemID, short posX, short posY)
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
    mMediaObject->drawImage(210, posX, posY, x, y, 16, 16);
}
