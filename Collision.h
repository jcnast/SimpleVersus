#ifndef COLLISION_H
#define COLLISION_H

// include game objects

// include necessary libraries

class Object;

class Collision{
	// collision objects
	Object *obj_1;
	Object *obj_2;
	// collision location
	float xPos;
	float yPos;
	// collision velocity
	float obj_1_xVel;
	float obj_1_yVel;
	float obj_2_xVel;
	float obj_2_yVel;

public:
	// constructor
	Collision(Object *obj_1, Object *obj_2, float xPos, float yPos,\
				float obj_1_xVel, float obj_1_yVel, float obj_2_xVel, float obj_2_yVel);
	~Collision();
	// get collision information
	float GetXPos();
	float GetYPos();
	float GetXVel(Object *object);
	float GetYVel(Object *object);
};

#endif