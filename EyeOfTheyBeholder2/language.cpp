#include "language.h"
#include <cstdio>
#include <cstring>

void CLanguage::init(short language = 0)
{
    // 0 = englisch, 1 = deutsch
    this->language = language;

    // Sprachfile öffnen
    FILE* file;
    char* fileName;
    if (language == 0)
        fileName = "data/english.lang";
    if (language == 1)
        fileName = "data/german.lang";

#ifdef WINCE
    char realFileName[128];
    sprintf_s(realFileName, "\\eob2\\%s", fileName);
    file = fopen(realFileName, "r");
#else
    file = fopen(fileName, "r");
#endif

    //Inhalt Zeilenweise einlesen
    char data[512];
    short arrayPointer = 0;
    while (!feof(file)) {
        fgets(data, 512, file);
        if (strlen(data) > 1) {
            data[strlen(data) - 1] = '\0';
#ifdef WINCE
            text[arrayPointer++] = _strdup(data);
#else
            text[arrayPointer++] = strdup(data);
#endif
        }
        data[0] = '\0';
    }
    fclose(file);
}
