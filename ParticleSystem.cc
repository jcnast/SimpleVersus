// include game objects
#include "ParticleSystem.h"
#include "Particle.h"
#include "GameMaster.h"

// include necessary libraries
#include <vector>
#include <algorithm>
#include <cstdlib>
#include <SDL2/SDL_image.h>

// create particle system
ParticleSystem::ParticleSystem(Uint8 maxRed, Uint8 minRed, Uint8 maxGreen, Uint8 minGreen, Uint8 maxBlue, Uint8 minBlue, float maxDuration, float minDuration, float startXPos, float startYPos, float maxXVelo, float minXVelo, float maxYVelo, float minYVelo, float maxRadius, float minRadius, bool fades, int numParticles, float gravityPortion):
	maxRed(maxRed), minRed(minRed), maxGreen(maxGreen), minGreen(minGreen), maxBlue(maxBlue), minBlue(minBlue), maxDuration(maxDuration), minDuration(minDuration), startXPos(startXPos), startYPos(startYPos),	maxXVelo(maxXVelo), minXVelo(minXVelo),
	maxYVelo(maxYVelo), minYVelo(minYVelo), maxRadius(maxRadius), minRadius(minRadius), numParticles(numParticles), gravityPortion(gravityPortion){

	// set the game master
	game = GameMaster::GetInstance();
	// create particles and add them to the system
	for(int i = 0; i < numParticles; i++){
		// current particle's velocity
		float pXVelo = ((maxXVelo - minXVelo)*(rand()%101)/100) + minXVelo;
		float pYVelo = ((maxYVelo - minYVelo)*(rand()%101)/100) + minYVelo;
		// current particle's size
		float pRadius = ((maxRadius - minRadius)*(rand()%101)/100) + minRadius;
		// current particle's duration
		float pDuration = ((maxDuration - minDuration)*(rand()%101)/100) + minDuration;
		// current particle's color
		float pRed = ((maxRed - minRed)*(rand()%101)/100) + minRed;
		float pGreen = ((maxGreen - minGreen)*(rand()%101)/100) + minGreen;
		float pBlue = ((maxBlue - minBlue)*(rand()%101)/100) + minBlue;

		// create new particle
		Particle *newParticle = new Particle(this, startXPos, startYPos, pXVelo, pYVelo, pRadius, pDuration, fades, gravityPortion);
		// set particle's color
		newParticle->SetColor(pRed, pGreen, pBlue);
		// add particle to vector
		particles.push_back(newParticle);
	}
}

// destroy particle system
ParticleSystem::~ParticleSystem(){
	particles.clear();
	// stop the game from keeping track of this particle system
	game->RemoveParticleSystem(this);
}

// remove a particle from the system
void ParticleSystem::RemoveParticle(Particle *delParticle){
	std::vector<Particle *>::iterator pos = std::find(particles.begin(), particles.end(), delParticle);
	if(pos != particles.end()){ // == myVector.end() means the element was not found
    particles.erase(pos);
  }
  // if no more particles are in the system, delete the system
  if(particles.size() == 0){
		game->QueueDelete(this);
	}
}

// add particle to list of those which are to be deleted
void ParticleSystem::QueueDelete(Particle *delParticle){
	delParticles.push_back(delParticle);
}

// update all the particles within the system
void ParticleSystem::PhysicsUpdate(float deltaTime){
	for(std::vector<Particle *>::iterator p = particles.begin(); p != particles.end(); ++p) {
   	(*p)->PhysicsUpdate(deltaTime);
	}
}

// render each particle to the screen
void ParticleSystem::Render(){
	for(std::vector<Particle *>::iterator p = particles.begin(); p != particles.end(); ++p) {
   	(*p)->Render();
	}
}

// remove the particles that have been queued
void ParticleSystem::Flush(){
	for(std::vector<Particle *>::iterator p = delParticles.begin(); p != delParticles.end(); ++p){
		delete (*p);
	}
	delParticles.clear();
}