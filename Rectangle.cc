// include game objects
#include "Rectangle.h"
#include "Object.h"
#include "Circle.h"
#include "Collision.h"

// include necessary libraries
#include <cmath>
#include <cstdlib>

Rectangle::Rectangle(float width, float height, float xPos, float yPos, float xVelo, float yVelo, float mass, float dampCo):
	Object(xPos, yPos, xVelo, yVelo, mass, dampCo), width(width), height(height){
	// nothing else needs to be set
}

Rectangle::~Rectangle(){
	// nothing else to destroy
}

// collide with another rectangle
Collision *Rectangle::CollisionCheck(Rectangle *object){
	float objXPos = (int)(object->GetXPos());
	float objYPos = (int)(object->GetYPos());
	float objXVelo = (int)(object->GetXVelo());
	float objYVelo = (int)(object->GetYVelo());

	// rectangle's information
	float topYValue = (int)(GetTop());
	float bottomYValue = (int)(GetBottom());
	float leftXValue = (int)(GetLeft());
	float rightXValue = (int)(GetRight());

	// object's information
	// rectangle's information
	float objTopYValue = (int)(object->GetTop());
	float objBottomYValue = (int)(object->GetBottom());
	float objLeftXValue = (int)(object->GetLeft());
	float objRightXValue = (int)(object->GetRight());

	// where the collision occurred
	float xColl;
	float yColl;
	bool collisionFound = false;
	// only do the calculation if the objects are not moving away from each other
	// since if they are, it is because they have already collided
	if((objXPos-(int)(GetXPos()))*xVelo >= 0 || ((int)(GetYPos())-objYPos)*yVelo >= 0){

		// check if collision is at the top of the object
		if(!collisionFound && bottomYValue >= objTopYValue && bottomYValue < objBottomYValue && topYValue < objTopYValue){
			yColl = objTopYValue;
			// rectangle is too far to one side for collision
			if(leftXValue > objRightXValue || rightXValue < objLeftXValue){
				// no collision
				return NULL;
			// rectangle has only the left side touching
			}else if(leftXValue <= objRightXValue && rightXValue > objRightXValue){
				xColl = ((objRightXValue+leftXValue)/2);
				collisionFound = true;
			// rectangle has only the right side touching
			}else if(leftXValue < objLeftXValue && rightXValue >= objLeftXValue){
				xColl = ((rightXValue+objLeftXValue)/2);
				collisionFound = true;
			// rectangle has both sides touching
			}else if(leftXValue >= objLeftXValue && rightXValue <= objRightXValue){
				xColl = ((rightXValue+leftXValue)/2);
				collisionFound = true;
			// rectangle is too big to touch both ends
			}else{
				xColl = ((objRightXValue+objLeftXValue)/2);
				collisionFound = true;
			}
		}
		// or at the bottom of the object
		if(!collisionFound && topYValue > objTopYValue && topYValue <= objBottomYValue && bottomYValue > objBottomYValue){
			yColl = objBottomYValue;
			// rectangle is too far to one side for collision
			if(leftXValue > objRightXValue || rightXValue < objLeftXValue){
				// no collision
				return NULL;
			// rectangle has only the left side touching
			}else if(leftXValue <= objRightXValue && rightXValue > objRightXValue){
				xColl = ((objRightXValue+leftXValue)/2);
				collisionFound = true;
			// rectangle has only the right side touching
			}else if(leftXValue < objLeftXValue && rightXValue >= objLeftXValue){
				xColl = ((rightXValue+objLeftXValue)/2);
				collisionFound = true;
			// rectangle has both sides touching
			}else if(leftXValue >= objLeftXValue && rightXValue <= objRightXValue){
				xColl = ((rightXValue+leftXValue)/2);
				collisionFound = true;
			// rectangle is too big to touch
			}else{
				xColl = ((objRightXValue+objLeftXValue)/2);
				collisionFound = true;
			}
		}
		// or at the right side of the object
		if(!collisionFound && rightXValue >= objLeftXValue && rightXValue < objRightXValue && leftXValue < objLeftXValue){
			xColl = objLeftXValue;
			// rectangle is too far up or down for collision
			if(bottomYValue < objTopYValue || topYValue > objBottomYValue){
				// no collision
				return NULL;
			// rectangle has only the top side touching
			}else if(topYValue <= objBottomYValue && bottomYValue > objBottomYValue){
				yColl = ((topYValue+objBottomYValue)/2);
				collisionFound = true;
			// rectangle has only the bottom side touching
			}else if(bottomYValue > objTopYValue && topYValue <= objTopYValue){
				yColl = ((objTopYValue+bottomYValue)/2);
				collisionFound = true;
			// rectangle has both edges touching
			}else if(topYValue >= objTopYValue && bottomYValue <= objBottomYValue){
				yColl = ((topYValue+bottomYValue)/2);
				collisionFound = true;
			// rectangle is too big to touch
			}else{
				yColl = ((objTopYValue+objBottomYValue)/2);
				collisionFound = true;
			}
		}
		// or at the left side of the object
		if(!collisionFound && leftXValue <= objRightXValue && leftXValue > objLeftXValue && rightXValue > objRightXValue){
			xColl = objRightXValue;
			// rectangle is too far up or down for collision
			if(bottomYValue < objTopYValue || topYValue > objBottomYValue){
				// no collision
				return NULL;
			// rectangle has only the top side touching
			}else if(topYValue <= objBottomYValue && bottomYValue > objBottomYValue){
				yColl = ((topYValue+objBottomYValue)/2);
				collisionFound = true;
			// rectangle has only the bottom side touching
			}else if(bottomYValue > objTopYValue && topYValue <= objTopYValue){
				yColl = ((objTopYValue+bottomYValue)/2);
				collisionFound = true;
			// rectangle has both edges touching
			}else if(topYValue >= objTopYValue && bottomYValue <= objBottomYValue){
				yColl = ((topYValue+bottomYValue)/2);
				collisionFound = true;
			// rectangle is too big to touch
			}else{
				yColl = ((objTopYValue+objBottomYValue)/2);
				collisionFound = true;
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
	}
}

// collide with a circle
Collision *Rectangle::CollisionCheck(Circle *object){
	float objXPos = (int)(object->GetXPos());
	float objYPos = (int)(object->GetYPos());
	float objXVelo = (int)(object->GetXVelo());
	float objYVelo = (int)(object->GetYVelo());

	// only do the calculation if the objects are not moving away from each other
	// since if they are, it is because they have already collided
	if((objXPos-GetXPos())*xVelo >= 0 || (GetYPos()-objYPos)*yVelo >= 0){

		// colliding circle information
		float radius = object->GetRadius();

		// rectangle's information
		float topYValue = (int)(GetTop());
		float bottomYValue = (int)(GetBottom());
		float leftXValue = (int)(GetLeft());
		float rightXValue = (int)(GetRight());

		// where the collision occurred
		float xColl;
		float yColl;
		bool collisionFound = false;

		// placeholder for solving R^2 = x^2 + y^2
		float hitValue;

		// check for top intersection
		hitValue = radius*radius - (topYValue-objYPos)*(topYValue-objYPos);
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			yColl = topYValue;
			// circle intersects on the right side of the left boundary
			bool leftHit = (leftXValue < (-1*sqrt(hitValue) + objXPos) && rightXValue > (-1*sqrt(hitValue) + objXPos));
			// circle intersects on the left side of the right boundary
			bool rightHit = (rightXValue > (sqrt(hitValue) + objXPos) && leftXValue < (sqrt(hitValue) + objXPos));

			// determine result of collision
			if(leftHit && rightHit){
				// collided at circle's x position
				xColl = objXPos;
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
		hitValue = radius*radius - (bottomYValue-objYPos)*(bottomYValue-objYPos);
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's bottom side
			yColl = bottomYValue;
			// circle intersects on the right side of the left boundary
			bool leftHit = (leftXValue < (-1*sqrt(hitValue) + objYPos) && rightXValue > (-1*sqrt(hitValue) + objYPos));
			// circle intersects on the left side of the right boundary
			bool rightHit = (rightXValue > (sqrt(hitValue) + objYPos) && leftXValue < (sqrt(hitValue) + objYPos));

			// determine result of collision
			if(leftHit && rightHit){
				// collided at circle's x position
				xColl = objXPos;
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
		hitValue = radius*radius - (leftXValue-objXPos)*(leftXValue-objXPos);
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			xColl = leftXValue;
			// circle intersects on the right side of the left boundary
			bool topHit = (topYValue < (-1*sqrt(hitValue) + objYPos) && bottomYValue > (-1*sqrt(hitValue) + objYPos));
			// circle intersects on the left side of the right boundary
			bool bottomHit = (bottomYValue > (sqrt(hitValue) + objYPos) && topYValue < (sqrt(hitValue) + objYPos));

			// determine result of collision
			if(topHit && bottomHit){
				// collided at circle's x position
				yColl = objYPos;
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
		hitValue = radius*radius - (rightXValue-objXPos)*(rightXValue-objXPos);
		if(!collisionFound && hitValue >= 0){
			// collision at rectangle's top side
			xColl = rightXValue;
			// circle intersects on the right side of the left boundary
			bool topHit = (topYValue < (-1*sqrt(hitValue) + objYPos) && bottomYValue > (-1*sqrt(hitValue) + objYPos));
			// circle intersects on the left side of the right boundary
			bool bottomHit = (bottomYValue > (sqrt(hitValue) + objYPos) && topYValue < (sqrt(hitValue) + objYPos));

			// determine result of collision
			if(topHit && bottomHit){
				// collided at circle's x position
				yColl = objYPos;
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

// get rectangle x position
float Rectangle::GetXPos(){
	return (xPos+(width/2));
}

// get rectangle y position
float Rectangle::GetYPos(){
	return (yPos+(height/2));
}

// get the top of the rectangle
float Rectangle::GetTop(){
	return (yPos);
}

// get the bottom of the rectangle
float Rectangle::GetBottom(){
	return (yPos+height);
}

// get the left side of the rectangle
float Rectangle::GetLeft(){
	return (xPos);
}

// get the right side of the rectangle
float Rectangle::GetRight(){
	return (xPos+width);
}

// get the middle x position of the rectangle
float Rectangle::GetCenterX(){
	return (xPos+(width/2));
}

// get the middle y position of the rectangle
float Rectangle::GetCenterY(){
	return (yPos+(height/2));
}