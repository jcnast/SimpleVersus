#ifndef GROUND_H
#define GROUND_H

// include game objects
#include "Rectangle.h"

// forward declarations
class Texture;
class Character;
class Bullet;
class GameMaster;
class Collision;

class Ground: public Rectangle{	
// game master
	GameMaster *game;

// ground sprite
	Texture *sprite;
	bool useImage;

public: // public methods
	Ground(float xPos, float yPos, bool useImage = true, float width = 100, float height = 50); // constructor
	~Ground(); // destructor

	void PhysicsUpdate(float deltaTime); // apply physics updates to object
	virtual void OnCollision(Character *hitCharacter, Collision *coll); // what to do after a collision
	virtual void OnCollision(Bullet *hitBullet, Collision *coll); // what to do after a collision
	virtual void OnCollision(Ground *hitGround, Collision *coll); // what to do after a collision

	virtual void Render(); // render the ground's sprite
	void Darken(bool dark); // darken ground

private: // private methods

};

#endif