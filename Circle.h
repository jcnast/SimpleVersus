#ifndef CIRCLE_H
#define CIRCLE_H

// include game objects
#include "Object.h"

// include necessary libraries

// forward declarations
class Object;
class Rectangle;
class Collision;

class Circle: public Object{

protected:
	float radius; // radius of the circle

public: // public methods
	Circle(float radius, float xPos, float yPos, float xVelo, float yVelo, float mass = 1, float dampCo = 0.5); // constructor
	virtual ~Circle(); // destructor

	Collision *CollisionCheck(Circle *object); // check if the object has collided with given object
	Collision *CollisionCheck(Rectangle *object); // check if the object has collided with given object

	float GetRadius(); // get radius of circle
	float GetXPos(); // get x position
	float GetYPos(); // get y position

private: // private methods
	
};

#endif