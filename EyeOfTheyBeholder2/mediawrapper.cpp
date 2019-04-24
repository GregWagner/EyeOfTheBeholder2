
/**********************************
Class MEDIAWrapper
***********************************/

#include <SDL.h>
#include <SDL_mixer.h>
#include <SFont.h>
#include <SFont.c>

//
// class initialisation
//

class MEDIAWrapper 
{
public:
	//neues Fenster erzueugen
	void setupWindow(int posX, int posY, int width, int height, bool fullscreen);

	//leeres Bild erstellen
	void createImage(int imageID, int paletteSourceImageID, int width, int height);

	//Bild laden
	void loadImage(int imageID, char *path);

	//Surface als BMP File abspeichern
	void saveBMP(int imageID);

	//Bild zu Bild blitten
	void copyImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int transpR, int transpG, int transpB);

	//Bild zu Bild blitten mit Größenänderung
	bool copyScaledImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int toWidth, int toHeight, int transpR, int transpG, int transpB, bool mirror);
 
	//Original CPS Bild laden
	void loadCPS(int imageID, char *filename, char *palette, int posX, int posY, int width, int height, bool transp, bool sprite);

	//WORD Wert aus CPS File auslesen
	int getWord(unsigned char string[64000], int pos);

	//Format80 Decoder
	int format80decode(unsigned char image_in[], unsigned char image_out[]);

	//Sound laden
	void loadSound(int nr, char path[128]);

	//Sound freigeben
	void freeSound(int nr);

	//Sound abspielen
	void playSound(int nr);

	//Sound anhalten
	void stopSound(int nr);

	//Sound spielt?
	bool isPlaying(int nr);

	//Bild aus Speicher löschen
	void freeImage(int nr);

	//prüfen ob Bild scjon geladen wurde
	bool imageLoaded(int nr);

	//Bild zeichnen
	void drawImage(int imageID, int posX, int posY, int fromPosX = 0, int fromPosY = 0, int width = 0, int height = 0);

	//Text zeichnen
	void drawText(int fontID, int posX, int posY, int r, int g, int b, char* text, bool center = false);

	//Bildschirm Refresh
	void refresh();

	//"echten" screen zeichnen
	void updateVideo();

	//Viereck zeichnen
	void fillRect(int posX, int posY, int width, int height, int r, int g, int b, int imageID);

	//Palette laden für spätere Pixel Aktionen
	void loadPal(char* filename);

	//Pixel zeichnen mit aktueller Palette
	void drawPixel(int posX, int posY, int palOffset, int imageID);

	//Gammewert setzen
	void setGamma(int r, int g, int b);

	//Maus Koordinaten übergeben
	void getMouseState(int *x, int *y);

	//Maustaste links
	bool getMouseLeft();

	//Maustaste rechts
	bool getMouseRight();

	//Tastenevents
	void updateKeys();

	//SDL beenden
	void quit();

	//bestimmte Zeit warten
	void sleep(int ms);

	//Millisekunden auslesen
	int getMilliSeconds();

	//SDL screen
	SDL_Surface *screen_game;
	SDL_Surface *screen;

	//zu SDL Farben konvertierte 6Bit EOB-Palette
	SDL_Color current_colors[256];

	//Auflösung
	int screenWidth;
	int screenHeight;

	//Fonts
	SFont_Font* font[10]; 

	//Grafikspeicher
	SDL_Surface *images[512];
	int imageCounter;

	//Sounds
	Mix_Chunk *sound[512];
	int soundCounter;

	//Tasten
	bool keyEsc, keyLeft, keyRight, keyUp, keyDown;

};


//
// Fenster erzeugen
//

