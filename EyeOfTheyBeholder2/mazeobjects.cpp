#include "mazeobjects.h"
#include "mediawrapper.h"
#include <cstdio>
#include <ctype.h>

//
// Wall initialisation
//

void CMazeObjects::init(short maze_id)
{
    char realpath[128];

    this->level_id = maze_id;

#ifdef WINCE
    sprintf(realpath, "/eob2/original/LEVEL%d.INF", maze_id);
    loadINF(realpath);
#else
    sprintf(realpath, "original/LEVEL%d.INF", maze_id);
    loadINF(realpath);
#endif

    //leere Bilder für die 4 Wandebenen generieren und weiß (transparenz-farbe) füllen
    mediaObject->createImage(260, 0, 176, 120);
    mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 260);
    mediaObject->createImage(261, 0, 176, 120);
    mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 261);
    mediaObject->createImage(262, 0, 176, 120);
    mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 262);
    mediaObject->createImage(263, 0, 176, 120);
    mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 263);

    //2 Hintergründe in Images 164 und 163 generieren
    mediaObject->createImage(163, 0, 176, 120);
    loadWallBackground(false, 163);
    mediaObject->createImage(164, 0, 176, 120);
    loadWallBackground(true, 164);
}

//
// MAZ File mit INF-Verweisen laden
//

void CMazeObjects::loadMAZ(char* file)
{
    //
    // File einlesen
    //

    FILE* source = nullptr;
    int filesize;

    source = fopen(file, "rb");
    if (source == nullptr) {
        printf("file not found: %s\n", file);
    }

    fseek(source, 0, SEEK_END);
    filesize = ftell(source);
    fseek(source, 0, SEEK_SET);

    fread(maz_src, 1, filesize, source);
    fclose(source);

    //Struktur mit Wall-Informationen füllen, Wall Daten beginnen bei Offset 0x6
    int count = 0;
    for (int i = 6; i < 4102; i += 4) {
        maze_data[count].n = maz_src[i];
        maze_data[count].e = maz_src[i + 1];
        maze_data[count].s = maz_src[i + 2];
        maze_data[count].w = maz_src[i + 3];
        //if(count/32 >= 8 && count/32 <= 12)
        //	printf("%d, %d: 0x%x 0x%x 0x%x 0x%x\n", count%32, count/32, maze_data[count].n, maze_data[count].e, maze_data[count].s, maze_data[count].w);
        count++;
    }
}

//
// INF File mit Level Definitionen laden
//

