#pragma once

#include <string>

class Config {
public:
    // Konfigurationsfile einlesen
    void openConf(std::string fileName);

    // Werte zuweisen
    void setValue(short key, short val);

    // Sprache
    short mLanguage {};

    // Fullscreen
    bool mFullscreen {};

    // Aufl�sung
    short mResolution {};

    // Starting map
    short mStartMapID;
};
