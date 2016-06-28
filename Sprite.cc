// include game objects
#include "Texture.h"
#include "Sprite.h"

// include necessary libraries
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

// debug
// #include <iostream>

// create sprite
Sprite::Sprite(int numClips, int width, int height):
	Texture(width, height), numClips(numClips){

	// nothing else to create
}

// delete sprite
Sprite::~Sprite(){
	// nothing to be deleted
}

// render a clip
void Sprite::Render(int x, int y, int clip, bool flip){
	// render rect
	SDL_Rect renderQuad = {x, y, width/numClips, height};
	// quad of frame to be rendered
	SDL_Rect frameQuad = {clip*width/numClips, 0, width/numClips, height};

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
		SDL_RenderCopyEx(renderer, texture, &frameQuad, &renderQuad, 0, NULL, SDL_FLIP_HORIZONTAL);
	}else{
		SDL_RenderCopyEx(renderer, texture, &frameQuad, &renderQuad, 0, NULL, SDL_FLIP_NONE);
	}
}