void CMazeObjects::loadINF(char* file)
{
    char realpath[128];

    //
    // File einlesen
    //

    FILE* source = nullptr;
    int filesize;

    source = fopen(file, "rb");
    if (source == nullptr) {
        printf("file not found: %s\n", file);
    }

    fseek(source, 0, SEEK_END);
    filesize = ftell(source);
    fseek(source, 0, SEEK_SET);

    unsigned char inf_tmp_src[64000];
    fread(inf_tmp_src, 1, filesize, source);
    fclose(source);

    //swapped Bytes 0 - 1 => Grösse der komprimierten Datei
    int compressed_filesize = getSwappedWord(inf_tmp_src[0], inf_tmp_src[1]);

    //swapped Bytes 4 - 5 => Grösse der unkomprimierten Datei
    int uncompressed_filesize = getSwappedWord(inf_tmp_src[4], inf_tmp_src[5]);

    //Bytes 10 - n => CPS Daten
    unsigned char compressed_data[64000];
    for (int i = 10; i < 64006; i++)
        compressed_data[i - 10] = inf_tmp_src[i];

    //Format80 codiertes File decodieren
    mediaObject->format80decode(compressed_data, inf_src);
    /*
	FILE *out = NULL;
	out = fopen("out.inf", "wb");
	fwrite(inf_src, 1, uncompressed_filesize, out);
	fclose(out);
*/
    //MAZ Daten laden - Level-Filename steht in Bytes 0x5 - 0x11
    unsigned char maz_filename[13];
    for (int i = 0; i < 12; i++)
        maz_filename[i] = toupper(inf_src[0x5 + i]);
    maz_filename[12] = '\0';

#ifdef WINCE
    sprintf(realpath, "/eob2/original/%s", maz_filename);
    loadMAZ(realpath);
#else
    sprintf(realpath, "original/%s", maz_filename);
    loadMAZ(realpath);
#endif

    //VMP und VCN Daten laden - Level-Filename steht in Bytes 0x12 - 0x19
    unsigned char graphics_filename[8];
    for (int i = 0; i < 7; i++)
        graphics_filename[i] = toupper(inf_src[0x12 + i]);
    graphics_filename[7] = '\0';
    sprintf(current_pal, "original/%s.PAL", graphics_filename);

#ifdef WINCE
    sprintf(realpath, "/eob2/original/%s.VMP", graphics_filename);
    loadVMP(realpath);
    sprintf(realpath, "/eob2/original/%s.VCN", graphics_filename);
    loadVCN(realpath);
    sprintf(realpath, "/eob2/original/%s.PAL", graphics_filename);
    mediaObject->loadPal(realpath);
#else
    sprintf(realpath, "original/%s.VMP", graphics_filename);
    loadVMP(realpath);
    sprintf(realpath, "original/%s.VCN", graphics_filename);
    loadVCN(realpath);
    sprintf(realpath, "original/%s.PAL", graphics_filename);
    mediaObject->loadPal(realpath);
#endif

    //
    // Dekorationsdaten für die Wände auslesen
    //

    //einige hardcoded offsets
    unsigned short decoration_offset;
    switch (level_id) {
    case 1:
        decoration_offset = 0x01F3;
        break;
    case 2:
        decoration_offset = 0x016F;
        break;
    case 3:
        decoration_offset = 0x011D;
        break;
    case 4:
        decoration_offset = 0x00B0;
        break;
    case 5:
        decoration_offset = 0x0130;
        break;
    case 6:
        decoration_offset = 0x011B;
        break;
    case 7:
        decoration_offset = 0x00F0;
        break;
    case 8:
        decoration_offset = 0x0169;
        break;
    case 9:
        decoration_offset = 0x01D1;
        break;
    case 10:
        decoration_offset = 0x0084;
        break;
    case 11:
        decoration_offset = 0x00EA;
        break;
    case 12:
        decoration_offset = 0x00DA;
        break;
    case 13:
        decoration_offset = 0x00EA;
        break;
    case 14:
        decoration_offset = 0x00B7;
        break;
    case 15:
        decoration_offset = 0x0195;
        break;
    case 16:
        decoration_offset = 0x00E1;
        break;
    default:
        printf("wall decoration error\n");
        exit(0);
        break;
    }

    //Mainloop
    unsigned char command;
    count_deco_info = 0;
    int count_deco_data = 0;
    for (int pos = decoration_offset; pos < 64000 - decoration_offset; pos++) {
        command = inf_src[pos];
        char filename[13];
        switch (command) {
        //0xEC = Dekoration Image und Rectangle Daten laden
        case 0xEC:
            //prüfen ob 27 Bytes später Rectangle daten folgen, wenn nicht Abbruch
            if (inf_src[pos + 27] != 0xFB)
                break;

            deco_info[count_deco_info].info_id = count_deco_info;
            pos++;

            //13 Bytes für Decoration-Image Dateinamen
            for (int i = pos; i < pos + 13; i++)
                filename[i - pos] = toupper(inf_src[i]);
            sprintf(deco_info[count_deco_info].image_name, "original/%s.CPS", filename);
            pos += 13;

            //13 Bytes für Rectangle-Daten Dateinamen
            for (int i = pos; i < pos + 13; i++)
                filename[i - pos] = toupper(inf_src[i]);
            sprintf(deco_info[count_deco_info].dec_file_name, "original/%s", filename);
            pos += 13;

            //0xFB = Rectangle Datensätze folgen
            while (inf_src[pos] == 0xFB) {
                deco_info[count_deco_info].decoration_data[count_deco_data].wall_mapping_index = inf_src[pos + 1];
                deco_info[count_deco_info].decoration_data[count_deco_data].wall_type = inf_src[pos + 2];
                deco_info[count_deco_info].decoration_data[count_deco_data].decoration_id = inf_src[pos + 3];
                count_deco_data++;
                pos += 6;
            }

            //weitere Dekoration Images folgen
            while (inf_src[pos] == 0xEC) {
                //prüfen ob 27 Bytes später Rectangle daten folgen, wenn nicht Abbruch
                if (inf_src[pos + 27] != 0xFB)
                    break;

                count_deco_data = 0;
                count_deco_info++;
                deco_info[count_deco_info].info_id = count_deco_info;
                pos++;

                //13 Bytes für Decoration-Image Dateinamen
                for (int i = pos; i < pos + 13; i++)
                    filename[i - pos] = toupper(inf_src[i]);
                sprintf(deco_info[count_deco_info].image_name, "original/%s.CPS", filename);
                pos += 13;

                //13 Bytes für Rectangle-Daten Dateinamen
                for (int i = pos; i < pos + 13; i++)
                    filename[i - pos] = toupper(inf_src[i]);
                sprintf(deco_info[count_deco_info].dec_file_name, "original/%s", filename);
                pos += 13;

                //0xFB = Rectangle Datensätze folgen
                while (inf_src[pos] == 0xFB) {
                    deco_info[count_deco_info].decoration_data[count_deco_data].wall_mapping_index = inf_src[pos + 1];
                    deco_info[count_deco_info].decoration_data[count_deco_data].wall_type = inf_src[pos + 2];
                    deco_info[count_deco_info].decoration_data[count_deco_data].decoration_id = inf_src[pos + 3];
                    count_deco_data++;
                    pos += 6;
                }
            }
            break;
        default:
            break;
        }
    }

    //Türen laden
    mediaObject->loadCPS(281, "original/DOOR1.CPS", current_pal, 0, 0, 320, 200, false);
    mediaObject->loadCPS(282, "original/DOOR2.CPS", current_pal, 0, 0, 320, 200, false);

    //Bilder und Rectangle-Info Dateien der einzelnen deco_info Elemente laden
    unsigned char dec_data[8192];
    for (int i = 0; i <= count_deco_info; i++) {
        //Bild laden
        mediaObject->loadCPS(264 + i, deco_info[i].image_name, current_pal, 0, 0, 320, 200, true, false);
        deco_info[i].image_id = 264 + i;

//.DEC File laden
#ifdef WINCE
        sprintf(realpath, "/eob2/%s", deco_info[i].dec_file_name);
#else
        sprintf(realpath, "%s", deco_info[i].dec_file_name);
#endif
        source = fopen(realpath, "rb");
        if (source == nullptr) {
            printf("file not found: %s\n", file);
        }

        fseek(source, 0, SEEK_END);
        filesize = ftell(source);
        fseek(source, 0, SEEK_SET);

        fread(dec_data, 1, filesize, source);
        fclose(source);

        //Anzahl der Dekorationen
        int dec_pos = 0;
        unsigned short nr_deco = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
        dec_pos += 2;

        //die einzelnen Dekorationen einlesen und die entsprechende Struktur mit Daten füllen
        for (int dc = 0; dc < nr_deco; dc++) {
            //Informationen mit möglichen Bildschirmpositionen - 10 sind möglich
            for (int p = 0; p < 10; p++)
                deco_info[i].decorations[dc].rectangleIndices[p] = dec_data[dec_pos + p];
            dec_pos += 10;

            //Information ob und wo eine Folgedekoration beginnt
            deco_info[i].decorations[dc].linkToNextDecoration = dec_data[dec_pos];
            dec_pos++;

            //Flag-Byte - Bit 1 steuert ob diese Dekoration gespiegelt dargestellt werden soll
            deco_info[i].decorations[dc].flags = dec_data[dec_pos];
            dec_pos++;

            //10 X Koordinaten für die 10 möglichen Screenpositionen für Dekorationen
            for (int p = 0; p < 10; p++) {
                deco_info[i].decorations[dc].xCoords[p] = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
                dec_pos += 2;
            }

            //10 Y Koordinaten für die 10 möglichen Screenpositionen für Dekorationen
            for (int p = 0; p < 10; p++) {
                deco_info[i].decorations[dc].yCoords[p] = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
                dec_pos += 2;
            }
        }

        //Anzahl der Dekoration-Rectangle Daten
        deco_info[i].count_rectangle_data = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
        dec_pos += 2;

        //Rectangle-Daten einlesen
        for (int r = 0; r < deco_info[i].count_rectangle_data; r++) {
            deco_info[i].rectangle_data[r].x = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
            dec_pos += 2;
            deco_info[i].rectangle_data[r].y = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
            dec_pos += 2;
            deco_info[i].rectangle_data[r].w = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
            dec_pos += 2;
            deco_info[i].rectangle_data[r].h = (dec_data[dec_pos + 1] << 8) | dec_data[dec_pos];
            dec_pos += 2;
        }
    }
}

//
// VMP File mit VNC-Definitionen laden
//

void CMazeObjects::loadVMP(char* file)
{
    //
    // File einlesen
    //

    FILE* source = nullptr;
    int filesize;

    source = fopen(file, "rb");
    if (source == nullptr) {
        printf("file not found: %s\n", file);
    }

    fseek(source, 0, SEEK_END);
    filesize = ftell(source);
    fseek(source, 0, SEEK_SET);

    fread(vmp_src, 1, filesize, source);
    fclose(source);

    //Tiledaten in Array einlesen
    int count_tiles = (filesize - 2 - ((22 * 15) * 2)) / (431 * 2);
    int pos = 2 + ((22 * 15) * 2);
    for (int i = 0; i < count_tiles; i++) {
        for (int p = 0; p < 431; p++) {
            //printf("%d- %d\n", filesize, pos + 1);
            tile_data[i][p] = (vmp_src[pos + 1] << 8) | vmp_src[pos];
            pos += 2;
        }
    }
}

//
// VCN File mit Grafikdaten laden
//