void MEDIAWrapper::setupWindow(int posX, int posY, int width, int height, bool fullscreen)
{
	//SDL screen_game erzeugen
	screen_game = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 32, 0,0,0,0);

	//SDL screen erzeugen
	this->screenWidth = width;
	this->screenHeight = height;

	if(fullscreen)
		screen = SDL_SetVideoMode(width, height, 16, SDL_FULLSCREEN); //z.B. SDL_FULLSCREEN
	else
		screen = SDL_SetVideoMode(width, height, 16, 0); //z.B. SDL_FULLSCREEN
    SDL_WM_SetCaption("Spectalum","");

	font[0] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 0, 0, 0); 
	font[1] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 0, 0, 255); 
	font[2] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 254, 254, 254); 
	font[3] = SFont_InitFont(SDL_LoadBMP("data/font1.bmp"), 116, 232, 252); 
	font[4] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 254, 254, 254);
	font[5] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 252, 84, 84);
	font[6] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 84, 252, 252);
	font[7] = SFont_InitBigFont(SDL_LoadBMP("data/font2.bmp"), 0, 0, 0);

	//Mauszeiger ausschalten
	SDL_ShowCursor(SDL_DISABLE);

	//Audio Init
	Mix_OpenAudio(24000, AUDIO_S16SYS, 2, 1024);

	//Key Init
	this->keyEsc = false;
	this->keyDown = false;
	this->keyLeft = false;
	this->keyRight = false;
	this->keyUp = false;

	//Surfaces initialisieren
	for(int s=0;s<512;s++)
		images[s] = NULL;
}


//
// Bild laden
//

void MEDIAWrapper::loadImage(int imageID, char *path)
{
	char realpath[128];

	this->freeImage(imageID);

	sprintf(realpath, "%s.bmp", path);
	images[imageID] = SDL_LoadBMP(realpath);
}

//
// Surface als BMP File abspeichern
//

void MEDIAWrapper::saveBMP(int imageID)
{
	if(images[imageID] != NULL)
		SDL_SaveBMP(images[imageID], "out.bmp");
	else
		printf("error saving image: %d\n", imageID);
}

//
// Bild aus Speicher löschen
//

void MEDIAWrapper::freeImage(int i)
{
	if(images[i] != NULL)
	{
		SDL_FreeSurface(images[i]);
	}
	
	images[i] = NULL;
	delete(images[i]);
	
}

//
// prüfen ob Bild bereits erstellt wurde
//

bool MEDIAWrapper::imageLoaded(int nr)
{
	if(images[nr] != NULL)
		return true;
	return false;
}

//
// CPS Bild laden
//

