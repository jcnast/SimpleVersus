#ifndef BULLET_H
#define BULLET_H

// include game objects
#include "Circle.h"

// include necessary libraries
#include <SDL2/SDL.h>

// forward declarations
class Texture;
class Character;
class Ground;
class GameMaster;
class Collision;

class Bullet: public Circle{
// game master
	GameMaster *game;
	
// bullet sprite
	Texture *sprite;

// bullet stats
	float damage;
	float gravityPortion;

	Character *character; // the character that fired the bullet

public: // public methods
	Bullet(Character *character, float damage, float xPos, float yPos, float xVelo, float yVelo, float gravityPortion = 0.1); // constructor
	~Bullet(); // destructor
	void SetColor(Uint8 red, Uint8 green, Uint8 blue); // set character's color

	void PhysicsUpdate(float deltaTime); // apply physics updates to bullet
	virtual void OnCollision(Character *hitCharacter, Collision *coll); // what to do after a collision
	virtual void OnCollision(Bullet *hitBullet, Collision *coll); // what to do after a collision
	virtual void OnCollision(Ground *hitGround, Collision *coll); // what to do after a collision

	Character *GetCharacter(); // get the bullet's character

	virtual void DoDamage(Character *character); // do damage to character
	virtual void Render(); // render bullet sprite

private: // private methods

};

#endif