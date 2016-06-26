// include game objects
#include "Particle.h"
#include "Object.h"
#include "Circle.h"
#include "Texture.h"
#include "ParticleSystem.h"
#include "GameMaster.h"

// include necessary libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

Particle::Particle(ParticleSystem *pSystem, float xPos, float yPos, float xVelo, float yVelo, float radius, float duration, bool fades, float gravityPortion):
	Circle(radius, xPos, yPos, xVelo, yVelo), pSystem(pSystem), duration(duration), gravityPortion(gravityPortion){
	// when the particle was created
	startTime = SDL_GetTicks();

	// set the particle image
	sprite = new Texture(2*radius, 2*radius);
	sprite->SetRenderer(GameMaster::GetInstance()->GetRenderer());
	if(fades){
		sprite->StartFade(duration);
	}
	sprite->SetImage("./Images/Particle.png");
}

Particle::~Particle(){
	// stop the game keeping track of this particle
	pSystem->RemoveParticle(this);
	// delete sprite
	delete sprite;
}

// set the particle's color
void Particle::SetColor(Uint8 red, Uint8 green, Uint8 blue){
	sprite->ColourChange(red, green, blue);
}

void Particle::PhysicsUpdate(float deltaTime){
	ApplyAcceleration(0, gravity*gravityPortion, deltaTime);
	xPos += (GetXVelo()*deltaTime);
	yPos -= (GetYVelo()*deltaTime); // - because of SDL weird y-axis

	if((SDL_GetTicks() - startTime)/1000 >= duration){
		pSystem->QueueDelete(this);
	}
}

void Particle::Render(){
	// render sprite
	sprite->Render(xPos, yPos);
	
	// draw where the hitbox should be
	// SDL_Rect outlineRect = {xPos, yPos, radius*2, radius*2};
 //  SDL_SetRenderDrawColor(game->GetRenderer(), 0x00, 0x00, 0xFF, 0xFF);        
 //  SDL_RenderDrawRect(game->GetRenderer(), &outlineRect);
}