void MEDIAWrapper::loadCPS(int imageID, char *filename, char *palette, int posX=0, int posY=0, int width=320, int height=200, bool transp=false, bool sprite=false)
{
	FILE *palfile, *source;
	unsigned char pal[768];
	unsigned char src[64000];
	unsigned char srcdata[64000];
	unsigned char dest[64000];
	long pos = 0;
	int filesize;

	this->freeImage(imageID);

	//
	// Palette laden
	//

	palfile = NULL;
	palfile = fopen(palette, "rb");
	if(palfile == NULL)
	{
		printf("palette not found: %s\n", palette);
		this->sleep(2000);
		this->quit();
	}
	fseek( palfile, 0, SEEK_END );
	filesize = ftell(palfile);
	fseek( palfile, 0, SEEK_SET );
	if(filesize != 768)
	{
		printf("no valid PAL file: %s\n", palette);
		this->sleep(2000);
		this->quit();
	}
	fread(pal, 1, 768, palfile);
	fclose(palfile);

	//
	// CPS File laden
	//

	source = NULL;
	source = fopen(filename, "rb");
	if(source == NULL)
	{
		printf("file not found: %s\n", filename);
		sleep(2000);
		this->quit();
	}

	fseek( source, 0, SEEK_END );
	filesize = ftell(source);
	fseek( source, 0, SEEK_SET );

	fread(src, 1, filesize, source);
	fclose(source);

	//Dateigröße
	filesize = getWord(src, 0) + 2;

	//Komprimierungsmethode
	if(src[2] != 0x04)
		printf("no valid CPS file: %s\n", filename);

	//Bilddaten beginnen bei Position 10
	for(int i=10; i<64000; i++)
		srcdata[i-10] = src[i];

	//
	// SDL Surface erstellen
	//

	SDL_Color colors[256];
	int colorcount = 0;
	for(int i=0;i<768;i+=3)
	{
		//6 Bit Palette der .PAL Files auf 8 Bit Palette portieren und schwarz in weiss umwandeln für spätere Transparenz
		colors[colorcount].r=pal[i]<<2;
		colors[colorcount].g=pal[i+1]<<2;
		colors[colorcount].b=pal[i+2]<<2;
		if(!sprite && transp && colors[colorcount].r == 0 && colors[colorcount].g == 0 && colors[colorcount].b == 0)
		{
			colors[colorcount].r = 255;
			colors[colorcount].g = 255;
			colors[colorcount].b = 255;
		}
		colorcount++;
	}
	colors[255].r = 1;
	colors[255].g = 1;
	colors[255].b = 1;

	SDL_Surface *tempImage = SDL_CreateRGBSurface(SDL_SWSURFACE, 320, 200, 8, 0,0,0,0);
	SDL_SetPalette(tempImage, SDL_LOGPAL|SDL_PHYSPAL, colors, 0, 256);
	SDL_Rect dstrect;

	//CPS Daten decodieren
	format80decode(srcdata, dest);
	
	//Transparent Pixel setzen
	int count = 0;
	for(int h=0; h<200; h++)
	{
		for(int w=0; w<320; w++)
		{
			dstrect.h = 1; dstrect.w = 1; dstrect.x = w; dstrect.y = h;
			if(sprite && colors[dest[count]].r == 0 && colors[dest[count]].g == 0 && colors[dest[count]].b == 0)
			{
				bool rightfree = true, leftfree = true, topfree = true, bottomfree = true;
				
				for(int x=1; x<=8; x++)
				{
					if(count - x >= 0 && count - x <= 63999)
						if(colors[dest[count - x]].r != 0 || colors[dest[count - x]].g != 0 || colors[dest[count - x]].b != 0)
							leftfree = false;
					if(count + x >= 0 && count + x <= 63999)
						if(colors[dest[count + x]].r != 0 || colors[dest[count + x]].g != 0 || colors[dest[count + x]].b != 0)
							rightfree = false;
				}
				for(int x=1; x<=32; x++)
				{
					if(count - (320*x) >= 0 && count - (320*x) <= 63999)
						if(colors[dest[count - (320*x)]].r != 0 || colors[dest[count - (320*x)]].g != 0 || colors[dest[count - (320*x)]].b != 0)
							topfree = false;
					if(count + (320*x) >= 0 && count + (320*x) <= 63999)
						if(colors[dest[count + (320*x)]].r != 0 || colors[dest[count + (320*x)]].g != 0 || colors[dest[count + (320*x)]].b != 0)
							bottomfree = false;
				}
				
				if(!rightfree && !leftfree && !topfree && !bottomfree)
					SDL_FillRect(tempImage, &dstrect, 255);
				else
					SDL_FillRect(tempImage, &dstrect, dest[count]);
			}
			else
				SDL_FillRect(tempImage, &dstrect, dest[count]);
			count++;
		}
	}

	images[imageID] = SDL_CreateRGBSurface(SDL_SWSURFACE, width,height, 32,0,0,0,0);
	SDL_Rect rcSrc = { posX,posY, width,height };
	SDL_Rect rcDst = { 0,0, width,height };;
	SDL_BlitSurface(tempImage, &rcSrc, images[imageID], &rcDst );
	SDL_FreeSurface(tempImage);

	
}
	
//WORD Wert aus CPS File auslesen
int MEDIAWrapper::getWord(unsigned char string[64000], int pos)
{
	return (string[pos+1] << 8) + string[pos];
}

int MEDIAWrapper::format80decode(unsigned char image_in[], unsigned char image_out[])
{
	unsigned char* copyp;
	unsigned char* readp = image_in;
	unsigned char* writep = image_out;
	unsigned short code;
	unsigned short count;

	while (1) {
		code = *readp++;
		if (~code & 0x80) {
			//bit 7 = 0
			//command 0 (0cccpppp p): copy
			count = (code >> 4) + 3;
			copyp = writep - (((code & 0xf) << 8) + *readp++);
			while (count--)
				*writep++ = *copyp++;
		} else {
			//bit 7 = 1
			count = code & 0x3f;
			if (~code & 0x40) {
				//bit 6 = 0
				if (!count)
					//end of image
					break;
				//command 1 (10cccccc): copy
				while (count--)
					*writep++ = *readp++;
			} else {
				//bit 6 = 1
				if (count < 0x3e) {
					//command 2 (11cccccc p p): copy
					count += 3;

					copyp = (unsigned char*)&image_out[*(unsigned short*)readp];
					readp += 2;

					while (count--)
						*writep++ = *copyp++;
				} else if (count == 0x3e) {
					//command 3 (11111110 c c v): fill

					count = *(unsigned short*)readp;
					readp += 2;
					code = *readp++;
					memset(writep, code, count);
					writep += count;
				} else {
					//command 4 (copy 11111111 c c p p): copy

					count = *(unsigned short*)readp;
					readp += 2;

					copyp = (unsigned char*)&image_out[*(unsigned short*)readp];
					readp += 2;
					while (count--)
						*writep++ = *copyp++;
				}
			}
		}
	}

	return (writep - image_out);
}



