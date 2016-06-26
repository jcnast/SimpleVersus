#ifndef TEXTURE_H
#define TEXTURE_H

// include game objects

// include necessary libraries
#include <string>
#include <SDL2/SDL.h>

class Texture{
	int width; // width of texture
	int height; // height of texture
	//default colors
	Uint8 defRed;
	Uint8 defGreen;
	Uint8 defBlue;
	Uint8 defAlpha;

	// flash info
	bool flashing;
	bool flashColours;
	float flashStart;
	float flashLength;
	float lastFlash;
	float flashInterval;
	// flash colours
	Uint8 flashRed;
	Uint8 flashGreen;
	Uint8 flashBlue;

	// fade info
	bool fading;
	float fadeStart;
	float fadeLength;
	// current fade alpha
	Uint8 fadeAlpha;

	// faded info
	bool faded;
	
	SDL_Renderer *renderer; // actual SDL renderer
	SDL_Texture *texture; // actual SDL texture

public:
	Texture(int width = 0, int height = 0); // constructor
	~Texture(); // destructor

	bool SetImage(std::string path); // set the image of the texture
	void FreeTexture(); // deallocates the SDL texture
	void ColourChange(Uint8 red, Uint8 green, Uint8 blue); // change the texture color
	void AlphaChange(Uint8 alpha); // change the alpha of the texture
	void Render(int x, int y, bool flip = false); // render texture at given coordinates

	void StartFlash(Uint8 red, Uint8 green, Uint8 blue, float length, float interval); // start texture flash
	void StartFade(float length); // start texture fade
	void ApplyFade(bool fade); // darken texture

	int GetWidth(); // get width of image
	int GetHeight(); // get height of image

	bool SetRenderer(SDL_Renderer *newRenderer); // set the texture's renderer

private:
	void Flash(); // do the flash
	void Fade(); // apply the fade
};

#endif