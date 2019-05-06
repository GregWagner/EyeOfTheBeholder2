#pragma once

class CConfig {
public:
    //Konfigurationsfile einlesen
    void openConf(char* fileName);

    //Werte zuweisen
    void setValue(short key, short val);

    //Sprache
    short language;

    //Fullscreen
    bool fullscreen;

    //Auflösung
    short resolution;

    //Start Map
    short startMapID;
};
