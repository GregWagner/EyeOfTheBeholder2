#pragma once

class CIntro {
public:
    //Init
    void init(MEDIAWrapper* mediaObject);

    //Playback
    void play();

    //Media Wrapper Objekt
    MEDIAWrapper* mediaObject;

    //Instanz der Sprachdaten
    CLanguage languageData;

    //Struktur für Animationssequenzen
    struct animation {
        char fileName[128];
        short frames;
        short repeat;
        short keyFrames[32];
    };

    //Strukur für die Sprachdaten
    struct lang {
        char* sringData;
        short from;
        short to;
        short row;
    };
    lang language[24];

    //Instanzen der 32 Animationsstufen
    animation anim[33];
    animation keyframe;

    //Anzahl der Bilder
    short images;
};