void CMazeObjects::loadVCN(char* file)
{
    //
    // File einlesen
    //

    FILE* source = NULL;
    int filesize;

    source = fopen(file, "rb");
    if (source == NULL) {
        printf("file not found: %s\n", file);
    }

    fseek(source, 0, SEEK_END);
    filesize = ftell(source);
    fseek(source, 0, SEEK_SET);

    unsigned char vcn_tmp_src[64000];
    fread(vcn_tmp_src, 1, filesize, source);
    fclose(source);

    //swapped Bytes 0 - 1 => Grösse der komprimierten Datei
    int compressed_filesize = getSwappedWord(vcn_tmp_src[0], vcn_tmp_src[1]);

    //swapped Bytes 4 - 5 => Grösse der unkomprimierten Datei
    int uncompressed_filesize = getSwappedWord(vcn_tmp_src[4], vcn_tmp_src[5]);

    //Bytes 10 - n => CPS Daten
    unsigned char compressed_data[64000];
    for (int i = 10; i < 64006; i++)
        compressed_data[i - 10] = vcn_tmp_src[i];

    //Format80 codiertes File decodieren
    mediaObject->format80decode(compressed_data, vcn_src);

    /*
	FILE *out = NULL;
	out = fopen("out.vcn", "wb");
	fwrite(vcn_src, 1, uncompressed_filesize, out);
	fclose(out);
	*/

    //Bytes 0 - 1 => Anzahl der Blöcke (8x8 Pixel Tiles) in diesem File
    vcn_count_blocks = (vcn_src[1] << 8) | vcn_src[0];
    //printf("%d\n", vcn_count_blocks);

    //Bytes 2 - 17 => 16 Farben der Hintergrundpalette
    unsigned short pos = 0;

    for (int p = 2; p < 18; p++)
        vcn_bg_palette[pos++] = vcn_src[p];

    //Bytes 18 - 33 => 16 Farben der Wandpalette
    pos = 0;
    for (int p = 18; p < 34; p++)
        vcn_wall_palette[pos++] = vcn_src[p];
}

//
// Bytes in einem WORD swappen
//

unsigned short CMazeObjects::getSwappedWord(unsigned char byte_0, unsigned char byte_1)
{
    return (byte_1 << 8) | byte_0;
}

//
// 8x8 Pixel Block mit aktueller Palette zeichnen
//

void CMazeObjects::drawBlock(short block_nr, short posX, short posY, bool wall_palette, bool x_flipp, int imageID)
{
    //Startbyte in VCN Data, jeder Block umfasst 4x8 Byte (32 Byte) - los gehts bei Pos. 0x42
    block_nr -= 1;
    unsigned short offset = 0x42 + (block_nr * 32);

    unsigned char color_1 = 0, color_2 = 0;
    unsigned short pos = 0;
    for (int y = 0; y < 8; y++) {
        if (x_flipp) {
            for (int x = 6; x >= 0; x -= 2) {
                //pro Byte 2 Farben - die ersten und letzen 4 Bit verweisen auf einen Paletten Eintrag
                unsigned char color_data = vcn_src[offset + pos++];

                if (wall_palette) {
                    color_1 = vcn_wall_palette[color_data >> 4];
                    color_2 = vcn_wall_palette[color_data & 0xF];
                } else {
                    color_1 = vcn_bg_palette[color_data >> 4];
                    color_2 = vcn_bg_palette[color_data & 0xF];
                }
                if (color_1 != 0)
                    mediaObject->drawPixel(posX + x, posY + y, color_1, imageID);
                if (color_2 != 0)
                    mediaObject->drawPixel(posX + x - 1, posY + y, color_2, imageID);
            }
        } else {
            for (int x = 0; x < 8; x += 2) {
                //pro Byte 2 Farben - die ersten und letzen 4 Bit verweisen auf einen Paletten Eintrag
                unsigned char color_data = vcn_src[offset + pos++];

                if (wall_palette) {
                    color_1 = vcn_wall_palette[color_data >> 4];
                    color_2 = vcn_wall_palette[color_data & 0xF];
                } else {
                    color_1 = vcn_bg_palette[color_data >> 4];
                    color_2 = vcn_bg_palette[color_data & 0xF];
                }
                if (color_1 != 0)
                    mediaObject->drawPixel(posX + x, posY + y, color_1, imageID);
                if (color_2 != 0)
                    mediaObject->drawPixel(posX + x + 1, posY + y, color_2, imageID);
            }
        }
    }
}

//
// Wall-Hintergrund zeichnen - wenn nötig x-flipped
//

void CMazeObjects::loadWallBackground(bool flipped, int imageID)
{
    //Hintergrund-Tile Daten beginnen bei 0x02 des VMP Files
    int background_offset = 0x02;

    //Zeichenbereich enthält 22 x 15 Blöcke
    short count = 0;
    for (int y = 0; y < 15; y++) {
        if (flipped) {
            for (int x = 21; x >= 0; x--) {
                //WORD mit Rohdaten
                unsigned char data1 = vmp_src[background_offset + count++];
                unsigned char data2 = vmp_src[background_offset + count++];
                unsigned short data = (data2 << 8) | data1;

                //Bit 2 (von 16) von data: 1=block wird gespiegelt
                unsigned char bit2 = (data & 0x7FFF) >> 14;

                //Bits 3 bis 16: block id
                unsigned short block_id = data & 0x3FFF;

                drawBlock(block_id, x * 8, y * 8, false, (bit2 == 1 ? true : false) ^ flipped, imageID);
            }
        } else {
            for (int x = 0; x < 22; x++) {
                //WORD mit Rohdaten
                unsigned char data1 = vmp_src[background_offset + count++];
                unsigned char data2 = vmp_src[background_offset + count++];
                unsigned short data = (data2 << 8) | data1;

                //Bit 2 (von 16) von data: 1=block wird gespiegelt
                unsigned char bit2 = (data & 0x7FFF) >> 14;

                //Bits 3 bis 16: block id
                unsigned short block_id = data & 0x3FFF;

                drawBlock(block_id, x * 8, y * 8, false, (bit2 == 1 ? true : false) ^ flipped, imageID);
            }
        }
    }
}

//
// einzelne Wand zeichnen
//

struct WallData {
    int baseOffset;
    int visibleWidthInBlocks;
    int visibleHeightInBlocks;
    int skipFlag;
    int posX;
    int posY;
    int flipFlag;
} wallRenderData[25] = {
    /* Side-Walls left back */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 1	A-east */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 2	B-east */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 3	C-east */

    /* Side-Walls right back */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 4	E-west */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 5	F-west */
    { 0, 0, 0, 0, 0, 0, 0 }, /* 6	G-west */

    /* Frontwalls back */
    { 32, 2, 5, 4, 0, 24, 0 }, /* 7	B-south */
    { 28, 6, 5, 0, 16, 24, 0 }, /* 8	C-south */
    { 28, 6, 5, 0, 64, 24, 0 }, /* 9	D-south */
    { 28, 6, 5, 0, 112, 24, 0 }, /* 10	E-south */
    { 28, 2, 5, 4, 160, 24, 0 }, /* 11	F-south */

    /* Side walls middle back left */
    { 16, 2, 6, 0, 48, 24, 0 }, /* 12	H-east */
    { -20, 2, 8, 0, 48, 16, 0 }, /* 13	I-east */

    /* Side walls middle back right */
    { -20, 2, 8, 0, 112, 16, 1 }, /* 14	K-west */
    { 16, 2, 6, 0, 112, 24, 1 }, /* 15	L-west */

    /* Frontwalls middle back */
    { 62, 6, 8, 4, 0, 16, 0 }, /* 16	I-south */
    { 58, 10, 8, 0, 48, 16, 0 }, /* 17	J-south */
    { 58, 6, 8, 4, 128, 16, 0 }, /* 18	K-south */

    /* Side walls middle front left */
    { -56, 3, 12, 0, 24, 8, 0 }, /* 19	M-east */

    /* Side walls middle front right */
    { -56, 3, 12, 0, 129, 8, 1 }, /* 20	O-west */

    /* Frontwalls middle front */
    { 151, 3, 12, 13, 0, 8, 0 }, /*21	M-south */
    { 138, 3, 12, 13, 152, 8, 0 }, /*22	O-south */
    { 138, 16, 12, 0, 24, 8, 0 }, /*23	N-south */

    /* Side wall front left */
    { -101, 3, 15, 0, 0, 0, 0 }, /*24	P-east */

    /* Side wall front right */
    { -101, 3, 15, 0, 152, 0, 1 }, /*25	Q-west */
};

