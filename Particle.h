#ifndef PARTICLE_H
#define PARTICLE_H

// include game objects
#include "Circle.h"

// include necessary libraries
#include <SDL2/SDL.h>

// forward declarations
class Texture;
class ParticleSystem;
class GameMaster;

class Particle: public Circle{
// particle system
	ParticleSystem *pSystem;
	
// particle sprite
	Texture *sprite;

// particle info
	float startTime;
	float duration;
	float xVelo;
	float yVelo;
	float gravityPortion;

public: // public methods
	Particle(ParticleSystem *pSystem, float xPos, float yPos, float xVelo, float yVelo, float radius, float duration, bool fades, float gravityPortion = 0.2); // constructor
	~Particle(); // destructor
	void SetColor(Uint8 red, Uint8 green, Uint8 blue); // set character's color

	void PhysicsUpdate(float deltaTime); // apply physics updates to particle

	virtual void Render(); // render particle sprite

private: // private methods

};

#endif