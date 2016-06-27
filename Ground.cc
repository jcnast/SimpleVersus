// include game objects
#include "Ground.h"
#include "Rectangle.h"
#include "Bullet.h"
#include "Character.h"
#include "Collision.h"
#include "ParticleSystem.h"
#include "GameMaster.h"
#include "AudioMaster.h"
#include "Texture.h"

// include necessary libraries

Ground::Ground(float xPos, float yPos, bool useImage, float width, float height):
	Rectangle(width, height, xPos, yPos, 0, 0, 10000), useImage(useImage){

	// game master
	game = GameMaster::GetInstance();

	// set rectangle's sprite
	sprite = new Texture(width, height);
	sprite->SetRenderer(game->GetRenderer());
	if(useImage){
		sprite->SetImage("./Images/Ground.png");	
	}else{
		sprite->SetImage("./Images/Empty.png");
	}
}

Ground::~Ground(){
	// stop the game keeping track of this ground
	game->RemoveGround(this);
	// delete sprite
	delete sprite;
}

void Ground::PhysicsUpdate(float deltaTime){
	// ground does not move from default physics
}

void Ground::OnCollision(Character *hitCharacter, Collision *coll){
	// ground does nothing when character hits it
}

void Ground::OnCollision(Bullet *hitBullet, Collision *coll){
	if(useImage){
		// ground spawns impact particles when bullet hits it
		float pXVelo = (coll->GetXVel(hitBullet));

		// spawn particles
		ParticleSystem *hitParticles = new ParticleSystem(205, 65, 110, 30, 25, 0, 1, 0.5, coll->GetXPos(), coll->GetYPos(), 0.05*pXVelo, 0.01*pXVelo, -0.05*(coll->GetYVel(hitBullet)), -0.01*(coll->GetYVel(hitBullet)), 6, 1, true, 20);
		game->AddParticleSystem(hitParticles);

		// sound effects
		AudioMaster::PlaySound("./Sounds/BulletHitGround.wav", 20);
	}
}

void Ground::OnCollision(Ground *hitGround, Collision *coll){
	// ground does nothing when another ground hits it
}

// render image
void Ground::Render(){
  // render sprite
	sprite->Render(xPos, yPos);
	
	// draw where the hitbox should be
	// SDL_Rect outlineRect = {xPos, yPos, width, height};
 //  SDL_SetRenderDrawColor(game->GetRenderer(), 0x00, 0xFF, 0x00, 0xFF);        
 //  SDL_RenderDrawRect(game->GetRenderer(), &outlineRect);
}

// darken sprite
void Ground::Darken(bool dark){
	sprite->Darken(dark);
}