bool CMazeObjects::renderWall(int wallSetID, int viewportPos, int imageID)
{

    viewportPos -= 1;

    if (wallSetID != -1 && viewportPos != 25) {
        unsigned short offset = wallRenderData[viewportPos].baseOffset + 101;
        //printf("%d\n", offset);
        for (int y = 0; y < wallRenderData[viewportPos].visibleHeightInBlocks; y++) {
            if (wallRenderData[viewportPos].flipFlag == 0) {
                for (int x = 0; x < wallRenderData[viewportPos].visibleWidthInBlocks; x++) {

                    int xpos = wallRenderData[viewportPos].posX + x;
                    int ypos = y;

                    //WORD mit Rohdaten
                    unsigned short data = tile_data[wallSetID][offset];

                    //Bit 2 (von 16) von data: 1=block wird gespiegelt
                    unsigned char bit2 = ((data & 0x7FFF) >> 14) ^ wallRenderData[viewportPos].flipFlag;

                    //Bits 3 bis 16: block id
                    unsigned short block_id = data & 0x3FFF;

                    drawBlock(block_id, (wallRenderData[viewportPos].posX) + x * 8, (wallRenderData[viewportPos].posY) + y * 8, true, (bit2 == 1 ? true : false), imageID);

                    offset++;
                }
            } else {
                for (int x = wallRenderData[viewportPos].visibleWidthInBlocks - 1; x >= 0; x--) {

                    int xpos = wallRenderData[viewportPos].posX + x;
                    int ypos = y;

                    //WORD mit Rohdaten
                    unsigned short data = tile_data[wallSetID][offset];

                    //Bit 2 (von 16) von data: 1=block wird gespiegelt
                    unsigned char bit2 = ((data & 0x7FFF) >> 14) ^ wallRenderData[viewportPos].flipFlag;

                    //Bits 3 bis 16: block id
                    unsigned short block_id = data & 0x3FFF;

                    drawBlock(block_id, (wallRenderData[viewportPos].posX) + x * 8, (wallRenderData[viewportPos].posY) + y * 8, true, (bit2 == 1 ? true : false), imageID);

                    offset++;
                }
            }
            offset += wallRenderData[viewportPos].skipFlag;
        }
    }

    //soll noch eine Dekoration mit dieser Wand gerendert werden dann wird dies jetzt durchgeführt
    if (do_decoration) {
        //Screenposition der einzelnen Walltypen
        unsigned char screen_coord[26] = {
            0,
            0,
            0,
            0,
            0,
            0,
            3, //3 Felder vor Player ganz links - glatte Wand
            3, //3 Felder vor Player links - glatte Wand
            3, //3 Felder vor Player mitte - glatte Wand
            3, //3 Felder vor Player rechts - glatte Wand
            3, //3 Felder vor Player ganz rechts - glatte Wand
            6, //2 Felder vor Player ganz links - Schräge
            6, //2 Felder vor Player links - Schräge
            6, //2 Felder vor Player ganz rechts - Schräge
            6, //2 Felder vor Player rechts - Schräge
            2, //2 Felder vor Player links - glatte Wand
            2, //2 Felder vor Player mitte - glatte Wand
            2, //2 Felder vor Player rechts - glatte Wand
            5, //direkt vor Player links - Schräge
            5, //direkt vor Player rechts - Schräge
            1, //direkt vor Player links - glatte Wand
            1, //direkt vor Player rechts - glatte Wand
            1, //direkt vor Player mitte - glatte Wand
            4, //neben Player links - Schräge
            4, //neben Player rechts - Schräge
            0 //Playerpos
        };

        //Dekorationen spiegeln für aktuellen Walltyp oder nicht
        unsigned char flip[26] = {
            0,
            0,
            0,
            0,
            0,
            0,
            0, //3 Felder vor Player ganz links - glatte Wand
            0, //3 Felder vor Player links - glatte Wand
            0, //3 Felder vor Player mitte - glatte Wand
            0, //3 Felder vor Player rechts - glatte Wand
            0, //3 Felder vor Player ganz rechts - glatte Wand
            0, //2 Felder vor Player ganz links - Schräge
            0, //2 Felder vor Player links - Schräge
            1, //2 Felder vor Player ganz rechts - Schräge
            1, //2 Felder vor Player rechts - Schräge
            0, //2 Felder vor Player links - glatte Wand
            0, //2 Felder vor Player mitte - glatte Wand
            0, //2 Felder vor Player rechts - glatte Wand
            0, //direkt vor Player links - Schräge
            1, //direkt vor Player rechts - Schräge
            0, //direkt vor Player links - glatte Wand
            0, //direkt vor Player rechts - glatte Wand
            0, //direkt vor Player mitte - glatte Wand
            0, //neben Player links - Schräge
            1, //neben Player rechts - Schräge
            0 //Playerpos
        };

        //X*8 offset der Dekoration für den aktuellen Walltyp
        unsigned short x_offset[26] = {
            0,
            0,
            0,
            0,
            0,
            0,
            -96, //3 Felder vor Player ganz links - glatte Wand
            -48, //3 Felder vor Player links - glatte Wand
            0, //3 Felder vor Player mitte - glatte Wand
            48, //3 Felder vor Player rechts - glatte Wand
            96, //3 Felder vor Player ganz rechts - glatte Wand
            0, //2 Felder vor Player ganz links - Schräge
            0, //2 Felder vor Player links - Schräge
            0, //2 Felder vor Player ganz rechts - Schräge
            0, //2 Felder vor Player rechts - Schräge
            -80, //2 Felder vor Player links - glatte Wand
            0, //2 Felder vor Player mitte - glatte Wand
            80, //2 Felder vor Player rechts - glatte Wand
            0, //direkt vor Player links - Schräge
            0, //direkt vor Player rechts - Schräge
            -128, //direkt vor Player links - glatte Wand
            128, //direkt vor Player rechts - glatte Wand
            0, //direkt vor Player mitte - glatte Wand
            0, //neben Player links - Schräge
            0, //neben Player rechts - Schräge
            0 //Playerpos
        };

        //x-offset bei Dekorationen ohne Hintergrundwand
        unsigned short wall_less_x_offset[26] = {
            0,
            0,
            0,
            0,
            0,
            0,
            -88, //3 Felder vor Player ganz links - glatte Wand
            -40, //3 Felder vor Player links - glatte Wand
            0, //3 Felder vor Player mitte - glatte Wand
            40, //3 Felder vor Player rechts - glatte Wand
            88, //3 Felder vor Player ganz rechts - glatte Wand
            0, //2 Felder vor Player ganz links - Schräge
            0, //2 Felder vor Player links - Schräge
            0, //2 Felder vor Player ganz rechts - Schräge
            0, //2 Felder vor Player rechts - Schräge
            -59, //2 Felder vor Player links - glatte Wand
            0, //2 Felder vor Player mitte - glatte Wand
            59, //2 Felder vor Player rechts - glatte Wand
            0, //direkt vor Player links - Schräge
            0, //direkt vor Player rechts - Schräge
            -80, //direkt vor Player links - glatte Wand
            80, //direkt vor Player rechts - glatte Wand
            0, //direkt vor Player mitte - glatte Wand
            0, //neben Player links - Schräge
            0, //neben Player rechts - Schräge
            0 //Playerpos
        };

        //Bild in welchem Deko-Daten zu finden sind
        int image_id = deco_info[do_decoration_wmi].image_id;

        unsigned char pos = screen_coord[viewportPos];
        unsigned char current_rectangle = deco_info[do_decoration_wmi].decorations[do_decoration_id].rectangleIndices[pos];
        unsigned char mirror_deco = deco_info[do_decoration_wmi].decorations[do_decoration_id].flags & 0x01;

        if (screen_coord[viewportPos] > -1 && current_rectangle != 0xFF) {
            //X,Y Position und Breite,Höhe des Ausschnittes welcher aus dem Deko-Bild kopiert wird
            unsigned short from_x = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].x * 8;
            unsigned short from_y = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].y;
            unsigned short width = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].w * 8;
            unsigned short height = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].h;

            //X,Y Position wo die Dekoration projeziert wird
            unsigned short to_x = deco_info[do_decoration_wmi].decorations[do_decoration_id].xCoords[pos];
            unsigned short to_y = deco_info[do_decoration_wmi].decorations[do_decoration_id].yCoords[pos];

            //jetzt Dekoration zeichnen
            if (flip[viewportPos] || mirror_deco == 1)
                mediaObject->copyScaledImage(image_id, imageID, from_x, from_y, 176 - to_x - width + (do_decoration_walltype == 0 ? wall_less_x_offset[viewportPos] : x_offset[viewportPos]) - 1, to_y, width, height, width, height, 255, 255, 255, true);
            else
                mediaObject->copyImage(image_id, imageID, from_x, from_y, to_x + (do_decoration_walltype == 0 ? wall_less_x_offset[viewportPos] : x_offset[viewportPos]), to_y, width, height, 255, 255, 255);
        }

        //eventuelle Folgedekorationen verarbeiten
        do_decoration_id = deco_info[do_decoration_wmi].decorations[do_decoration_id].linkToNextDecoration;

        while (do_decoration_id > 0) {
            unsigned char current_rectangle = deco_info[do_decoration_wmi].decorations[do_decoration_id].rectangleIndices[pos];
            unsigned char mirror_deco = deco_info[do_decoration_wmi].decorations[do_decoration_id].flags & 0x01;

            if (screen_coord[viewportPos] > -1 && current_rectangle != 0xFF) {
                //X,Y Position und Breite,Höhe des Ausschnittes welcher aus dem Deko-Bild kopiert wird
                unsigned short from_x = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].x * 8;
                unsigned short from_y = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].y;
                unsigned short width = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].w * 8;
                unsigned short height = deco_info[do_decoration_wmi].rectangle_data[current_rectangle].h;

                //X,Y Position wo die Dekoration projeziert wird
                unsigned short to_x = deco_info[do_decoration_wmi].decorations[do_decoration_id].xCoords[pos];
                unsigned short to_y = deco_info[do_decoration_wmi].decorations[do_decoration_id].yCoords[pos];

                //jetzt Dekoration zeichnen
                if (flip[viewportPos] || mirror_deco == 1)
                    mediaObject->copyScaledImage(image_id, imageID, from_x, from_y, 176 - to_x - width + (do_decoration_walltype == 0 ? wall_less_x_offset[viewportPos] : x_offset[viewportPos]) - 1, to_y, width, height, width, height, 255, 255, 255, true);
                else
                    mediaObject->copyImage(image_id, imageID, from_x, from_y, to_x + (do_decoration_walltype == 0 ? wall_less_x_offset[viewportPos] : x_offset[viewportPos]), to_y, width, height, 255, 255, 255);
            }
            do_decoration_id = deco_info[do_decoration_wmi].decorations[do_decoration_id].linkToNextDecoration;
        }
    }

    return true;
}

