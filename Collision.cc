// include game objects
#include "Collision.h"
#include "Object.h"

// include necessary libraries

Collision::Collision(Object *obj_1, Object *obj_2, float xPos, float yPos,\
						float obj_1_xVel, float obj_1_yVel, float obj_2_xVel, float obj_2_yVel):
	obj_1(obj_1), obj_2(obj_2), xPos(xPos), yPos(yPos), obj_1_xVel(obj_1_xVel),\
	obj_1_yVel(obj_1_yVel), obj_2_xVel(obj_2_xVel), obj_2_yVel(obj_2_yVel){
	// nothing else to assign
}

Collision::~Collision(){
	// nothing to destruct
}

// get collision x coordinate
float Collision::GetXPos(){
	return xPos;
}

// get collision y coordinate
float Collision::GetYPos(){
	return yPos;
}

// get given object's new x velocity
float Collision::GetXVel(Object *object){
	if(object == obj_1){
		return obj_1_xVel;
	}else if(object == obj_2){
		return obj_2_xVel;
	}else{
		return 999999;
	}
}

// get given object's new y velocity
float Collision::GetYVel(Object *object){
	if(object == obj_1){
		return obj_1_yVel;
	}else if(object == obj_2){
		return obj_2_yVel;
	}else{
		return 999999;
	}
}

