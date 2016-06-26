// include game objects
#include "Circle.h"
#include "Object.h"
#include "Rectangle.h"
#include "Collision.h"

// include necessary libraries
#include <cmath>
#include <iostream>

Circle::Circle(float radius, float xPos, float yPos, float xVelo, float yVelo, float mass, float dampCo):\
	Object(xPos, yPos, xVelo, yVelo, mass, dampCo), radius(radius){
		// nothing else need to be created
}

Circle::~Circle(){
	// nothing to destroy
}

// circle collides with a circle
Collision *Circle::CollisionCheck(Circle *object){
	// colliding circle info
	float objRadius = (int)(object->GetRadius());
	float objXPos = (int)(object->GetXPos());
	float objYPos = (int)(object->GetYPos());
	float objXVelo = (int)(object->GetXVelo());
	float objYVelo = (int)(object->GetYVelo());

	// only do the calculation if the objects are not moving away from each other
	// since if they are, it is because they have already collided
	if((objXPos-(int)(GetXPos()))*xVelo >= 0 || ((int)(GetYPos())-objYPos)*yVelo >= 0){

		// if the circles are close enough to collide
		if(objRadius+radius <= Distance((int)(GetXPos()), (int)(GetYPos()), objXPos, objYPos)){
			// rest of colliding circle info
			float objMass = object->GetMass();

			// where the two circles collide
			float xColl = (((int)(GetXPos())*objRadius) + (objXPos*radius))/(radius+objRadius);
			float yColl = (((int)(GetYPos())*objRadius) + (objYPos*radius))/(radius+objRadius);
			
			// the velocity of the collision
			float newXVel = (xVelo*(mass-objMass) + (2*objMass*objXVelo))/(mass+objMass);
			float newYVel = (yVelo*(mass-objMass) + (2*objMass*objYVelo))/(mass+objMass);
			float newObjXVel = (objXVelo*(objMass-mass) + (2*mass*xVelo))/(mass+objMass);
			float newObjYVel = (objYVelo*(objMass-mass) + (2*mass*yVelo))/(mass+objMass);

			// update the velocities of the two circles
			Collision *newCollision = new Collision(this, object, xColl, yColl, newXVel, newYVel, newObjXVel, newObjYVel);
			return newCollision;
		}
	}else{
		// no collision
		return NULL;
	}
}

