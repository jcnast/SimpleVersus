#ifndef CHARACTER_H
#define CHARACTER_H

// include game objects
#include "Rectangle.h"

// include necessary libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

// forward declarations
class Sprite;
class Rectangle;
class Bullet;
class Ground;
class GameMaster;
class Collision;
class Controller;

class Character: public Rectangle{
// game master
	GameMaster *game;

// controller
	Controller *controller;

// character sound effects
	// SoundEffect *bulletFired;
	// SoundEffect *hitByBullet;
	// SoundEffect *jumpSound;
	// SoundEffect *landSound;

// character sprite
	Sprite *sprite;
	int curFrame;
	int walkFrameRate;
	int lastSpriteChange;
	float leftXOffset;
	float rightXOffset;
	Uint8 red;
	Uint8 green;
	Uint8 blue;

// if character is going to be in the game
	bool active;

// player stats
	float health;// = 100;
	bool dead; // is the character dead
	float deathStart; // when the death animation started
	float deathLength; // how long the death animation lasts

	bool uncontrollable; // if the character listens to controller input
	float uncontrollableStart; // when the uncontrollable-ness started
	float uncontrollableLength; // how long character is uncontrollable for
	float uncontrollableAccel;

	bool jumping; // character is still applying jump
	float jumpMaxSpeed;
	float jumpChargeLength;

	float moveAccel;
	float moveSpeed;

	bool facingRight; // direction character is facing
	float collisionOffset; // how close to a given edge the collision must be (# pixels)

	bool inAir; // is the character in the air
	bool onLeftWall; // character hitting left wall
	bool onRightWall; // character hitting right wall

	bool charging; // if a charge is in progress
	bool chargeReset; // if the button was released from the last charge
	float chargeStart; // when a charge has started
	float chargeEnd; // when a charge has ended
	float maxChargeLength; // 1 second

	bool immune; // if the character is currently immune
	float immuneDuration;// = 0.5 seconds
	float immuneStart;

// bullet info
	float bulletDamage;// = 10;
	float bulletSpeed;

public: // public methods
	Character(bool facingRight, float xPos, float yPos, float xVelo, float yVelo); // constructor
	~Character(); // destructor

	void GetController(); // set the character's controller
	void RemoveController(); // remove the player's controller
	bool HasController(); // true if player has controller assigned, false otherwise
	bool IsActive(); // true if the character is active
	void SetActive(bool newActive);

	void SetColor(Uint8 newRed, Uint8 newGreen, Uint8 newBlue); // set character's color
	
	void Reset(bool activeStatus); // reset character to default condition
	void SetInAir(); // character is considered to be 'in the air'
	void SetPosition(bool facingR, float xPos, float yPos);
	void SetNotOnWall(); // character is considered to not be on a wall
	bool Grounded(); // is the character on the ground
	bool Dead(); // is the character dead

	void PhysicsUpdate(float deltaTime); // apply physics updates to object
	virtual void OnCollision(Character *hitCharacter, Collision *coll); // what to do after a collision
	virtual void OnCollision(Bullet *bullet, Collision *coll); // what to do after a collision
	virtual void OnCollision(Ground *ground, Collision *coll); // what to do after a collision
	virtual void ApplyDamage(float damage, Character *enemy); // if the object takes damage

	void MenuUpdate(float deltaTime); // apply player input to menu
	void GameOverUpdate(float deltaTime); // apply player input to game over screen
	void Update(float deltaTime); // apply player controls to the player object
	virtual void Render(); // render character sprite

private: // private methods
	void Jump(float deltaTime); // jump
	void Shoot(); // fire a bullet

};

#endif