#ifndef OBJECT_H
#define OBJECT_H

// include game objects

// include necessary libraries

class Collision;

class Object{

protected:
// stats of all objects
	static const float gravity; 
	static const float maxSpeed;
	float mass;
// position
	float xPos;
	float yPos;
// velocity
	float xVelo;
	float yVelo;
// damping coefficient
	float dampCo;

public: // public methods
	Object(float xPos, float yPos, float xVelo, float yVelo, float mass = 1, float dampCo = 0.5); // constructorQ
	virtual	~Object() = 0; // destructor

	virtual void PhysicsUpdate(float deltaTime); // apply physics updates to object
	virtual void OnCollision(Collision *coll); // what to do after a collision
	
	void ApplyAcceleration(float xAccel, float yAccel, float deltaTime); // apply acceleration to an object
	void SetVelocity(float xVel, float yel);

	virtual float GetXPos(); // get x position
	virtual float GetYPos(); // get y position
	float GetXVelo(); // get x velocity
	float GetYVelo(); // get y velocity
	float GetMass(); // get mass of object

protected:
	float Distance(float xPos1, float yPos1, float xPos2, float yPos2); // calculate distance between two points

private: // private methods
};

#endif