//
// Wandfront zeichnen - level 4 (ganz hinten) bis 1 (neben Player)
//

void CMazeObjects::renderWalls(int level, int playerPos, int playerFace)
{
    /*
	
	MAZ Value	WallMappingIndex Value
	0									keine Wand
	1			0						Wand Type 1
	2			1						Wand Type 2
	3 .. 22								Türen
	23			3						Treppenstufen
	24			4						Treppenstufen
	=================================================

	- playerFace: 0=N /\, 1=S \/, 2=E >, 3=W <

	*/

    switch (playerFace) {
    case 0:
        switch (level) {
        case 1:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 263);
            current_imageID = 263;
            renderWall(getRealWallID(maze_data[playerPos - 2 - 32 - 32 - 32].s, 7, playerPos - 2 - 32 - 32 - 32), 7, 263); //B
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32 - 32 - 32].s, 8, playerPos - 1 - 32 - 32 - 32), 8, 263); //C
            renderWall(getRealWallID(maze_data[playerPos - 32 - 32 - 32].s, 9, playerPos - 32 - 32 - 32), 9, 263); //D
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32 - 32 - 32].s, 10, playerPos + 1 - 32 - 32 - 32), 10, 263); //E
            renderWall(getRealWallID(maze_data[playerPos + 2 - 32 - 32 - 32].s, 11, playerPos + 2 - 32 - 32 - 32), 11, 263); //F
            mediaObject->drawImage(263, 0, 0, 0, 0, 176, 120);
            break;
        case 2:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 262);
            current_imageID = 262;
            //renderWall(getRealWallID(maze_data[playerPos-2-32-32].e), 12, 262);	//H
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32 - 32].e, 13, playerPos - 1 - 32 - 32), 13, 262); //I
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32 - 32].w, 14, playerPos + 1 - 32 - 32), 14, 262); //K
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32 - 32].s, 16, playerPos - 1 - 32 - 32), 16, 262); //I
            renderWall(getRealWallID(maze_data[playerPos - 32 - 32].s, 17, playerPos - 32 - 32), 17, 262); //J
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32 - 32].s, 18, playerPos + 1 - 32 - 32), 18, 262); //K
            //renderWall(getRealWallID(maze_data[playerPos+2-32-32].w), 15, 262);	//L
            mediaObject->drawImage(262, 0, 0, 0, 0, 176, 120);
            break;
        case 3:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 261);
            current_imageID = 261;
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32].e, 19, playerPos - 1 - 32), 19, 261); //M
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32].w, 20, playerPos + 1 - 32), 20, 261); //O
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32].s, 21, playerPos - 1 - 32), 21, 261); //M
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32].s, 22, playerPos + 1 - 32), 22, 261); //O
            renderWall(getRealWallID(maze_data[playerPos - 32].s, 23, playerPos - 32), 23, 261); //N
            mediaObject->drawImage(261, 0, 0, 0, 0, 176, 120);
            break;
        case 4:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 260);
            current_imageID = 260;
            renderWall(getRealWallID(maze_data[playerPos - 1].e, 24, playerPos - 1), 24, 260); //P
            renderWall(getRealWallID(maze_data[playerPos + 1].w, 25, playerPos + 1), 25, 260); //Q
            renderWall(getRealWallID(maze_data[playerPos].n, 26, playerPos), 26, 260); //Playerpos
            mediaObject->drawImage(260, 0, 0, 0, 0, 176, 120);
            break;
        default:
            break;
        }
        break;
    case 1:
        switch (level) {
        case 1:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 263);
            current_imageID = 263;
            renderWall(getRealWallID(maze_data[playerPos + 2 + 32 + 32 + 32].n, 7, playerPos + 2 + 32 + 32 + 32), 7, 263); //B
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32 + 32 + 32].n, 8, playerPos + 1 + 32 + 32 + 32), 8, 263); //C
            renderWall(getRealWallID(maze_data[playerPos + 32 + 32 + 32].n, 9, playerPos + 32 + 32 + 32), 9, 263); //D
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32 + 32 + 32].n, 10, playerPos - 1 + 32 + 32 + 32), 10, 263); //E
            renderWall(getRealWallID(maze_data[playerPos - 2 + 32 + 32 + 32].n, 11, playerPos - 2 + 32 + 32 + 32), 11, 263); //F
            mediaObject->drawImage(263, 0, 0, 0, 0, 176, 120);
            break;
        case 2:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 262);
            current_imageID = 262;
            //renderWall(getRealWallID(maze_data[playerPos+2+32+32].w), 12, 262);	//H
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32 + 32].w, 13, playerPos + 1 + 32 + 32), 13, 262); //I
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32 + 32].e, 14, playerPos - 1 + 32 + 32), 14, 262); //K
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32 + 32].n, 16, playerPos + 1 + 32 + 32), 16, 262); //I
            renderWall(getRealWallID(maze_data[playerPos + 32 + 32].n, 17, playerPos + 32 + 32), 17, 262); //J
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32 + 32].n, 18, playerPos - 1 + 32 + 32), 18, 262); //K
            //renderWall(getRealWallID(maze_data[playerPos-2+32+32].e), 15, 262);	//L
            mediaObject->drawImage(262, 0, 0, 0, 0, 176, 120);
            break;
        case 3:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 261);
            current_imageID = 261;
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32].w, 19, playerPos + 1 + 32), 19, 261); //M
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32].e, 20, playerPos - 1 + 32), 20, 261); //O
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32].n, 21, playerPos + 1 + 32), 21, 261); //M
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32].n, 22, playerPos - 1 + 32), 22, 261); //O
            renderWall(getRealWallID(maze_data[playerPos + 32].n, 23, playerPos + 32), 23, 261); //N
            mediaObject->drawImage(261, 0, 0, 0, 0, 176, 120);
            break;
        case 4:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 260);
            current_imageID = 260;
            renderWall(getRealWallID(maze_data[playerPos + 1].w, 24, playerPos + 1), 24, 260); //P
            renderWall(getRealWallID(maze_data[playerPos - 1].e, 25, playerPos - 1), 25, 260); //Q
            renderWall(getRealWallID(maze_data[playerPos].s, 26, playerPos), 26, 260); //Playerpos
            mediaObject->drawImage(260, 0, 0, 0, 0, 176, 120);
            break;
        default:
            break;
        }
        break;
    case 2:
        switch (level) {
        case 1:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 263);
            current_imageID = 263;
            renderWall(getRealWallID(maze_data[playerPos + 3 - 32 - 32].w, 7, playerPos + 3 - 32 - 32), 7, 263); //B
            renderWall(getRealWallID(maze_data[playerPos + 3 - 32].w, 8, playerPos + 3 - 32), 8, 263); //C
            renderWall(getRealWallID(maze_data[playerPos + 3].w, 9, playerPos + 3), 9, 263); //D
            renderWall(getRealWallID(maze_data[playerPos + 3 + 32].w, 10, playerPos + 3 + 32), 10, 263); //E
            renderWall(getRealWallID(maze_data[playerPos + 3 + 32 + 32].w, 11, playerPos + 3 + 32 + 32), 11, 263); //F
            mediaObject->drawImage(263, 0, 0, 0, 0, 176, 120);
            break;
        case 2:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 262);
            current_imageID = 262;
            //renderWall(getRealWallID(maze_data[playerPos+2-32-32].s), 12, 262);	//H
            renderWall(getRealWallID(maze_data[playerPos + 2 - 32].s, 13, playerPos + 2 - 32), 13, 262); //I
            renderWall(getRealWallID(maze_data[playerPos + 2 + 32].n, 14, playerPos + 2 + 32), 14, 262); //K
            renderWall(getRealWallID(maze_data[playerPos + 2 - 32].w, 16, playerPos + 2 - 32), 16, 262); //I
            renderWall(getRealWallID(maze_data[playerPos + 2].w, 17, playerPos + 2), 17, 262); //J
            renderWall(getRealWallID(maze_data[playerPos + 2 + 32].w, 18, playerPos + 2 + 32), 18, 262); //K
            //renderWall(getRealWallID(maze_data[playerPos+2+32+32].n), 15, 262);	//L
            mediaObject->drawImage(262, 0, 0, 0, 0, 176, 120);
            break;
        case 3:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 261);
            current_imageID = 261;
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32].s, 19, playerPos + 1 - 32), 19, 261); //M
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32].n, 20, playerPos + 1 + 32), 20, 261); //O
            renderWall(getRealWallID(maze_data[playerPos + 1 - 32].w, 21, playerPos + 1 - 32), 21, 261); //M
            renderWall(getRealWallID(maze_data[playerPos + 1 + 32].w, 22, playerPos + 1 + 32), 22, 261); //O
            renderWall(getRealWallID(maze_data[playerPos + 1].w, 23, playerPos + 1), 23, 261); //N
            mediaObject->drawImage(261, 0, 0, 0, 0, 176, 120);
            break;
        case 4:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 260);
            current_imageID = 260;
            renderWall(getRealWallID(maze_data[playerPos - 32].s, 24, playerPos - 32), 24, 260); //P
            renderWall(getRealWallID(maze_data[playerPos + 32].n, 25, playerPos + 32), 25, 260); //Q
            renderWall(getRealWallID(maze_data[playerPos].e, 26, playerPos), 26, 260); //Playerpos
            mediaObject->drawImage(260, 0, 0, 0, 0, 176, 120);
            break;
        default:
            break;
        }
        break;
    case 3:
        switch (level) {
        case 1:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 263);
            current_imageID = 263;
            renderWall(getRealWallID(maze_data[playerPos - 3 + 32 + 32].e, 7, playerPos - 3 + 32 + 32), 7, 263); //B
            renderWall(getRealWallID(maze_data[playerPos - 3 + 32].e, 8, playerPos - 3 + 32), 8, 263); //C
            renderWall(getRealWallID(maze_data[playerPos - 3].e, 9, playerPos - 3), 9, 263); //D
            renderWall(getRealWallID(maze_data[playerPos - 3 - 32].e, 10, playerPos - 3 - 32), 10, 263); //E
            renderWall(getRealWallID(maze_data[playerPos - 3 - 32 - 32].e, 11, playerPos - 3 - 32 - 32), 11, 263); //F
            mediaObject->drawImage(263, 0, 0, 0, 0, 176, 120);
            break;
        case 2:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 262);
            current_imageID = 262;
            //renderWall(getRealWallID(maze_data[playerPos-2+32+32].n), 12, 262);	//H
            renderWall(getRealWallID(maze_data[playerPos - 2 + 32].n, 13, playerPos - 2 + 32), 13, 262); //I
            renderWall(getRealWallID(maze_data[playerPos - 2 - 32].s, 14, playerPos - 2 - 32), 14, 262); //K
            renderWall(getRealWallID(maze_data[playerPos - 2 + 32].e, 16, playerPos - 2 + 32), 16, 262); //I
            renderWall(getRealWallID(maze_data[playerPos - 2].e, 17, playerPos - 2), 17, 262); //J
            renderWall(getRealWallID(maze_data[playerPos - 2 - 32].e, 18, playerPos - 2 - 32), 18, 262); //K
            //renderWall(getRealWallID(maze_data[playerPos-2-32-32].s), 15, 262);	//L
            mediaObject->drawImage(262, 0, 0, 0, 0, 176, 120);
            break;
        case 3:
            //printf("%d,%d (%d):  %d,%d:%d,%d,%d,%d -- %d,%d:%d,%d,%d,%d -- %d,%d:%d,%d,%d,%d\n", playerPos%32, playerPos/32, playerPos, (playerPos-1+32)%32, (playerPos-1+32)/32, maze_data[playerPos-1+32].n, maze_data[playerPos-1+32].s, maze_data[playerPos-1+32].e, maze_data[playerPos-1+32].w, (playerPos-1)%32, (playerPos-1)/32, maze_data[playerPos-1].n, maze_data[playerPos-1].s, maze_data[playerPos-1].e, maze_data[playerPos-1].w, (playerPos-1-32)%32, (playerPos-1-32)/32+1, maze_data[playerPos-1].n, maze_data[playerPos-1].s, maze_data[playerPos-1].e, maze_data[playerPos-1].w);
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 261);
            current_imageID = 261;
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32].n, 19, playerPos - 1 + 32), 19, 261); //M
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32].s, 20, playerPos - 1 - 32), 20, 261); //O
            renderWall(getRealWallID(maze_data[playerPos - 1 + 32].e, 21, playerPos - 1 + 32), 21, 261); //M
            renderWall(getRealWallID(maze_data[playerPos - 1 - 32].e, 22, playerPos - 1 - 32), 22, 261); //O
            renderWall(getRealWallID(maze_data[playerPos - 1].e, 23, playerPos - 1), 23, 261); //N
            mediaObject->drawImage(261, 0, 0, 0, 0, 176, 120);
            break;
        case 4:
            mediaObject->fillRect(0, 0, 176, 120, 255, 255, 255, 260);
            current_imageID = 260;
            renderWall(getRealWallID(maze_data[playerPos + 32].n, 24, playerPos + 32), 24, 260); //P
            renderWall(getRealWallID(maze_data[playerPos - 32].s, 25, playerPos - 32), 25, 260); //Q
            renderWall(getRealWallID(maze_data[playerPos].w, 26, playerPos), 26, 260); //Playerpos
            mediaObject->drawImage(260, 0, 0, 0, 0, 176, 120);
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

