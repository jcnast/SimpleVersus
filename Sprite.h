#ifndef SPRITE_H
#define SPRITE_H

// include game objects
#include "Texture.h"

// include necessary libraries
#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Sprite: public Texture{
	
	int numClips;

public:
	Sprite(int numClips = 1, int width = 0, int height = 0); // constructor
	~Sprite(); // destructor

	void Render(int x, int y, int clip, bool flip = false); // render texture at given coordinates

private:
};

#endif