#ifndef PARTICLESYSTEM_H
#define PARTICLESYSTEM_H

// include game objects

// include necessary libraries
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

class Particle;
class GameMaster;

class ParticleSystem{
	// game master
	GameMaster *game;

	// all particles in system
	std::vector<Particle *> particles;
	std::vector<Particle *> delParticles;
	int numParticles;

	// particle color info
	Uint8 maxRed, minRed;
	Uint8 maxGreen, minGreen;
	Uint8 maxBlue, minBlue;

	// particle system info
	float maxDuration, minDuration; // system duration

	float maxRadius, minRadius; // particle size range

	float startXPos, startYPos; // origin of particles

	float maxXVelo, minXVelo; // x velocity ranges

	float maxYVelo, minYVelo; // y velocity ranges

	float gravityPortion; // how impact the particle are by gravity

public:
	// construct the particle system
	ParticleSystem(Uint8 maxRed, Uint8 minRed, Uint8 maxGreen, Uint8 minGreen, Uint8 maxBlue, Uint8 minBlue, float maxDuration, float minDuration, float startXPos, float startYPos, float maxXVelo, float minXVelo, float maxYVelo, float minYVelo, float maxRadius, float minsRadius, bool fades, int numParticles = 10, float gravityPortion = 0.05);
	// destruct the particle system
	~ParticleSystem();

	void RemoveParticle(Particle *delParticle); // remove particle from system
	void QueueDelete(Particle *delParticle); // queue particle for deletion

	void PhysicsUpdate(float deltaTime); // update the particles within the system
	void Render(); // render all particles in the system
	void Flush(); // remove the particles queued for deletion

private:
};

#endif