// circle collides with a rectangle
Collision *Circle::CollisionCheck(Rectangle *object){
	float objXPos = (int)(object->GetXPos());
	float objYPos = (int)(object->GetYPos());
	float objXVelo = (int)(object->GetXVelo());
	float objYVelo = (int)(object->GetYVelo());

	// only do the calculation if the objects are not moving away from each other
	// since if they are, it is because they have already collided
	if((objXPos-GetXPos())*xVelo >= 0 || (GetYPos()-objYPos)*yVelo >= 0){

		// colliding rectangle information
		float topYValue = (int)(object->GetTop());
		float bottomYValue = (int)(object->GetBottom());
		float leftXValue = (int)(object->GetLeft());
		float rightXValue = (int)(object->GetRight());

		// where the collision occurred
		float xColl;
		float yColl;
		bool collisionFound = false;

		// placeholder for solving R^2 = x^2 + y^2
		float hitValue;

		// check for top intersection
		hitValue = radius*radius - (topYValue-(int)(GetYPos()))*(topYValue-(int)(GetYPos()));
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			yColl = topYValue;
			// circle intersects on the right side of the left boundary
			bool leftHit = (leftXValue < (-1*sqrt(hitValue) + (int)(GetXPos())) && rightXValue > (-1*sqrt(hitValue) + (int)(GetXPos())));
			// circle intersects on the left side of the right boundary
			bool rightHit = (rightXValue > (sqrt(hitValue) + (int)(GetXPos())) && leftXValue < (sqrt(hitValue) + (int)(GetXPos())));

			// determine result of collision
			if(leftHit && rightHit){
				// collided at circle's x position
				xColl = GetXPos();
				collisionFound = true;
			}else if(leftHit){
				// collided at rectangle's right corner
				xColl = rightXValue;
				collisionFound = true;
			}else if(rightHit){
				// collided at rectangle's left corner
				xColl = leftXValue;
				collisionFound = true;
			}else{
				// no collision
				return NULL;
			}
		}
		// check for bottom intersection
		hitValue = radius*radius - (bottomYValue-(int)(GetYPos()))*(bottomYValue-(int)(GetYPos()));
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's bottom side
			yColl = bottomYValue;
			// circle intersects on the right side of the left boundary
			bool leftHit = (leftXValue < (-1*sqrt(hitValue) + (int)(GetXPos())) && rightXValue > (-1*sqrt(hitValue) + (int)(GetXPos())));
			// circle intersects on the left side of the right boundary
			bool rightHit = (rightXValue > (sqrt(hitValue) + (int)(GetXPos())) && leftXValue < (sqrt(hitValue) + (int)(GetXPos())));

			// determine result of collision
			if(leftHit && rightHit){
				// collided at circle's x position
				xColl = (int)(GetXPos());
				collisionFound = true;
			}else if(leftHit){
				// collided at rectangle's right corner
				xColl = rightXValue;
				collisionFound = true;
			}
			else if(rightHit){
				// collided at rectangle's left corner
				xColl = leftXValue;
				collisionFound = true;
			}else{
				// no collision
				return NULL;
			}
		}
		// check for left intersection
		hitValue = radius*radius - (leftXValue-(int)(GetXPos()))*(leftXValue-(int)(GetXPos()));
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			xColl = leftXValue;
			// circle intersects on the right side of the left boundary
			bool topHit = (topYValue < (-1*sqrt(hitValue) + (int)(GetYPos())) && bottomYValue > (-1*sqrt(hitValue) + (int)(GetYPos())));
			// circle intersects on the left side of the right boundary
			bool bottomHit = (bottomYValue > (sqrt(hitValue) + (int)(GetYPos())) && topYValue < (sqrt(hitValue) + (int)(GetYPos())));

			// determine result of collision
			if(topHit && bottomHit){
				// collided at circle's x position
				yColl = (int)(GetYPos());
				collisionFound = true;
			}else if(topHit){
				// collided at rectangle's top corner
				yColl = topYValue;
				collisionFound = true;
			}
			else if(bottomHit){
				// collided at rectangle's bottom corner
				yColl = bottomYValue;
				collisionFound = true;
			}else{
				// no collision
				return NULL;
			}
		}
		// check for right intersection
		hitValue = radius*radius - (rightXValue-(int)(GetXPos()))*(rightXValue-(int)(GetXPos()));
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			xColl = rightXValue;
			// circle intersects on the right side of the left boundary
			bool topHit = (topYValue < (-1*sqrt(hitValue) + (int)(GetYPos())) && bottomYValue > (-1*sqrt(hitValue) + (int)(GetYPos())));
			// circle intersects on the left side of the right boundary
			bool bottomHit = (bottomYValue > (sqrt(hitValue) + (int)(GetYPos())) && topYValue < (sqrt(hitValue) + (int)(GetYPos())));

			// determine result of collision
			if(topHit && bottomHit){
				// collided at circle's x position
				yColl = (int)(GetYPos());
				collisionFound = true;
			}else if(topHit){
				// collided at rectangle's top corner
				yColl = topYValue;
				collisionFound = true;
			}
			else if(bottomHit){
				// collided at rectangle's bottom corner
				yColl = bottomYValue;
				collisionFound = true;
			}else{
				// no collision
				return NULL;
			}
		}

		// if the objects collide, apply the collision
		if(collisionFound){
			float objMass = object->GetMass();

			// the velocity of the collision
			float newXVel = (xVelo*(mass-objMass) + (2*objMass*objXVelo))/(mass+objMass);
			float newYVel = (yVelo*(mass-objMass) + (2*objMass*objYVelo))/(mass+objMass);
			float newObjXVel = (objXVelo*(objMass-mass) + (2*mass*xVelo))/(mass+objMass);
			float newObjYVel = (objYVelo*(objMass-mass) + (2*mass*yVelo))/(mass+objMass);

			// update the velocities of the two circles
			Collision *newCollision = new Collision(this, object, xColl, yColl, newXVel, newYVel, newObjXVel, newObjYVel);
			return newCollision;
		}else{
			// no collision
			return NULL;
		}
	}else{
		// no collision
		return NULL;
	}
}

// get the radius of the circle
float Circle::GetRadius(){
	return radius;
}

// get x position of circle
float Circle::GetXPos(){
	return (xPos+radius);
}

// get y position of circle
float Circle::GetYPos(){
	return (yPos+radius);
}
