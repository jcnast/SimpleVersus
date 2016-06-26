#ifndef RECTANGLE_H
#define RECTANGLE_H

// include game objects
#include "Object.h"

// include necessary libraries

// forward declarations
class Circle;
class Collision;

class Rectangle: public Object{	

protected:
	float width; // width of the rectangle
	float height; // height of the rectangle

public: // public methods
	Rectangle(float width, float height, float xPos, float yPos, float xVelo, float yVelo, float mass = 1, float dampCo = 0.5); // constructor
	virtual ~Rectangle(); // destructor

	Collision *CollisionCheck(Rectangle *object); // check if the object has collided with given object
	Collision *CollisionCheck(Circle *object); // check if the object has collided with given object

	float GetXPos(); // get x position
	float GetYPos(); // get y position
	
	float GetTop(); // get y value of top position
	float GetBottom(); // get y value of bottom position
	float GetLeft(); // get x value of left position
	float GetRight(); // get x value of right position
	float GetCenterX(); // get middle x position
	float GetCenterY(); // get middle y position

private: // private methods

};

#endif