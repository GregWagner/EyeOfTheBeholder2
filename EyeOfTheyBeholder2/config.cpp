#pragma warning(disable : 4996)

#include "config.h"

void CConfig::openConf(std::string fileName)
{
    this->fullscreen = false;
    this->language = 0;
    this->resolution = 0;

    //Config File öffnen
    FILE* file;
    char filedata[8192];

    file = fopen(fileName.c_str(), "r");

    if (!file) {
        this->fullscreen = true;
        this->language = 0;
        this->startMapID = 0;
        printf("Configuration file not found!\n");
        exit(0);
    } else {
        //Inhalt in Variable zeilenweise einlesen
        char data[128][128];
        char key[128][128];
        char val[128][128];
        short row = 0;
        short pos_space = 0;
        short pos_linebreak = 0;
        while (!feof(file)) {
            row++;
            fgets(data[row], 128, file);
            pos_space = strchr(data[row], ' ') - data[row];
            if (strlen(data[row]) > 1 && pos_space > 3) {
                strncpy(key[row], data[row], pos_space);

                //Name der Option
                key[row][pos_space] = '\0';
                //Wert der Option
                short i;
                short valpos;
                valpos = 0;
                for (i = pos_space + 3; i < strlen(data[row]) - 1; i++) {
                    val[row][valpos++] = data[row][i];
                }
                val[row][valpos] = '\0';

                //Sprache
                if (strcmp(key[row], "lang") == 0)
                    if (strcmp(val[row], "german") == 0)
                        setValue(1, 1);
                    else
                        setValue(1, 0);
                //Fullscreen
                if (strcmp(key[row], "fullscreen") == 0)
                    if (strcmp(val[row], "yes") == 0)
                        setValue(0, 1);
                    else
                        setValue(0, 0);
                //Auflösung
                if (strcmp(key[row], "resolution") == 0)
                    if (strcmp(val[row], "320x200") == 0)
                        setValue(3, 0);
                    else
                        setValue(3, 1);
                //Start Map;
                if (strcmp(key[row], "startmap") == 0)
                    setValue(2, atoi(val[row]));
            }
            data[row][0] = '\0';
        }
        fclose(file);
    }
}

//
// Werte setzen
//

void CConfig::setValue(short key, short val)
{
    switch (key) {
    //Fullscreen
    case 0:
        if (val == 0) {
            this->fullscreen = false;
        } else if (val == 1) {
            this->fullscreen = true;
        }
        break;
    //Sprache
    case 1:
        if (val == 0) {
            this->language = 0;
        } else if (val == 1) {
            this->language = 1;
        }
        break;
    //Start Map
    case 2:
        this->startMapID = val;
        break;
    //Auflösung
    case 3:
        this->resolution = val;
        break;
    }
}
