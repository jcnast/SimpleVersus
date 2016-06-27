// include game objects
#include "Texture.h"
#include "GameMaster.h"

// include necessary libraries
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>

Texture::Texture(int width, int height): width(width), height(height){
	// set the parameters of the texture
	defRed = 254;
	defGreen = 254;
	defBlue = 254;
	defAlpha = 255;

	flashing = false;
	flashColours = false;
	flashRed = 254;
	flashGreen = 254;
	flashBlue = 254;

	fading = false;
	fadeAlpha = 255;

	faded = false;

	darkened = false;

	// set the SDL elements of the texture
	texture = NULL;
}

Texture::~Texture(){
// delete the contained SDL texture
	FreeTexture();
	renderer = NULL;
}

// set the image of the texture
bool Texture::SetImage(std::string path){
	if(path.empty()){
		std::cout << "Attempted to load from empty path" << std::endl;
		return false;
	}
	// get rid of current image
	FreeTexture();

	// temp surface to load images to
	SDL_Surface *tempSurface = IMG_Load(path.c_str());

	if(tempSurface == NULL){
		std::cout << "Unable to load image" << std::endl;
		return false;
	}else{
		// color key image
		SDL_SetColorKey(tempSurface, SDL_TRUE, SDL_MapRGB(tempSurface->format, 0, 0xFF, 0xFF));
		// create texture from tempSurface pixels
		texture = SDL_CreateTextureFromSurface(renderer, tempSurface);
		if(texture == NULL){
			std::cout << "unable to create texture from image" << std::endl;
			return false;
		}
		// get width and height of image
		if(width == 0 && height == 0){
			width = tempSurface->w;
			height = tempSurface->h;
		}
		// set the blend mode
		SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
		// free the temp surface
		SDL_FreeSurface(tempSurface);
	}

	return (texture != NULL);
}

// free the current texture
void Texture::FreeTexture(){
	if(texture != NULL){
		SDL_DestroyTexture(texture);
		texture = NULL;
		width = 0;
		height = 0;
	}
}

// change the default color of the sprite when rendered
void Texture::ColourChange(Uint8 red, Uint8 green, Uint8 blue){
	defRed = red;
	defGreen = green;
	defBlue = blue; 
}

// change the alpha value of the texture
void Texture::AlphaChange(Uint8 alpha){
	defAlpha = alpha;
}

// render the image
void Texture::Render(int x, int y, bool flip){
	// render rect
	SDL_Rect renderQuad = {x, y, width, height};

	// handle any flashing events
	if(darkened){
		SDL_SetTextureColorMod(texture, defRed/2, defGreen/2, defBlue/2);
	}else if(!flashing){
		SDL_SetTextureColorMod(texture, defRed, defGreen, defBlue);
	}else{
		Flash();
	}

	// handle any fading events
	if(faded){
		SDL_SetTextureAlphaMod(texture, defAlpha/2);
	}else if(!fading){
		SDL_SetTextureAlphaMod(texture, defAlpha);
	}else{
		Fade();		
	}

	// if the texture is flipped, flip the sprite
	if(flip){
		SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
	}else{
		SDL_RenderCopyEx(renderer, texture, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);
	}
}

// start the texture flashing
void Texture::StartFlash(Uint8 red, Uint8 green, Uint8 blue, float length, float interval){
	// set flash info
	flashing = true;
	flashColours = true;
	flashStart = SDL_GetTicks();
	flashLength = length;
	lastFlash = flashStart;
	flashInterval = interval;

	// colours that will be flashed
	flashRed = red;
	flashGreen = green;
	flashBlue = blue;
}

// start texture fading
void Texture::StartFade(float length){
	// set fade info
	fading = true;
	fadeStart = SDL_GetTicks();
	fadeLength = length;

	// set the initial alpha to fade from
	fadeAlpha = defAlpha;
}

void Texture::ApplyFade(bool fade){
	faded = fade;
}

void Texture::Darken(bool dark){
	darkened = dark;
}

// set the renderer
bool Texture::SetRenderer(SDL_Renderer *newRenderer){
	renderer = newRenderer;
}

// the flash animation
void Texture::Flash(){
	float curTime = SDL_GetTicks();
	if(((curTime - flashStart)/1000) >= flashLength){
		flashing = false;
		SDL_SetTextureColorMod(texture, defRed, defGreen, defBlue);
		return;
	}else{
		if(((curTime-lastFlash)/1000) >= flashInterval){
			flashColours = (!flashColours);
			lastFlash = curTime;
		}
		if(flashColours){
			SDL_SetTextureColorMod(texture, flashRed, flashGreen, flashBlue);
		}else{
			SDL_SetTextureColorMod(texture, defRed, defGreen, defBlue);
		}
	}
}

// the fade animation
void Texture::Fade(){
	// the percent of the fade time that has passed
	float fadePortion = 1 - ((SDL_GetTicks() - fadeStart)/1000)/fadeLength;
	// get the resulting alpha value
	if(fadePortion >= 0){
		fadeAlpha = fadePortion * defAlpha;
	}
	
	// apply the result
	SDL_SetTextureAlphaMod(texture, fadeAlpha);
	if(((SDL_GetTicks() - fadeStart)/1000) >= fadeLength){
		fading = false;
	}
}