//
//Bild zeichnen
//

void MEDIAWrapper::drawImage(int imageID, int posX, int posY, int fromPosX, int fromPosY, int width, int height)
{
	SDL_Rect rcDest = { posX, posY, 0, 0 };
	SDL_Rect rcSrc  = {fromPosX, fromPosY, width, height};
	if(width > 0 && height > 0)
	{
		if(imageID >= 0 && images[imageID])
		{
			//weiss transparent setzen
			SDL_SetColorKey(images[imageID], SDL_SRCCOLORKEY, SDL_MapRGB(images[imageID]->format, 255, 255, 255));
			SDL_BlitSurface ( images[imageID], &rcSrc, screen_game, &rcDest );
		}
	}
	else
	{
		if(imageID >= 0 && images[imageID])
		{
			//weiss transparent setzen
			SDL_SetColorKey(images[imageID], SDL_SRCCOLORKEY, SDL_MapRGB(images[imageID]->format, 255, 255, 255));
			SDL_BlitSurface ( images[imageID], NULL, screen_game, &rcDest );
		}
	}
}

//
// leeres Bild erstellen
//
void MEDIAWrapper::createImage(int imageID, int paletteSourceImageID, int width, int height)
{
	if(images[imageID] != NULL)
		images[imageID] = NULL;
	
	images[imageID] = SDL_CreateRGBSurface(SDL_SWSURFACE, width, height, 32, 0, 0, 0, 0);
}

//
// Bild zu Bild blitten
//

void MEDIAWrapper::copyImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int transpR=-1, int transpG=-1, int transpB=-1)
{
	SDL_Rect rcSrc = { fromPosX,fromPosY, width,height };
	SDL_Rect rcDst = { toPosX, toPosY, 0, 0 };
	if(transpR != -1 && transpG != -1 && transpB != -1)
		SDL_SetColorKey(images[imageFromID], SDL_SRCCOLORKEY, SDL_MapRGB(images[imageFromID]->format, transpR, transpG, transpB));
	SDL_BlitSurface(images[imageFromID], &rcSrc, images[imageToID], &rcDst );
	//SDL_SaveBMP(images[imageToID], "data/out.bmp");
}

//
// Bild zu Bild blitten mit Größenänderung
//

bool MEDIAWrapper::copyScaledImage(int imageFromID, int imageToID, int fromPosX, int fromPosY, int toPosX, int toPosY, int width, int height, int toWidth, int toHeight, int transpR=-1, int transpG=-1, int transpB=-1, bool mirror=false)
{
	if(toWidth != width || toHeight != height || mirror)
	{
		SDL_Rect from = { 0,0,0,0 };
		SDL_Rect to   = { 0,0,0,0 };
		float xfactor = 0; 
		float yfactor = 0;
		float temp = 0;
		int tempcount = 0;

		xfactor = (float)((float)(toWidth - 0) / (float)(width - 0));
		yfactor = (float)((float)(toHeight - 0) / (float)(height - 0));

		for(float y=fromPosY; y<fromPosY+height; y++)
		{
			tempcount = 0;
			if(!mirror)
			{
				for(float x=fromPosX; x<fromPosX+width; x++)
				{
					from.x = x;
					from.y = y;
					from.w = 1;
					from.h = 1;	

					for(int yf=0; yf<yfactor; yf++)
						for(int xf=0; xf<xfactor; xf++)
						{
							to.x = toPosX + (x-fromPosX)*xfactor + xf;
							to.y = toPosY + (y-fromPosY)*yfactor - yf;
							to.w = 1;
							to.h = 1;
							SDL_BlitSurface(images[imageFromID], &from, images[imageToID], &to );
						}
				}
			}
			else
			{
				for(float x=fromPosX; x<fromPosX+width; x++)
				{
					from.x = fromPosX+width-tempcount++;
					from.y = y;
					from.w = 1;
					from.h = 1;	

					for(int yf=0; yf<yfactor; yf++)
						for(int xf=0; xf<xfactor; xf++)
						{
							to.x = toPosX + (x-fromPosX)*xfactor + xf;
							to.y = toPosY + (y-fromPosY)*yfactor - yf;
							to.w = 1;
							to.h = 1;
							SDL_BlitSurface(images[imageFromID], &from, images[imageToID], &to );
						}
				}
			}
		}	
	}
	else
	{
		SDL_Rect rcSrc = { fromPosX,fromPosY, width,height };
		SDL_Rect rcDst = { 0, 0, 0, 0 };
		SDL_BlitSurface(images[imageFromID], &rcSrc, images[imageToID], &rcDst );
	}

	return true;
}

