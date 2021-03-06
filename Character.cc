// include game objects
#include "Character.h"
#include "Rectangle.h"
#include "Bullet.h"
#include "Ground.h"
#include "Collision.h"
#include "ControllerMaster.h"
#include "Controller.h"
#include "ParticleSystem.h"
#include "Sprite.h"
#include "GameMaster.h"
#include "AudioMaster.h"

// include necessary libraries
#include <cmath>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_events.h>

//debug
#include <iostream>

Character::Character(bool facingRight, float xPos, float yPos, float xVelo, float yVelo):
	Rectangle(25, 75, xPos, yPos, xVelo, yVelo, 100, 0.9), facingRight(facingRight){
	
	// game master
	game = GameMaster::GetInstance();

	// game controller
	controller = NULL;

	// if the player is active
	active = false;

	// set player stats
	health = 100;
	dead = false;
	deathStart = 0;
	deathLength = 2;

	uncontrollable = false;
	uncontrollableStart = 0;
	uncontrollableLength = 0.5;
	uncontrollableAccel = 500;

	jumping = false;
	jumpMaxSpeed = 350;

	moveAccel = 2000;
	moveSpeed = 500;

	collisionOffset = 5;

	inAir = true;
	onRightWall = false;
	onLeftWall = false;

	charging = false;
	chargeReset = true;
	maxChargeLength = 0.6;

	immune = false;
	immuneDuration = 0.5;

	bulletDamage = 10;
	bulletSpeed = 700;

	// set sprite attributes
	sprite = new Sprite(6);
	curFrame = 0;
	walkFrameRate = 10;
	lastSpriteChange = 0;
	leftXOffset = 15;
	rightXOffset = 35;
	sprite->SetRenderer(game->GetRenderer());
	sprite->SetImage("./Images/Character.png");
	sprite->Darken(true);
}

Character::~Character(){
	// get rid of controller
	RemoveController();
	// stop the game keeping track of this character
	game->DeleteCharacter(this);
	// delete sprite
	delete sprite;
}

// set the character's controller
void Character::GetController(){
	// remove current controller
	RemoveController();

	// set the character's controller to be the given controller
	controller = ControllerMaster::GetController(this);
}

// remove the character's controller
void Character::RemoveController(){
	if(controller != NULL){
		controller->RemoveCharacter();
		controller = NULL;
	}
}

// does the character have a controller
bool Character::HasController(){
	if(controller == NULL){
		return false;
	}else{
		return true;
	}
}

// is the character active
bool Character::IsActive(){
	return active;
}

// set if the character is active
void Character::SetActive(bool newActive){
	active = newActive;
}

// set the character's color
void Character::SetColor(Uint8 newRed, Uint8 newGreen, Uint8 newBlue){
	red = newRed;
	green = newGreen;
	blue = newBlue;

	sprite->ColourChange(red, green, blue);
}

// reset character to default condition
void Character::Reset(bool activeStatus){
	// if the player is active
	active = activeStatus;
	xVelo = 0; 
	yVelo = 0;

	// set player stats
	health = 100;
	dead = false;

	jumping = false;

	inAir = true;
	onRightWall = false;
	onLeftWall = false;
	charging = false;
	immune = false;


	sprite->Darken(!activeStatus);
}

// set the character to be considered 'in the air'
void Character::SetInAir(){
	inAir = true;
}

// set the character's position
void Character::SetPosition(bool facingR, float xPos, float yPos){
	facingRight = facingR;
	this->xPos = xPos;
	this->yPos = yPos;
}

// set the character to not be considered 'on a wall'
void Character::SetNotOnWall(){
	onLeftWall = false;
	onRightWall = false;
}

// is the character on the ground
bool Character::Grounded(){
	return (!inAir);
}

bool Character::Dead(){
	return (dead);
}

