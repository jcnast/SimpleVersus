// include game objects
#include "Bullet.h"
#include "Circle.h"
#include "Character.h"
#include "Ground.h"
#include "Collision.h"
#include "AudioMaster.h"
#include "Texture.h"
#include "GameMaster.h"

// include necessary libraries
#include <SDL2/SDL.h>

Bullet::Bullet(Character *character, float damage, float xPos, float yPos, float xVelo, float yVelo, float gravityPortion):
	Circle(10, xPos, yPos, xVelo, yVelo), character(character), damage(damage), gravityPortion(gravityPortion){

	// game master
	game = GameMaster::GetInstance();

	// set the bullet image
	sprite = new Texture(2*radius, 2*radius);
	sprite->SetRenderer(game->GetRenderer());
	sprite->SetImage("./Images/Bullet.png");
}

// clear object for destruction
Bullet::~Bullet(){
	// stop the game keeping track of this bullet
	game->RemoveBullet(this);
	// delete sprite
	delete sprite;
}

// set the bullet's color
void Bullet::SetColor(Uint8 red, Uint8 green, Uint8 blue){
	sprite->ColourChange(red, green, blue);
}

// update bullet physics
void Bullet::PhysicsUpdate(float deltaTime){
	// apply only a fraction of gravity
	ApplyAcceleration(0, gravity*gravityPortion, deltaTime);

	xPos += (xVelo*deltaTime);
	yPos -= (yVelo*deltaTime); // - because of SDL weird y-axis
}

void Bullet::OnCollision(Character *hitCharacter, Collision *coll){
	if(character != hitCharacter){
		hitCharacter->ApplyDamage(damage, this);
		// queue the bullet to be destroyed at the end of the current game loop
		game->QueueDelete(this);

		// audio
		AudioMaster::PlaySound("./Sounds/BulletHitGround.wav", 50);
	}else{
		// do not damage the character who fired the bullet
	}
}

void Bullet::OnCollision(Bullet *hitBullet, Collision *coll){
	// bullets pass right through each other
}

void Bullet::OnCollision(Ground *hitGround, Collision *coll){
	// queue the bullet to be destroyed at the end of the current game loop
	game->QueueDelete(this);
}

Character *Bullet::GetCharacter(){
	return character;
}

// do damage to character
void Bullet::DoDamage(Character *character){
	character->ApplyDamage(damage, this);
}

// render bullet sprite
void Bullet::Render(){
  // render sprite
	sprite->Render(xPos, yPos);
	
	// draw where the hitbox should be
	// SDL_Rect outlineRect = {xPos, yPos, radius*2, radius*2};
  // SDL_SetRenderDrawColor(game->GetRenderer(), 0x00, 0x00, 0xFF, 0xFF);        
  // SDL_RenderDrawRect(game->GetRenderer(), &outlineRect);
}