typedef struct tColorRGBA {
	Uint8 r;
	Uint8 g;
	Uint8 b;
	Uint8 a;
} tColorRGBA;


 

//
// Text zeichen
//

void MEDIAWrapper::drawText(int fontID, int posX, int posY, int r, int g, int b, char* text, bool center)
{
	if(fontID < 4)
	{
		posY += 1; 
		if(center)
			posX = (this->screen_game->w / 2) - (SFont_TextWidth(font[0],text) / 2);
		if(r==0 && g==0 && b==0)
			SFont_Write (screen_game, font[0], posX, posY,text); 
		if(r==0 && g==0 && b==255)
			SFont_Write (screen_game, font[1], posX, posY,text); 
		if(r==255 && g==255 && b==255)
			SFont_Write (screen_game, font[2], posX, posY,text); 
		if(r==116 && g==232 && b==252)
			SFont_Write (screen_game, font[3], posX, posY,text); 
	}
	else
	{
		//schwarzen Schatten zeichnen
		SFont_BigWrite (screen_game, font[7], posX-1, posY+1,text);

		//Font zeichnen
		SFont_BigWrite (screen_game, font[fontID], posX, posY,text); 
	}
}

//
// SDL beenden
//

void MEDIAWrapper::quit()
{
	SDL_Quit();
	exit(0);
}

//
// x ms warten
//

void MEDIAWrapper::sleep(int ms)
{
	SDL_Delay(ms);
}

//
// Sound laden
//

void MEDIAWrapper::loadSound(int nr, char *path)
{
	char realpath[128];
	sprintf(realpath, "%s.ogg", path);
	sound[nr] = Mix_LoadWAV(realpath);
}

//
// Sound löschen
//

void MEDIAWrapper::freeSound(int nr)
{
	if(sound[nr] != NULL)
	{
		Mix_FreeChunk(sound[nr]);
		sound[nr] = NULL;
	}
}

//
// Sound abspielen
//

void MEDIAWrapper::playSound(int nr)
{
	Mix_PlayChannel(-1, sound[nr], 0);
}

//
// Sound stoppen
//

void MEDIAWrapper::stopSound(int nr)
{
	if(sound[nr] != NULL)
		Mix_HaltChannel(nr);
}

//
// Sound spielt
//

bool MEDIAWrapper::isPlaying(int nr)
{
	if(sound[nr] != NULL)
		return true;
	else
		return false;
}

//
// Screen Refresh
//