// update player physics
void Character::PhysicsUpdate(float deltaTime){
	// only apply gravity when the character is in the air
	if(inAir){
		if(jumping){
			ApplyAcceleration(0, gravity*0.15, deltaTime);
		}else{
			ApplyAcceleration(0, gravity, deltaTime);
		}
	}

	// always move the plyer accordingly
	xPos += (xVelo*deltaTime);
	yPos -= (yVelo*deltaTime); // - because of SDL weird y-axis
}

// character hits another character
void Character::OnCollision(Character *hitCharacter, Collision *coll){
	// characters turn around and bounce away from each other on collision

	int collX = (int)(coll->GetXPos());
	int collY = (int)(coll->GetYPos());

	bool lrColl = false;
	bool tbColl = false;

	int xCollOffset = game->DeltaTime()*std::abs(xVelo);
	if(xCollOffset < collisionOffset){
		xCollOffset = collisionOffset;
	}
	int yCollOffset = game->DeltaTime()*std::abs(yVelo);
	if(yCollOffset < collisionOffset){
		yCollOffset = collisionOffset;
	}

	// x velocity
	if(/*xVelo > 0 && */collX <= (int)(GetRight()) && collX > (int)(GetRight() - xCollOffset)){ // switching to ints to remove rounding errors, only pixel position matters
		if(!(collY <= (int)(GetBottom()) && collY > (int)(GetBottom() - yCollOffset))){
		// take care of when you hit character to your right
			if(!onLeftWall){
				SetVelocity(-400, yVelo);
			}

			lrColl = true;
		}
	}else if(/*xVelo < 0 && */collX >= (int)(GetLeft()) && collX < (int)(GetLeft() + xCollOffset)){ // switching to ints to remove rounding errors, only pixel position matters
		
		if(!(collY <= (int)(GetBottom()) && collY > (int)(GetBottom() - yCollOffset))){
		// take care of when you hit character to your left
			if(!onRightWall){
				SetVelocity(400, yVelo);
			}

			lrColl = true;
		}
	// this check must be done down here due to how thin the rectangle is
	}
	if(collY >= (int)(GetCenterY() - yCollOffset) && collY <= (int)(GetCenterY() + yCollOffset)){
		// collision is to the side
	}else if(collY >= (int)(GetTop()) && collY <= (int)(GetTop() + yCollOffset)){
		// take care of hitting player with head
		if(inAir){
			SetVelocity(xVelo, -400);

			tbColl = true;
			// hitting head stops player's jump
			if(jumping){
				jumping = false;
			}
		}
	}else if(collY <= (int)(GetBottom()) && collY >= (int)(GetBottom() - yCollOffset)){
		// take care of hitting player with feet
		SetVelocity(xVelo, 400);
		inAir = true;

		tbColl = true;

		// stepping on someone's head does damage
		hitCharacter->ApplyDamage(bulletDamage*2, this);
		
		// particle effects
		ParticleSystem *hitParticles = new ParticleSystem(255, 125, 0, 0, 0, 0, 1, 0.5, coll->GetXPos(), coll->GetYPos(), 35, -35, 35, -35, 8, 3, true, 20, 0);
		game->AddParticleSystem(hitParticles);
	}

	if(lrColl || tbColl){
		// character cannot be controlled
		uncontrollable = true;
		uncontrollableStart = SDL_GetTicks();

		// visual effect
		// sprite->StartFlash(red/2, green/2, blue/2, uncontrollableLength, 0.1);

		// play sound
		AudioMaster::PlaySound("./Sounds/CharacterGrunt.wav", 50);
	}
}
 // character hits bullet
void Character::OnCollision(Bullet *bullet, Collision *coll){
	// bullet is in charge of doing damage to bullet
	if(bullet->GetCharacter() != this && !immune){ // do not interact with 'friendly' bullet

		ParticleSystem *hitParticles = new ParticleSystem(255, 125, 0, 0, 0, 0, 1, 0.5, coll->GetXPos(), coll->GetYPos(), 35, -35, 35, -35, 8, 3, true, 20, 0);
		game->AddParticleSystem(hitParticles);

		// play sound
		AudioMaster::PlaySound("./Sounds/CharacterGrunt.wav", 30);
	}
}