//
// .MAZ Wall ID in reale Wall ID umwandeln
//

signed char CMazeObjects::getRealWallID(int maz_id, int wall_id, int maz_pos)
{
    /*
	
	MAZ Value	WallMappingIndex Value
	0									keine Wand
	1			0						Wand Type 1
	2			1						Wand Type 2
	3 .. 22								Türen
	23			3						Treppenstufen
	24			4						Treppenstufen
	=================================================

	- playerFace: 0=N /\, 1=S \/, 2=E >, 3=W <

	*/

    current_pos = maz_pos;
    do_decoration = false;

    //wenn eine Wall ID z.B. per Event gesetzt wurde wird nur diese angezeigt
    if (cell_info[current_pos].wall_code > 0) {
        maz_id = cell_info[current_pos].wall_code;
        if (cell_info[current_pos].wall_code == 1111)
            maz_id = 0;
        maze_data[current_pos].e = maz_id;
        maze_data[current_pos].w = maz_id;
        maze_data[current_pos].n = maz_id;
        maze_data[current_pos].s = maz_id;
    }

    //keine Wand
    if (maz_id == 0)
        return -1;

    //Wand Typ 1
    if (maz_id == 1)
        return 1;

    //Wand Typ 2
    if (maz_id == 2)
        return 0;

    //Türen
    if (maz_id >= 3 && maz_id <= 22)
        return renderDoor(maz_id, wall_id);

    //Treppe
    if (maz_id == 23)
        return 3;

    //Treppe
    if (maz_id == 24)
        return 4;

    //Dekorationen rendern
    if (maz_id > 24)
        return computeDecoration(maz_id);

    return -1;
}

