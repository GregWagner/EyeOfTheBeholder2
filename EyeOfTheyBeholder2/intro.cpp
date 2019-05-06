
/**********************************
Class CIntro
***********************************/
#include "intro.h"
#include <cstdio>

//
// Initialisierung
//

void CIntro::init(MEDIAWrapper* mediaObject)
{
    this->mediaObject = mediaObject;

    //CPS Grafiken laden
    mediaObject->loadCPS(51, "original/COIN.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(52, "original/DOORWAY1.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(53, "original/DOORWAY2.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(54, "original/KHELBAN1.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(55, "original/KHELBAN2.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(56, "original/KHELBAN3.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(57, "original/KHELBAN4.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(58, "original/KHELBAN5.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(59, "original/KHELBAN6.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(60, "original/STREET1.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(61, "original/STREET2.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(62, "original/WINDING.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);
    mediaObject->loadCPS(63, "original/CREDITS2.CPS", "original/PALETTE1.PAL", 0, 0, 320, 200, true, true);

    //Setup der Animationsstufen
    sprintf_s(anim[0].fileName, "%s", "mov/intro_seq0");
    anim[0].frames = 5;
    anim[0].keyFrames[0] = 0;
    anim[0].repeat = 1;
    sprintf_s(anim[1].fileName, "%s", "mov/intro_seq1");
    anim[1].frames = 3;
    anim[1].keyFrames[0] = 36;
    anim[1].keyFrames[1] = 232;
    anim[1].keyFrames[2] = 539;
    anim[1].repeat = 3;
    sprintf_s(anim[2].fileName, "%s", "mov/intro_seq2");
    anim[2].frames = 4;
    anim[2].keyFrames[0] = 79;
    anim[2].keyFrames[1] = 249;
    anim[2].repeat = 2;
    sprintf_s(anim[3].fileName, "%s", "mov/intro_seq3");
    anim[3].frames = 3;
    anim[3].keyFrames[0] = 107;
    anim[3].repeat = 1;
    sprintf_s(anim[4].fileName, "%s", "mov/intro_seq4");
    anim[4].frames = 4;
    anim[4].keyFrames[0] = 159;
    anim[4].repeat = 1;
    sprintf_s(anim[5].fileName, "%s", "mov/intro_seq5");
    anim[5].frames = 5;
    anim[5].keyFrames[0] = 163;
    anim[5].keyFrames[1] = 354;
    anim[5].keyFrames[2] = 369;
    anim[5].keyFrames[3] = 427;
    anim[5].repeat = 4;
    sprintf_s(anim[6].fileName, "%s", "mov/intro_seq6");
    anim[6].frames = 2;
    anim[6].keyFrames[0] = 170;
    anim[6].keyFrames[2] = 259;
    anim[6].keyFrames[3] = 431;
    anim[6].keyFrames[4] = 486;
    anim[6].repeat = 4;
    sprintf_s(anim[7].fileName, "%s", "mov/intro_seq7");
    anim[7].frames = 4;
    anim[7].keyFrames[0] = 196;
    anim[7].repeat = 1;
    sprintf_s(anim[8].fileName, "%s", "mov/intro_seq8");
    anim[8].frames = 3;
    anim[8].keyFrames[0] = 229;
    anim[8].repeat = 1;
    sprintf_s(anim[9].fileName, "%s", "mov/intro_seq9");
    anim[9].frames = 7;
    anim[9].keyFrames[0] = 234;
    anim[9].repeat = 1;
    sprintf_s(anim[10].fileName, "%s", "mov/intro_seq10");
    anim[10].frames = 5;
    anim[10].keyFrames[0] = 286;
    anim[10].repeat = 1;
    sprintf_s(anim[11].fileName, "%s", "mov/intro_seq11");
    anim[11].frames = 6;
    anim[11].keyFrames[0] = 349;
    anim[11].repeat = 1;
    sprintf_s(anim[12].fileName, "%s", "mov/intro_seq12");
    anim[12].frames = 2;
    anim[12].keyFrames[0] = 562;
    anim[12].repeat = 1;
    sprintf_s(anim[13].fileName, "%s", "mov/intro_seq13");
    anim[13].frames = 1;
    anim[13].keyFrames[0] = 627;
    anim[13].repeat = 1;
    sprintf_s(anim[14].fileName, "%s", "mov/intro_seq14");
    anim[14].frames = 4;
    anim[14].keyFrames[0] = 675;
    anim[14].repeat = 1;
    sprintf_s(anim[15].fileName, "%s", "mov/intro_seq15");
    anim[15].frames = 3;
    anim[15].keyFrames[0] = 685;
    anim[15].keyFrames[1] = 689;
    anim[15].keyFrames[2] = 692;
    anim[15].keyFrames[3] = 695;
    anim[15].keyFrames[4] = 698;
    anim[15].keyFrames[5] = 702;
    anim[15].keyFrames[6] = 729;
    anim[15].keyFrames[7] = 733;
    anim[15].keyFrames[8] = 736;
    anim[15].keyFrames[9] = 739;
    anim[15].keyFrames[10] = 743;
    anim[15].keyFrames[11] = 746;
    anim[15].keyFrames[12] = 749;
    anim[15].keyFrames[13] = 752;
    anim[15].keyFrames[14] = 755;
    anim[15].repeat = 15;
    sprintf_s(anim[16].fileName, "%s", "mov/intro_seq16");
    anim[16].frames = 3;
    anim[16].keyFrames[0] = 769;
    anim[16].repeat = 1;
    sprintf_s(anim[17].fileName, "%s", "mov/intro_seq17");
    anim[17].frames = 9;
    anim[17].keyFrames[0] = 856;
    anim[17].keyFrames[1] = 869;
    anim[17].keyFrames[2] = 884;
    anim[17].keyFrames[3] = 895;
    anim[17].keyFrames[4] = 906;
    anim[17].keyFrames[5] = 917;
    anim[17].repeat = 6;
    sprintf_s(anim[18].fileName, "%s", "mov/intro_seq18");
    anim[18].frames = 4;
    anim[18].keyFrames[0] = 936;
    anim[18].keyFrames[1] = 942;
    anim[18].keyFrames[2] = 949;
    anim[18].keyFrames[3] = 956;
    anim[18].keyFrames[4] = 965;
    anim[18].keyFrames[5] = 972;
    anim[18].keyFrames[6] = 981;
    anim[18].keyFrames[7] = 991;
    anim[18].keyFrames[8] = 1002;
    anim[18].keyFrames[9] = 1280;
    anim[18].keyFrames[10] = 1300;
    anim[18].keyFrames[11] = 1320;
    anim[18].keyFrames[12] = 1340;
    anim[18].keyFrames[13] = 1360;
    anim[18].keyFrames[14] = 1370;
    anim[18].keyFrames[15] = 1380;
    anim[18].keyFrames[16] = 1400;
    anim[18].keyFrames[17] = 1420;
    anim[18].keyFrames[18] = 1440;
    anim[18].keyFrames[19] = 1460;
    anim[18].keyFrames[20] = 1480;
    anim[18].keyFrames[21] = 1500;
    anim[18].keyFrames[22] = 1527;
    anim[18].repeat = 23;
    sprintf_s(anim[19].fileName, "%s", "mov/intro_seq19");
    anim[19].frames = 4;
    anim[19].keyFrames[0] = 959;
    anim[19].repeat = 1;
    sprintf_s(anim[20].fileName, "%s", "mov/intro_seq20");
    anim[20].frames = 1;
    anim[20].keyFrames[0] = 1013;
    anim[20].repeat = 1;
    sprintf_s(anim[21].fileName, "%s", "mov/intro_seq21");
    anim[21].frames = 3;
    anim[21].keyFrames[0] = 1013;
    anim[21].keyFrames[1] = 1020;
    anim[21].keyFrames[2] = 1030;
    anim[21].keyFrames[3] = 1040;
    anim[21].keyFrames[4] = 1050;
    anim[21].keyFrames[5] = 1070;
    anim[21].keyFrames[6] = 1080;
    anim[21].keyFrames[7] = 1090;
    anim[21].keyFrames[8] = 1110;
    anim[21].keyFrames[9] = 1130;
    anim[21].keyFrames[10] = 1140;
    anim[21].keyFrames[11] = 1160;
    anim[21].keyFrames[12] = 1170;
    anim[21].keyFrames[13] = 1190;
    anim[21].keyFrames[14] = 1210;
    anim[21].repeat = 15;
    sprintf_s(anim[22].fileName, "%s", "mov/intro_seq22");
    anim[22].frames = 5;
    anim[22].keyFrames[0] = 1020;
    anim[22].keyFrames[1] = 1028;
    anim[22].keyFrames[2] = 1033;
    anim[22].keyFrames[3] = 1041;
    anim[22].keyFrames[4] = 1072;
    anim[22].keyFrames[5] = 1088;
    anim[22].keyFrames[6] = 1095;
    anim[22].keyFrames[7] = 1121;
    anim[22].keyFrames[8] = 1129;
    anim[22].keyFrames[9] = 1134;
    anim[22].keyFrames[10] = 1142;
    anim[22].keyFrames[11] = 1148;
    anim[22].keyFrames[12] = 1156;
    anim[22].keyFrames[13] = 1162;
    anim[22].keyFrames[14] = 1170;
    anim[22].repeat = 15;
    sprintf_s(anim[23].fileName, "%s", "mov/intro_seq23");
    anim[23].frames = 3;
    anim[23].keyFrames[0] = 1233;
    anim[23].keyFrames[1] = 1238;
    anim[23].keyFrames[2] = 1242;
    anim[23].keyFrames[3] = 1250;
    anim[23].keyFrames[4] = 1257;
    anim[23].keyFrames[5] = 1264;
    anim[23].keyFrames[6] = 1271;
    anim[23].repeat = 7;
    sprintf_s(anim[24].fileName, "%s", "mov/intro_seq24");
    anim[24].frames = 1;
    anim[24].keyFrames[0] = 1279;
    anim[24].repeat = 1;
    sprintf_s(anim[25].fileName, "%s", "mov/intro_seq25");
    anim[25].frames = 4;
    anim[25].keyFrames[0] = 1289;
    anim[25].keyFrames[1] = 1296;
    anim[25].keyFrames[2] = 1299;
    anim[25].keyFrames[3] = 1306;
    anim[25].keyFrames[4] = 1311;
    anim[25].keyFrames[5] = 1315;
    anim[25].keyFrames[6] = 1318;
    anim[25].keyFrames[7] = 1325;
    anim[25].keyFrames[8] = 1343;
    anim[25].keyFrames[9] = 1349;
    anim[25].keyFrames[10] = 1353;
    anim[25].keyFrames[11] = 1359;
    anim[25].keyFrames[12] = 1364;
    anim[25].keyFrames[13] = 1371;
    anim[25].keyFrames[14] = 1381;
    anim[25].keyFrames[15] = 1412;
    anim[25].keyFrames[16] = 1418;
    anim[25].keyFrames[17] = 1427;
    anim[25].keyFrames[18] = 1437;
    anim[25].keyFrames[19] = 1582;
    anim[25].keyFrames[20] = 1586;
    anim[25].keyFrames[21] = 1593;
    anim[25].keyFrames[22] = 1596;
    anim[25].keyFrames[23] = 1602;
    anim[25].keyFrames[24] = 1629;
    anim[25].keyFrames[25] = 1639;
    anim[25].keyFrames[26] = 1646;
    anim[25].keyFrames[27] = 1656;
    anim[25].keyFrames[28] = 1695;
    anim[25].repeat = 29;
    sprintf_s(anim[26].fileName, "%s", "mov/intro_seq26");
    anim[26].frames = 1;
    anim[26].keyFrames[0] = 1494;
    anim[26].repeat = 1;
    sprintf_s(anim[27].fileName, "%s", "mov/intro_seq27");
    anim[27].frames = 1;
    anim[27].keyFrames[0] = 1539;
    anim[27].repeat = 1;
    sprintf_s(anim[28].fileName, "%s", "mov/intro_seq28");
    anim[28].frames = 6;
    anim[28].keyFrames[0] = 1548;
    anim[28].repeat = 1;
    sprintf_s(anim[29].fileName, "%s", "mov/intro_seq29");
    anim[29].frames = 1;
    anim[29].keyFrames[0] = 1575;
    anim[29].repeat = 1;
    sprintf_s(anim[30].fileName, "%s", "mov/intro_seq30");
    anim[30].frames = 1;
    anim[30].keyFrames[0] = 1703;
    anim[30].repeat = 1;
    sprintf_s(anim[31].fileName, "%s", "mov/intro_seq31");
    anim[31].frames = 1;
    anim[31].keyFrames[0] = 1801;
    anim[31].repeat = 1;
    sprintf_s(anim[32].fileName, "%s", "mov/intro_seq32");
    anim[32].frames = 11;
    anim[32].keyFrames[0] = 1809;
    anim[32].keyFrames[1] = 1822;
    anim[32].keyFrames[2] = 1835;
    anim[32].keyFrames[3] = 1848;
    anim[32].keyFrames[4] = 1861;
    anim[32].repeat = 5;
    sprintf_s(keyframe.fileName, "%s", "mov/intro_keyframes");
    keyframe.frames = 8;
    keyframe.keyFrames[0] = 0;
    keyframe.keyFrames[1] = 627;
    keyframe.keyFrames[2] = 856;
    keyframe.keyFrames[3] = 936;
    keyframe.keyFrames[4] = 1013;
    keyframe.keyFrames[5] = 1279;
    keyframe.keyFrames[6] = 1539;
    keyframe.keyFrames[7] = 1575;
    keyframe.repeat = 0;

    //
    // Bilder mit einzelnen Frames/Keyframes erstellen
    //

    //Frameset 0
    mediaObject->createImage(0, 0, 1600, 200);
    mediaObject->fillRect(0, 0, 1600, 200, 255, 255, 255, 0);
    mediaObject->copyImage(60, 0, 0, 0, 8, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 0, 0, 0, 328, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 0, 0, 0, 648, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 0, 0, 0, 968, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 0, 0, 0, 1288, 8, 320, 128, 0, 0, 0);

    //Frameset 1
    mediaObject->createImage(1, 0, 960, 200);
    mediaObject->fillRect(0, 0, 960, 200, 255, 255, 255, 1);
    mediaObject->copyImage(60, 1, 0, 0, 8, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 1, 0, 0, 328, 8, 320, 128, 0, 0, 0);
    mediaObject->copyImage(60, 1, 0, 0, 648, 8, 320, 128, 0, 0, 0);

    //Frameset 2
    mediaObject->createImage(2, 0, 1280, 200);
    mediaObject->fillRect(0, 0, 1280, 200, 255, 255, 255, 2);
    mediaObject->copyImage(60, 2, 10, 136, 210, 15, 26, 45, 0, 0, 0);
    mediaObject->copyImage(60, 2, 10, 136, 530, 15, 26, 45, 0, 0, 0);
    mediaObject->copyImage(60, 2, 10, 136, 850, 15, 26, 45, 0, 0, 0);
    mediaObject->copyImage(60, 2, 177, -8, 1145, 0, 82, 92, 0, 0, 0);

    //Frameset 3
    mediaObject->createImage(3, 0, 960, 200);
    mediaObject->fillRect(0, 0, 960, 200, 255, 255, 255, 3);
    mediaObject->copyImage(63, 3, 0, 0, 35, 47, 252, 16, 0, 0, 0);
    mediaObject->copyImage(63, 3, 0, 0, 355, 47, 252, 16, 0, 0, 0);
    mediaObject->copyImage(63, 3, 0, 0, 675, 47, 252, 16, 0, 0, 0);

    //Frameset 4
    mediaObject->createImage(4, 0, 1280, 200);
    mediaObject->fillRect(0, 0, 1280, 200, 255, 255, 255, 4);
    mediaObject->copyImage(63, 4, 0, 0, 35, 47, 252, 16, 0, 0, 0);
    mediaObject->copyImage(63, 4, 0, 0, 355, 47, 252, 16, 0, 0, 0);
    mediaObject->copyImage(63, 4, 0, 0, 675, 47, 252, 16, 0, 0, 0);

    //Frameset 5
    mediaObject->createImage(5, 0, 1600, 200);
    mediaObject->fillRect(0, 0, 1600, 200, 255, 255, 255, 5);
    //mediaObject->copyImage(60, 5, 0, 0, 8, 8, 320, 128);
    mediaObject->copyImage(60, 5, 8, 184, 18, 17, 4, 4, 0, 0, 0);
    //mediaObject->copyImage(60, 5, 0, 0, 328, 8, 320, 128);
    mediaObject->copyImage(60, 5, 8, 401, 54, 17, 4, 4, 0, 0, 0);
    //mediaObject->copyImage(60, 5, 0, 0, 648, 8, 320, 128);
    mediaObject->copyImage(60, 5, 8, 744, 88, 17, 4, 4, 0, 0, 0);
    //mediaObject->copyImage(60, 5, 0, 0, 968, 8, 320, 128);
    mediaObject->copyImage(60, 5, 8, 1090, 124, 17, 4, 4, 0, 0, 0);
    //mediaObject->copyImage(60, 5, 0, 0, 1288, 8, 320, 128);

    //Frameset 6

    //Frameset 7

    //Frameset 8

    //Frameset 9

    //Frameset 10

    //Frameset 11

    //Frameset 12

    //Frameset 13

    //Frameset 14

    //Frameset 15

    //Frameset 16

    //Frameset 17

    //Frameset 18

    //Frameset 19

    //Frameset 20

    //Frameset 21

    //Frameset 22

    //Frameset 23

    //Frameset 24

    //Frameset 25

    //Frameset 26

    //Frameset 27

    //Frameset 28

    //Frameset 29

    //Frameset 30

    //Frameset 31

    //Frameset 32

    //Frameset 33
    mediaObject->createImage(33, 0, 2560, 200);
    mediaObject->copyImage(52, 33, 0, 0, 328, 8, 320, 200);
    mediaObject->copyImage(62, 33, 0, 0, 744, 0, 105, 200);
    mediaObject->copyImage(54, 33, 0, 0, 968, 8, 320, 128);
    mediaObject->copyImage(56, 33, 0, 0, 1288, 8, 320, 128);
    mediaObject->copyImage(59, 33, 0, 0, 1608, 8, 320, 128);
    mediaObject->copyImage(51, 33, 103, 0, 2023, 0, 107, 200);
    mediaObject->copyImage(59, 33, 0, 0, 2248, 8, 320, 128);
    /*
	//32 Animationsstufen laden
	for(short i=0; i<33; i++)
	{
		mediaObject->loadImage(i, anim[i].fileName);
	}
	mediaObject->loadImage(33, keyframe.fileName);
	mediaObject->imageCounter = 0;
	*/

    //Setup der Texte
    language[0].sringData = languageData.text[12];
    language[0].from = 369;
    language[0].to = 426;
    language[0].row = 0;
    language[1].sringData = languageData.text[13];
    language[1].from = 369;
    language[1].to = 426;
    language[1].row = 1;
    language[2].sringData = languageData.text[14];
    language[2].from = 427;
    language[2].to = 484;
    language[2].row = 0;
    language[3].sringData = languageData.text[15];
    language[3].from = 486;
    language[3].to = 543;
    language[3].row = 0;
    language[4].sringData = languageData.text[16];
    language[4].from = 486;
    language[4].to = 543;
    language[4].row = 1;
    language[5].sringData = languageData.text[17];
    language[5].from = 545;
    language[5].to = 619;
    language[5].row = 0;
    language[6].sringData = languageData.text[18];
    language[6].from = 632;
    language[6].to = 683;
    language[6].row = 0;
    language[7].sringData = languageData.text[19];
    language[7].from = 685;
    language[7].to = 728;
    language[7].row = 0;
    language[8].sringData = languageData.text[20];
    language[8].from = 729;
    language[8].to = 854;
    language[8].row = 0;
    language[9].sringData = languageData.text[21];
    language[9].from = 946;
    language[9].to = 1000;
    language[9].row = 0;
    language[10].sringData = languageData.text[22];
    language[10].from = 1017;
    language[10].to = 1067;
    language[10].row = 0;
    language[11].sringData = languageData.text[23];
    language[11].from = 1069;
    language[11].to = 1116;
    language[11].row = 0;
    language[12].sringData = languageData.text[24];
    language[12].from = 1069;
    language[12].to = 1116;
    language[12].row = 1;
    language[13].sringData = languageData.text[25];
    language[13].from = 1118;
    language[13].to = 1186;
    language[13].row = 0;
    language[14].sringData = languageData.text[26];
    language[14].from = 1238;
    language[14].to = 1277;
    language[14].row = 0;
    language[15].sringData = languageData.text[27];
    language[15].from = 1289;
    language[15].to = 1341;
    language[15].row = 0;
    language[16].sringData = languageData.text[28];
    language[16].from = 1289;
    language[16].to = 1341;
    language[16].row = 1;
    language[17].sringData = languageData.text[29];
    language[17].from = 1343;
    language[17].to = 1409;
    language[17].row = 0;
    language[18].sringData = languageData.text[30];
    language[18].from = 1411;
    language[18].to = 1473;
    language[18].row = 0;
    language[19].sringData = languageData.text[31];
    language[19].from = 1494;
    language[19].to = 1537;
    language[19].row = 0;
    language[20].sringData = languageData.text[32];
    language[20].from = 1575;
    language[20].to = 1628;
    language[20].row = 0;
    language[21].sringData = languageData.text[33];
    language[21].from = 1629;
    language[21].to = 1682;
    language[21].row = 0;
    language[22].sringData = languageData.text[34];
    language[22].from = 1693;
    language[22].to = 1745;
    language[22].row = 0;
    language[23].sringData = languageData.text[35];
    language[23].from = 1757;
    language[23].to = 1799;
    language[23].row = 0;

    //Musik
    mediaObject->loadSound(0, "sound/intro");
}

void CIntro::play()
{
    short running = 1;
    short currentFrame = 0;
    short frameToShow = 0;
    short tick1 = 0;
    short tick2 = 0;

    while (running == 1) {
        //Frame updaten
        if (tick1 >= tick2) {
            for (short bg = 0; bg < keyframe.frames; bg++)
                if (keyframe.keyFrames[bg] == currentFrame)
                    mediaObject->drawImage(33, 0, 0, 320 * bg, 0, 320, 200);
            if (currentFrame == 0) {
                mediaObject->sleep(2000);
                mediaObject->playSound(0);
            }
            //alle Animation durchlaufen und prüfen welche jetzt angezeigt werden muss
            for (short i = 0; i < 33; i++) {
                for (short a = 0; a < anim[i].repeat; a++) {
                    if (currentFrame >= anim[i].keyFrames[a] && currentFrame <= (anim[i].keyFrames[a] + anim[i].frames)) {
                        frameToShow = currentFrame - anim[i].keyFrames[a];
                        mediaObject->drawImage(i, 0, 0, 320 * frameToShow, 0, 320, 200);
                        //printf("anim[%d].keyFrames[%d]\n", i, a);
                    }
                }
            }
            //Scrolling von Frame 562 - 615
            if (currentFrame >= 562 && currentFrame <= 615) {
                //Bild scrollt in 6 - Pxel Schritten von links nach rechts
                mediaObject->drawImage(12, 0, 0, ((currentFrame - 562) * 6), 0, 320, 200);
            }

            //Texte rendern
            for (short t = 0; t < 24; t++) {
                if (currentFrame >= language[t].from && currentFrame <= language[t].to) {
                    if (language[t].row == 0) {
                        mediaObject->fillRect(0, 140, 320, 60, 0, 0, 0);
                    }

                    if (currentFrame < 685)
                        mediaObject->drawText(0, 150, 150 + (10 * language[t].row), 116, 232, 252, language[t].sringData, true);
                    else
                        mediaObject->drawText(0, 150, 150 + (10 * language[t].row), 255, 255, 255, language[t].sringData, true);
                }
            }

            //Ausblenden von 1866 bis 1888
            if (currentFrame >= 1866 && currentFrame <= 1888) {
                //Helligkeit erhöhen
                mediaObject->setGamma(1 + ((currentFrame - 1866) * 4), 1 + ((currentFrame - 1866) * 4), 1 + ((currentFrame - 1866) * 4));
            }

            //Screen Update
            mediaObject->refresh();

#ifdef WINCE
//
#else
            //Bildschirmrefresh
            mediaObject->updateVideo();
#endif

            tick2 = tick1 + 100;
            currentFrame++;
        }

        // Events verarbeiten
        mediaObject->updateKeys();

        //auf Taste ESC reagieren
        if (mediaObject->mEscapeKey)
            running = 0;

        //auf Mausclick reagieren
        mediaObject->getMouseState(NULL, NULL);
        if (mediaObject->getMouseLeft() || mediaObject->getMouseRight())
            running = 0;

        tick1 = mediaObject->getMilliSeconds();
        if (currentFrame > 1890)
            running = 0;
    }

    //Ausgangswerte setzen
    mediaObject->mEscapeKey = false;
    mediaObject->stopSound(0);
    mediaObject->setGamma(1, 1, 1);
    for (int i = 0; i <= 33; i++)
        mediaObject->freeImage(i);
    for (int i = 51; i <= 63; i++)
        mediaObject->freeImage(i);
    mediaObject->freeSound(0);
}