// character hits ground
void Character::OnCollision(Ground *ground, Collision *coll){
	// collision affects velocity
	int collX = (int)(coll->GetXPos());
	int collY = (int)(coll->GetYPos());

	int xCollOffset = game->DeltaTime()*std::abs(xVelo);
	if(xCollOffset < collisionOffset){
		xCollOffset = collisionOffset;
	}
	int yCollOffset = game->DeltaTime()*std::abs(yVelo);
	if(yCollOffset < collisionOffset){
		yCollOffset = collisionOffset;
	}

	// x velocity
	if(xVelo >= 0 && collX <= (int)(GetRight()) && collX >= (int)(GetRight() - xCollOffset)){ // switching to ints to remove rounding errors, only pixel position matters
	
		if(!(collY <= (int)(GetBottom()) && collY > (int)(GetBottom() - yCollOffset))){
		// take care of when on top corner of platform
			onRightWall = true;
			xVelo = 0;
			xPos -= ((int)(GetRight()) - collX);
		}
	}else if(xVelo <= 0 && collX >= (int)(GetLeft()) && collX <= (int)(GetLeft() + xCollOffset)){ // switching to ints to remove rounding errors, only pixel position matters
		
		if(!(collY <= (int)(GetBottom()) && collY > (int)(GetBottom() - yCollOffset))){
		// take care of when on top corner of platform
			onLeftWall = true;
			xVelo = 0;
			xPos -= ((int)(GetLeft()) - collX);
		}
	// this check must be done down here due to how thin the rectangle is
	}else if(collX >= (int)(GetCenterX() - xCollOffset) && collX <= (int)(GetCenterX() + xCollOffset)){
		// collision is underfoot
	}

	// character has landed
	if(inAir && yVelo <= 0 && collY <= (int)(GetBottom()) && collY > (int)(GetBottom() - yCollOffset)){
		inAir = false;
		jumping = false;

		// spawn particles
		ParticleSystem *landParticles = new ParticleSystem(205, 65, 110, 30, 25, 0, 0.5, 0.2, coll->GetXPos(), coll->GetYPos(), -50, 50, 100, 50, 10, 4, true, 20, 0.4);
		game->AddParticleSystem(landParticles);

		// play sound
		AudioMaster::PlaySound("./Sounds/CharacterLanding.wav", 30);
	}
	// y velocity
	if(collY >= (int)(GetCenterY() - yCollOffset) && collY <= (int)(GetCenterY() + yCollOffset)){
		// collision is to the side
	}else if(yVelo >= 0 && collY >= (int)(GetTop()) && collY <= (int)(GetTop() + yCollOffset)){
		yVelo = 0;
		yPos -= ((int)(GetTop()) - collY);

		// hitting head stops player's jump
		if(jumping){
			jumping = false;
			chargeReset = false;
		}

		// spawn particles
		ParticleSystem *headParticles = new ParticleSystem(205, 65, 110, 30, 25, 0, 0.5, 0.2, coll->GetXPos(), coll->GetYPos(), -50, 50, 100, 50, 10, 4, true, 20, 0.4);
		game->AddParticleSystem(headParticles);
	}else if(yVelo <= 0 && collY <= (int)(GetBottom()) && collY >= (int)(GetBottom() - yCollOffset)){
		yVelo = 0;
		yPos -= ((int)(GetBottom()) - collY);
	}
}

// character takes damage
void Character::ApplyDamage(float damage, Character *enemy){
	if(!immune){
		// apply damage
		health -= damage;
		// start invulnerability frames
		immune = true;
		immuneStart = SDL_GetTicks();
		// damage taken flash
		sprite->StartFlash(red+255, green+255, blue+255, immuneDuration, 0.1);
	}

	if(health <= 0){
		dead = true;
		deathStart = SDL_GetTicks();
		sprite->StartFade(deathLength);
	}
}

