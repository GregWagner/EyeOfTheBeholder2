#pragma once

#include "language.h"
#include "mediawrapper.h"

class Intro {
public:
    //Init
    void init(MediaWrapper* mediaObject);

    //Playback
    void play();

    //Media Wrapper Objekt
    MediaWrapper* mediaObject;

    //Instanz der Sprachdaten
    Language languageData;

    //Struktur f�r Animationssequenzen
    struct animation {
        char fileName[128];
        short frames;
        short repeat;
        short keyFrames[32];
    };

    //Strukur f�r die Sprachdaten
    struct lang {
        char* sringData;
        short from;
        short to;
        short row;
    };
    lang mLanguage[24];

    //Instanzen der 32 Animationsstufen
    animation anim[33];
    animation keyframe;

    //Anzahl der Bilder
    short images;
};
