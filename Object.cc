// include game objects
#include "Object.h"
#include "Collision.h"

// include necessary libraries
#include <cmath>

const float Object::gravity = -500;
const float Object::maxSpeed = 400;

// create object by setting position, gravity and damping coefficient
Object::Object(float xPos, float yPos, float xVelo, float yVelo, float mass, float dampCo):\
	xPos(xPos), yPos(yPos), xVelo(xVelo), yVelo(yVelo), mass(mass), dampCo(dampCo){
	// nothing else to set
}

// destroy object
Object::~Object(){
	// nothing special to destroy
}

// update object position according to physics
void Object::PhysicsUpdate(float deltaTime){
	ApplyAcceleration(0, gravity, deltaTime);
	// only move if the velocity would move them over 1 pixel
	if(xVelo*deltaTime <= -1 || xVelo*deltaTime >= 1){
		xPos += (xVelo*deltaTime);
	}
	yPos -= (yVelo*deltaTime); // - because of SDL weird y-axis
}

void Object::OnCollision(Collision *coll){
	// collision affects velocity
	xVelo = coll->GetXVel(this);
	yVelo = coll->GetYVel(this);
}

// apply acceleration to an object
void Object::ApplyAcceleration(float xAccel, float yAccel, float deltaTime){
	xVelo += (xAccel*deltaTime);
	// max out x velocity
	// if(xVelo >= maxSpeed){
	// 	xVelo = maxSpeed;
	// }else if(xVelo <= -1*maxSpeed){
	// 	xVelo = -1*maxSpeed;
	// }

	yVelo += (yAccel*deltaTime);
	// max out y velocity
	// if(yVelo >= maxSpeed){
	// 	yVelo = maxSpeed;
	// }else 
	if(yVelo <= -1*maxSpeed){
		yVelo = -1*maxSpeed;
	}
}

// set object velocity
void Object::SetVelocity(float xVel, float yVel){
	xVelo = xVel;
	yVelo = yVel;
}

float Object::GetXPos(){
	return xPos;
}

float Object::GetYPos(){
	return yPos;
}

float Object::GetXVelo(){
	return xVelo;
}

float Object::GetYVelo(){
	return yVelo;
}

float Object::GetMass(){
	return mass;
}

float Object::Distance(float xPos1, float yPos1, float xPos2, float yPos2){
	float dist = sqrt((xPos2-xPos1)*(xPos2-xPos1) + (yPos2-yPos1)*(yPos2-yPos1));

	return dist;
}