// character menu controls
void Character::MenuUpdate(float deltaTime){
	float curTime = SDL_GetTicks();
	// if the character does not have a controller, try to get one
	if(controller == NULL){
		GetController();
	}
	
	// act according to controller values
	if(controller != NULL){
		// if the button is being pressed
		bool buttonDown = controller->ButtonDown();
		bool startCharging = false;
		bool endCharging = false;
		// determine if the player is now charging
		if(buttonDown && !charging){
			startCharging = true;
			chargeStart = SDL_GetTicks();
			charging = true;
		// or if they have stopped charging
		}else if(!buttonDown && charging){
			endCharging = true;
			chargeEnd = SDL_GetTicks();
			charging = false;
		}
		// set player to active, or start game
		if(active && ((endCharging && ((chargeEnd - chargeStart)/1000) >= 0.5) || (charging && ((SDL_GetTicks() - chargeStart)/1000) >= 0.5))){
			charging = false;
			game->RequestStartGame();
		}else if(endCharging){
			if(!active){
				sprite->Darken(false);
				active = true;

				// play sound
				AudioMaster::PlaySound("./Sounds/CharacterActive.wav", 40);
			}else{
				sprite->Darken(true);
				active = false;

				// play sound
				AudioMaster::PlaySound("./Sounds/CharacterNotActive.wav", 30);
			}
		}
	}
}

// game over input
void Character::GameOverUpdate(float deltaTime){
	float curTime = SDL_GetTicks();
	// if the character does not have a controller, try to get one
	if(controller == NULL){
		GetController();
	}
	
	// act according to controller values
	if(controller != NULL){
		// if the button is being pressed
		bool buttonDown = controller->ButtonDown();
		bool startCharging = false;
		bool endCharging = false;
		// determine if the player is now charging
		if(buttonDown && !charging){
			startCharging = true;
			chargeStart = SDL_GetTicks();
			charging = true;
		// or if they have stopped charging
		}else if(!buttonDown && charging){
			endCharging = true;
			chargeEnd = SDL_GetTicks();
			charging = false;
		}
		// inform game to go back to menu
		if((endCharging && ((chargeEnd - chargeStart)/1000) >= 0.5) || (charging && ((SDL_GetTicks() - chargeStart)/1000) >= 0.5)){
			// hold for a rematch
			game->Restart(true);
		}else if(endCharging){
			// quick press to go to main menu
			game->Restart();
		}
	}
}

