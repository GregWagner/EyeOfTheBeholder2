#pragma warning(disable : 4996)

#include "language.h"
#include <cstdio>
#include <string>

void CLanguage::init(short language = 0)
{
    // 0 = englisch, 1 = deutsch
    this->language = language;

    // Sprachfile öffnen
    FILE* file;
    std::string fileName;
    if (language == 0)
        fileName = "data/english.lang";
    if (language == 1)
        fileName = "data/german.lang";

    file = fopen(fileName.c_str(), "r");

    //Inhalt Zeilenweise einlesen
    char data[512];
    short arrayPointer = 0;
    while (!feof(file)) {
        fgets(data, 512, file);
        if (strlen(data) > 1) {
            data[strlen(data) - 1] = '\0';
            text[arrayPointer++] = strdup(data);
        }
        data[0] = '\0';
    }
    fclose(file);
}
