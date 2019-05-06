#pragma once

class CMazeObjects {
public:
    void init(short maze_id);

    //MAZ File laden
    void loadMAZ(char* file);

    //INF File laden
    void loadINF(char* file);

    //VCN File laden
    void loadVCN(char* file);

    // Bytes in einem WORD swappen
    unsigned short getSwappedWord(unsigned char byte_0, unsigned char byte_1);

    //VMP File laden
    void loadVMP(char* file);

    //8x8 Pixel Block zeichnen mit einer der zwei Paletten
    void drawBlock(short block_nr, short posX, short posY, bool wall_palette, bool x_flipp, int imageID);

    //Hintergrund zeichnen - wenn n�tig x-flipped
    void loadWallBackground(bool flipped, int imageID);

    //Wandfront zeichnen
    void renderWalls(int level, int playerPos, int playerFace);

    //T�ren zeichnen
    int renderDoor(int maz_id, int wall_id);

    //Wand Dekoration rendern
    int computeDecoration(int maz_id);

    //einzelne Wand zeichnen
    bool renderWall(int wallID, int viewportPos, int imageID);

    //MAZ Id in WMI konvertieren
    signed char getRealWallID(int maz_id, int wall_id = -1, int maz_pos = 0);

    //Media Wrapper Objekt
    MEDIAWrapper* mediaObject;

    //Level ID
    int level_id;

    //MAZ Daten
    unsigned char maz_src[4102];

    //INF Daten
    unsigned char inf_src[64000];

    //VCN Daten
    unsigned char vcn_src[64000];

    //Anzahl der Bl�cke im VCN File
    unsigned short vcn_count_blocks;

    //16 Farben aus der aktuellen 256-Farb-Palette f�r Hinterg�nde
    unsigned char vcn_bg_palette[16];

    //16 Farben aus der aktuellen 256-Farb-Palette f�r W�nde
    unsigned char vcn_wall_palette[16];

    //Tiles f�r Wanddaten
    unsigned short tile_data[32][431];

    //VMP Daten
    unsigned char vmp_src[64000];

    //aktuell zum rendern genutzte ImageID
    int current_imageID;

    //aktuell verarbeitete maze-position
    unsigned short current_pos;

    //aktuelle Palette
    char current_pal[16];

    //steuern ob nach Wand noch eine Dekoration gerendert werden soll
    bool do_decoration;
    unsigned char do_decoration_id, do_decoration_wmi, do_decoration_walltype;

    //Event - und sonstige Infos zu den einzelnen Zellen der Karte
    struct cellInformation {
        bool door_is_open; //T�r - offen/geschlossen
        bool is_door; //Zelle enth�lt eine T�r
        unsigned char deco_code; //Decoration ID f�r diese Zelle �berschreibt alle anderen Dekos
        unsigned char wall_code; //Wall ID f�r diese Zelle �berschreibt Wall ID des .maz Files dieser Zelle
        unsigned char empty_space04; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space05; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space06; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space07; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space08; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space09; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space10; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space11; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space12; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space13; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space14; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space15; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space16; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space17; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space18; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space19; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space20; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space21; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space22; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space23; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space24; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space25; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space26; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space27; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space28; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space29; //Platzhalter f�r sp�tere Werte
        unsigned char empty_space30; //Platzhalter f�r sp�tere Werte
    } cell_info[1089];

    //Struktur mit Wall-Daten aus MAZ File - 32 x 32 Bl�cke pro Level
    struct MazeStructure {
        unsigned char n;
        unsigned char s;
        unsigned char w;
        unsigned char e;
    } maze_data[1024];

    //Struktur f�r Wand Dekorationen
    struct WallMapping {
        unsigned char wall_mapping_index;
        unsigned char wall_type;
        unsigned char decoration_id;
    };

    //Dekorations-Infos f�r diesen Level
    int count_deco_info;
    struct Decoration {
        unsigned char rectangleIndices[10];
        unsigned char linkToNextDecoration;
        unsigned char flags;
        unsigned short xCoords[10];
        unsigned short yCoords[10];
    };
    struct DecorationRectangle {
        unsigned short x;
        unsigned short y;
        unsigned short w;
        unsigned short h;
    };
    struct DecorationInformation {
        char info_id;
        char image_name[32];
        int image_id;
        char dec_file_name[32];
        WallMapping decoration_data[256];
        Decoration decorations[256];
        int count_rectangle_data;
        DecorationRectangle rectangle_data[256];
    } deco_info[16];

    //Variablen zur Steuerung der T�r/auf T�r/zu Animationen
    double large_anim_offset_y;
    double middle_anim_offset_y;
    double small_anim_offset_y;
    bool do_door_anim, do_door_open;
    long door_timer;

    //f�r Debugging Zwecke
    int p_shift, p_shift2;
};