// character controls
void Character::Update(float deltaTime){
	float curTime = SDL_GetTicks();
	// if the character does not have a controller, try to get one
	if(controller == NULL){
		GetController();
	}
	// if the character is immune, check if they should now be made vulnerable
	if(immune){
		if(((curTime - immuneStart)/1000) >= immuneDuration){
			immune = false;
		}
	}
	// if the character is uncontrollable, check if they should be made controllable
	if(uncontrollable){
		if(((curTime - uncontrollableStart)/1000) >= uncontrollableLength){
			uncontrollable = false;
		}
	}
	
	// act according to controller values
	if(controller != NULL && !dead && !uncontrollable){
		// if the button is being pressed
		bool buttonDown = controller->ButtonDown();
		bool startCharging = false;
		bool endCharging = false;

		// determine if the player is now charging
		if(buttonDown && !charging){
			startCharging = true;
			chargeStart = SDL_GetTicks();
			charging = true;
			chargeReset = false;
		// or if they have stopped charging
		}else if(!buttonDown && charging){
			endCharging = true;
			chargeEnd = SDL_GetTicks();
			charging = false;
			if(!chargeReset){
				chargeReset = true;
			}
		}

		// player can always move
		int xVal = controller->XValue();
		int xDir = 0;
		// controller is pointing to the right
		if(xVal > 0){
			xDir = 1;
			facingRight = true;
		// controller is pointing to the left
		}else if(xVal < 0){
			xDir = -1;
			facingRight = false;
		}
		// no direction was given
		if(xDir == 0){
			// accelerate in opposite direction to slow down
			if(xVelo > 0){
				if(!inAir){
					ApplyAcceleration(-1*moveAccel, 0 , deltaTime);
					// accelerate to a stop only
					if(xVelo < 0){
						xVelo = 0;
					}
				}
			}else if(xVelo < 0){
				if(!inAir){
					ApplyAcceleration(moveAccel, 0, deltaTime);
					// accelerate to a stop only
					if(xVelo > 0){
						xVelo = 0;
					}
				}
			}else{
				// not moving
			}
		// move, up to the max moveSpeed, in the desired direction
		}else if(xVelo >= moveSpeed && xDir == 1){
			// set velocity to max velocity
			xVelo = moveSpeed;

			if(onRightWall){
				// do not move further into wall
				xVelo = 0;
				// holding on to wall slows decent
				if(inAir && yVelo <= 0){
					// ApplyAcceleration(0, -1*gravity*0.95, deltaTime);
					SetVelocity(0, -25);
				}
			}else if(onLeftWall){
				// moving away from wall
				onLeftWall = false;
			}
		}else if(xVelo <= -1*moveSpeed && xDir == -1){
			// set velocity to negative max velocity
			xVelo = xDir*moveSpeed;

			if(onLeftWall){
				// do not move further into wall
				xVelo = 0;
				// holding on to wall slows decent
				if(inAir && yVelo <= 0){
					// ApplyAcceleration(0, -1*gravity*0.95, deltaTime);
					SetVelocity(0, -25);
				}
			}else if(onRightWall){
				// moving away from wall
				onRightWall = false;
			}
		}else{
			if(onRightWall && xDir == 1){
				// do not move further into wall
				xVelo = 0;
				// holding on to wall slows decent
				if(inAir && yVelo <= 0){
					// ApplyAcceleration(0, -1*gravity*0.95, deltaTime);
					SetVelocity(0, -25);
				}
			}else if(onLeftWall && xDir == -1){
				// do not move further into wall
				xVelo = 0;
				// holding on to wall slows decent
				if(inAir && yVelo <= 0){
					// ApplyAcceleration(0, -1*gravity*0.95, deltaTime);
					SetVelocity(0, -25);
				}
			}else{
				onRightWall = false;
				onLeftWall = false;
				// accelerate in the desired direction
				ApplyAcceleration(xDir*moveAccel, 0, deltaTime);
			}
		}

		// character is in the air, and the button is released
		if(inAir && endCharging){
			// if they are jumping, they stop jumping
			if(jumping){
				jumping = false;
			}else if(chargeReset){
				// otherwise, they shoot
				Shoot();
			}
		}else if(inAir && startCharging && yVelo <= 0 && !jumping){
			// on right wall and player is holding right
			if(onRightWall){
				Jump(deltaTime);
				jumping = true;
			// on left wall and play is holding left
			}else if(onLeftWall){
				Jump(deltaTime);
				jumping = true;
			}
		// character is not in the air, and releases the button, or they are in the air and the button is down in the air while they are jumping
		}else if((!inAir && startCharging && !jumping) || (inAir && jumping && charging)){
			// then they continue to jump
			jumpChargeLength = (SDL_GetTicks() - chargeStart)/1000;
			if(jumpChargeLength < maxChargeLength){
				// have character jump
				if(!jumping){
					Jump(deltaTime);
					inAir = true;
				}
				jumping = true;
			// character has jumped for max duration
			}else{
				jumping = false;
				// charging ends, forcing character to re-press button to fire
				chargeReset = true;
			}
		}
	}else{
		// accelerate in opposite direction to slow down
		if(xVelo > 0){
			if(uncontrollable){
				ApplyAcceleration(-1*uncontrollableAccel, 0, deltaTime);
			}else{
				ApplyAcceleration(-1*moveAccel, 0 , deltaTime);
			}
		}else if(xVelo < 0){
			if(uncontrollable){
				ApplyAcceleration(uncontrollableAccel, 0, deltaTime);
			}else{
				ApplyAcceleration(moveAccel, 0, deltaTime);
			}
		}else{
			// not moving
		}
	}
	if(dead){
		if((SDL_GetTicks() - deathStart)/1000 >= deathLength){
			game->QueueDelete(this);
		}
	}
}