void MEDIAWrapper::refresh()
{
	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

//
// Maus Koordinaten zurückgeben
//

void MEDIAWrapper::getMouseState(int *x, int *y)
{
	SDL_PumpEvents();
	SDL_GetMouseState(x, y);
}

//
// prüfen ob linke Maustaste gedrückt wurde
//

bool MEDIAWrapper::getMouseLeft()
{
	SDL_PumpEvents();
	if(SDL_GetMouseState(0,0)&SDL_BUTTON(1))
		return true;
	else
		return false;
}

//
// prüfen ob rechte Maustaste gedrückt wurde
//

bool MEDIAWrapper::getMouseRight()
{
	SDL_PumpEvents();
	if(SDL_GetMouseState(0,0)&SDL_BUTTON(3))
		return true;
	else
		return false;
}

//
// Taste down
//

void MEDIAWrapper::updateKeys()
{
	SDL_Event event;
    while(SDL_PollEvent(&event)) 
	{
		switch(event.type) 
		{
			// 
			// Tastatur Events
			//

			case SDL_KEYDOWN:
				switch(event.key.keysym.sym) 
				{
					case SDLK_ESCAPE:
						this->keyEsc = true;
						break;
					case SDLK_DOWN:
						this->keyDown = true;
						break;
					case SDLK_UP:
						this->keyUp = true;
						break;
					case SDLK_LEFT:
						this->keyLeft = true;
						break;
					case SDLK_RIGHT:
						this->keyRight = true;
						break;
					default:
						break;
				}
				break;
			case SDL_KEYUP:
				this->keyDown = this->keyEsc = this->keyLeft = this->keyRight = this->keyUp = false;
				break;
        }
    }
}

//
// gefülltest Viereck zeichnen
//

void MEDIAWrapper::fillRect(int posX, int posY, int width, int height, int r, int g, int b, int imageID = -1)
{
	SDL_Rect dstrect = {posX, posY, width, height};
	SDL_FillRect(imageID==-1?screen_game:images[imageID], &dstrect, SDL_MapRGB(imageID==-1?screen_game->format:images[imageID]->format, r, g, b));
}


//
// Palette laden für spätere Pixel Aktionen
//
void MEDIAWrapper::loadPal(char* filename)
{
	FILE *palfile;
	unsigned char pal[768];
	int filesize;

	//
	// Palette laden
	//

	palfile = NULL;
	palfile = fopen(filename, "rb");
	if(palfile == NULL)
	{
		printf("palette not found: %s\n", filename);
		this->sleep(2000);
		this->quit();
	}
	fseek( palfile, 0, SEEK_END );
	filesize = ftell(palfile);
	fseek( palfile, 0, SEEK_SET );
	if(filesize != 768)
	{
		printf("no valid PAL file: %s\n", filename);
		this->sleep(2000);
		this->quit();
	}
	fread(pal, 1, 768, palfile);
	fclose(palfile);

	//
	// SDL Surface erstellen
	//

	int colorcount = 0;
	for(int i=0;i<768;i+=3)
	{
		//6 Bit Palette der .PAL Files auf 8 Bit Palette portieren
		current_colors[colorcount].r=pal[i]<<2;
		current_colors[colorcount].g=pal[i+1]<<2;
		current_colors[colorcount].b=pal[i+2]<<2;
		colorcount++;
	}
}

//
// Pixel zeichnen mit aktueller Palette
//

void MEDIAWrapper::drawPixel(int posX, int posY, int palOffset, int imageID = -1)
{
SDL_Rect dstrect = {posX, posY, 2, 1};
	SDL_FillRect(imageID==-1?screen_game:images[imageID], &dstrect, SDL_MapRGB(imageID==-1?screen_game->format:images[imageID]->format, current_colors[palOffset].r, current_colors[palOffset].g, current_colors[palOffset].b));
}

//
// Gamescreen in echten screen surface kopieren und alles anzeigen
//

void MEDIAWrapper::updateVideo()
{
	// Skalierung nur nötig wenn Auflösung != screen_game
	if(this->screenWidth > 320 && this->screenHeight > 200)
	{
		SDL_Rect from = { 0,0,0,0 };
		SDL_Rect to   = { 0,0,0,0 };
		float xfactor = 0; 
		float yfactor = 0;
		float temp = 0;

		xfactor = (float)((float)(screen->w - 0) / (float)(screen_game->w - 0));
		yfactor = (float)((float)(screen->h - 0) / (float)(screen_game->h - 0));

		for(float y=0; y<screen_game->h; y++)
		{
			for(float x=0; x<screen_game->w; x++)
			{
				from.x = x;
				from.y = y;
				from.w = 1;
				from.h = 1;	

				//screen_game entsprechend der eingestellten Auflösung rendern
				for(int yf=0; yf<yfactor; yf++)
					for(int xf=0; xf<xfactor; xf++)
					{
						to.x = 0 + x*xfactor + xf;
						to.y = 0 + y*yfactor - yf;
						to.w = 1;
						to.h = 1;
						SDL_BlitSurface(screen_game, &from, screen, &to );
					}
			}
		}	
	}
	else
	{
		SDL_Rect rcSrc = { 0,0, screen_game->w,screen_game->h };
		SDL_Rect rcDst = { 0, 0, 0, 0 };
		SDL_BlitSurface(screen_game, &rcSrc, screen, &rcDst );
	}

	SDL_UpdateRect(screen, 0, 0, screen->w, screen->h);
}

//
//Gammewert setzen
//

void MEDIAWrapper::setGamma(int r, int g, int b)
{
	SDL_SetGamma(r, g, b);
}

//
//Millisekunden auslesen
//

int MEDIAWrapper::getMilliSeconds()
{
	return SDL_GetTicks();
}