//
// Wand Dekoration rendern
//

int CMazeObjects::computeDecoration(int maz_id)
{
    // *Bug-Alarm* Deco-Anpassungen für (unerklärliche) fehlende Dekos
    if (maz_id == 47)
        maz_id = 42; //Knopf in Wand
    if (maz_id == 49)
        maz_id = 42; //Knopf in Wand
    if (maz_id == 67)
        maz_id = 42; //Knopf in Wand
    if (maz_id == 55)
        maz_id = 42; //Knopf in Wand
    if (maz_id == 48)
        maz_id = 42; //Knopf in Wand

    //passende Deko suchen und für späteres Rendern vorbereiten
    for (int i = 0; i < 16; i++) {
        for (int d = 0; d < 256; d++) {
            if (deco_info[i].decoration_data[d].wall_mapping_index == maz_id) {
                //die Dekoration mit dieser ID wird nach der Wand gezeichnet
                if (deco_info[i].decoration_data[d].decoration_id > -1) {
                    do_decoration_id = deco_info[i].decoration_data[d].decoration_id;
                    do_decoration_walltype = deco_info[i].decoration_data[d].wall_type;
                    do_decoration_wmi = i;
                    if (do_decoration_id != 255)
                        do_decoration = true;
                }
                if (do_decoration_walltype == 0)
                    return -1;
                else
                    return do_decoration_walltype;
            }
        }
    }
    return -1;
}

//
// Türen zeichnen
//