// draw character sprite
void Character::Render(){
  // render sprite
  float xOffset = leftXOffset;
  if(!facingRight){
  	xOffset = rightXOffset;
  }

  // character is standing by default
  int clip = 0;
  lastSpriteChange++;

  // character has just been hit
  if(immune || uncontrollable){
  	clip = 3;
  // character in the air
  }else if(inAir){
  	// character is on left wall
  	if(onLeftWall && yVelo <= 0){
  		clip = 5;
  	// character is on right wall
  	}else if(onRightWall && yVelo <= 0){
  		clip = 5;
  	// character is jumping
  	}else{
  		clip = 4;
  	}
  // character has just been hit
  }else if(xVelo*xVelo > 0){
  	if(lastSpriteChange > walkFrameRate){
  		if(curFrame == 1){
  			clip = 2;
  		}else{
  			clip = 1;
  		}
  		lastSpriteChange = 0;
  	}else{
  		clip = curFrame;
  	}
  }
	curFrame = clip;

	// on right wall makes you look in to map
	if(onRightWall){
		xOffset = rightXOffset;
		facingRight = false;
	// on left wall makes you look in to map
	}else if(onLeftWall){
  	xOffset = leftXOffset;
		facingRight = true;
	}

	sprite->Render(xPos - xOffset, yPos, clip, !facingRight);
	// render health bar
	if(!dead && active){
		SDL_Rect healthBarRect = {GetXPos()-(health/2), yPos-20, health, 10};
		SDL_SetRenderDrawColor(game->GetRenderer(), 0xFF, 0x00, 0x00, 0xFF);
		SDL_RenderFillRect(game->GetRenderer(), &healthBarRect);
	}

	// draw where the hitbox should be
	// SDL_Rect outlineRect = {xPos, yPos, width, height};
 //  SDL_SetRenderDrawColor(game->GetRenderer(), 0x00, 0xFF, 0x00, 0xFF);        
 //  SDL_RenderDrawRect(game->GetRenderer(), &outlineRect);
}

// character wants to jump
void Character::Jump(float deltaTime){
	// have the character jump
	SetVelocity(xVelo, yVelo+(jumpMaxSpeed));

	// play sound
	AudioMaster::PlaySound("./Sounds/CharacterJump.wav", 60);

	// spawn particles
	ParticleSystem *jumpParticles = new ParticleSystem(255, 190, 255, 190, 255, 190, 0.5, 0.2, GetCenterX(), GetBottom(), 25, -25, 80, -20, 10, 4, true, 20, 0);
	game->AddParticleSystem(jumpParticles);
}

// character fires a bullet
void Character::Shoot(){
	// bullet info
	float xPos = GetRight();
	float yPos = ((GetTop()+GetBottom())/2);
	float xVel = bulletSpeed;
	float yVel = 0;

	if(onRightWall){
		facingRight = false;
	}else if(onLeftWall){
		facingRight = true;
	}

	// spawn bullet on the side the character is facing
	if(!facingRight){
		xPos = GetLeft();
		xVel = -1*xVel;
	}
	// offset for where the player's gun is
	float xOffset = leftXOffset;
  if(!facingRight){
  	xOffset = -1*rightXOffset;
  }
  // apply offset to bullet spawn point
  xPos += xOffset;
	// the bullet fired
	Bullet *newBullet = new Bullet(this, bulletDamage, xPos, yPos-10, xVel, yVel);
	newBullet->SetColor(red, green, blue);
	// make the game keep track of bullet
	game->AddBullet(newBullet);

	// play sound
	AudioMaster::PlaySound("./Sounds/BulletFired.wav", 60);
}