int CMazeObjects::renderDoor(int maz_id, int wall_id)
{
    wall_id -= 1;

    unsigned short x_offset[25] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0, //3 Felder vor Player ganz links - glatte Wand
        29, //3 Felder vor Player links - glatte Wand
        73, //3 Felder vor Player mitte - glatte Wand
        117, //3 Felder vor Player rechts - glatte Wand
        0, //3 Felder vor Player ganz rechts - glatte Wand
        0, //2 Felder vor Player ganz links - Schräge
        0, //2 Felder vor Player links - Schräge
        0, //2 Felder vor Player ganz rechts - Schräge
        0, //2 Felder vor Player rechts - Schräge
        13, //2 Felder vor Player links - glatte Wand
        71, //2 Felder vor Player mitte - glatte Wand
        128, //2 Felder vor Player rechts - glatte Wand
        0, //direkt vor Player links - Schräge
        0, //direkt vor Player rechts - Schräge
        -35, //direkt vor Player links - glatte Wand
        140, //direkt vor Player rechts - glatte Wand
        58, //direkt vor Player mitte - glatte Wand
        0, //neben Player links - Schräge
        0 //neben Player rechts - Schräge
    };
    unsigned short y_offset[25] = {
        0,
        0,
        0,
        0,
        0,
        0,
        0, //3 Felder vor Player ganz links - glatte Wand
        31, //3 Felder vor Player links - glatte Wand
        31, //3 Felder vor Player mitte - glatte Wand
        31, //3 Felder vor Player rechts - glatte Wand
        0, //3 Felder vor Player ganz rechts - glatte Wand
        0, //2 Felder vor Player ganz links - Schräge
        0, //2 Felder vor Player links - Schräge
        0, //2 Felder vor Player ganz rechts - Schräge
        0, //2 Felder vor Player rechts - Schräge
        27, //2 Felder vor Player links - glatte Wand
        27, //2 Felder vor Player mitte - glatte Wand
        27, //2 Felder vor Player rechts - glatte Wand
        0, //direkt vor Player links - Schräge
        0, //direkt vor Player rechts - Schräge
        21, //direkt vor Player links - glatte Wand
        21, //direkt vor Player rechts - glatte Wand
        21, //direkt vor Player mitte - glatte Wand
        0, //neben Player links - Schräge
        0 //neben Player rechts - Schräge
    };
    unsigned short doorsize[25] = {
        0,
        0,
        0,
        0,
        0,
        0,
        3, //3 Felder vor Player ganz links - glatte Wand
        3, //3 Felder vor Player links - glatte Wand
        3, //3 Felder vor Player mitte - glatte Wand
        3, //3 Felder vor Player rechts - glatte Wand
        3, //3 Felder vor Player ganz rechts - glatte Wand
        0, //2 Felder vor Player ganz links - Schräge
        0, //2 Felder vor Player links - Schräge
        0, //2 Felder vor Player ganz rechts - Schräge
        0, //2 Felder vor Player rechts - Schräge
        2, //2 Felder vor Player links - glatte Wand
        2, //2 Felder vor Player mitte - glatte Wand
        2, //2 Felder vor Player rechts - glatte Wand
        0, //direkt vor Player links - Schräge
        0, //direkt vor Player rechts - Schräge
        1, //direkt vor Player links - glatte Wand
        1, //direkt vor Player rechts - glatte Wand
        1, //direkt vor Player mitte - glatte Wand
        0, //neben Player links - Schräge
        0 //neben Player rechts - Schräge
    };

    if (wall_id == -1 || y_offset[wall_id] == 0)
        return -1;

    //Türtypen - ID Level spezifisch zuweisen
    unsigned char level_doortype[17][20] = {
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, // ungenutzt
        { 0, 0, 0, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 }, //Level 1
        { 0, 0, 0, 5, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 }, //Level 2
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 3
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 4
        { 0, 0, 0, 2, 0, 0, 0, 0, 1, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0 }, //Level 5
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 6
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 7
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 8
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 9
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 10
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 11
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 12
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 13
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 14
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }, //Level 15
        { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 } //Level 16
    };

    //Türrahmen - das Ganze hier geht sicher auch noch besser...?
    if (doorsize[wall_id] == 1 && wall_id == 22) {
        renderWall(1, 17, current_imageID);
        mediaObject->fillRect(58, 20, 61, 58, 255, 255, 255, current_imageID);
        //Knöpfe
        if (level_doortype[level_id][maz_id] == 2
            || level_doortype[level_id][maz_id] == 5)
            mediaObject->copyImage(281, current_imageID, 128, 179, 120, 43, 8, 10);
    }
    if (doorsize[wall_id] == 2 && wall_id == 16) {
        renderWall(1, 9, current_imageID);
        mediaObject->fillRect(71, 26, 34, 37, 255, 255, 255, current_imageID);
        if (level_doortype[level_id][maz_id] == 2
            || level_doortype[level_id][maz_id] == 5)
            mediaObject->copyImage(281, current_imageID, 138, 179, 107, 43, 5, 6);
    }

    //wenn diese Tür offen ist jetzt abbrechen
    if (cell_info[current_pos].door_is_open && !do_door_anim)
        return -2;

    //printf("%d\n", maz_id);

    //Tür Animation
    if (!do_door_anim) {
        large_anim_offset_y = 0;
        middle_anim_offset_y = 0;
        small_anim_offset_y = 0;
        door_timer = 0;
    } else {
        if (door_timer < mediaObject->getMilliSeconds()) {
            if (do_door_open) {
                door_timer += 50;
                large_anim_offset_y += 2;
                middle_anim_offset_y += 1.2;
                small_anim_offset_y += 0.6;
                if (large_anim_offset_y >= 76)
                    do_door_anim = false;
            } else {
                door_timer += 50;
                large_anim_offset_y -= 2;
                middle_anim_offset_y -= 1.2;
                small_anim_offset_y -= 0.6;
                if (large_anim_offset_y <= 0)
                    do_door_anim = false;
            }
        }
    }

    //Türen
    switch (level_doortype[level_id][maz_id]) {
    //Darkmoon Tür ohne Knopf
    case 1:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 93 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 75, 153 + middle_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 50, 44 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 160, 46 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 72, 70 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    //Darkmoon Tür mit Knopf
    case 2:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 93 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 75, 153 + middle_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 50, 44 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 160, 46 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 72, 70 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    //Ausgang zum Wald
    case 3:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 122 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 236, 2, x_offset[wall_id], y_offset[wall_id] + middle_anim_offset_y, 48, 38 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 164, 119 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 64, 59 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    //Dungeon Tür ohne Knopf
    case 4:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 0 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 75, 1, x_offset[wall_id], y_offset[wall_id] + middle_anim_offset_y, 50, 44 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 0, 0 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 72, 70 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    //Dungeon Tür mit Knopf
    case 5:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 0 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 75, 1, x_offset[wall_id], y_offset[wall_id] + middle_anim_offset_y, 50, 44 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 0, 0 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 72, 70 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    //Zellen Tür ohne Knopf
    case 6:
        if (doorsize[wall_id] == 3)
            mediaObject->copyImage(281, current_imageID, 128, 30 + small_anim_offset_y, x_offset[wall_id], y_offset[wall_id] - small_anim_offset_y, 32, 28);
        if (doorsize[wall_id] == 2)
            mediaObject->copyScaledImage(281, current_imageID, 75, 48, x_offset[wall_id], y_offset[wall_id] + middle_anim_offset_y, 50, 44 - middle_anim_offset_y, 34, 37 - middle_anim_offset_y);
        if (doorsize[wall_id] == 1)
            mediaObject->copyScaledImage(281, current_imageID, 0, 71 + large_anim_offset_y, x_offset[wall_id], y_offset[wall_id], 72, 70 - large_anim_offset_y, 61, 58 - large_anim_offset_y);
        break;

    default:
        break;
    }

    return -2;
}
