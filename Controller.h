#ifndef CONTROLLER_H
#define CONTROLLER_H

// include game objects

// include necessary libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

class Character;

class Controller{
// properties
	// the related joystick
	SDL_Joystick *joystick;
	SDL_JoystickID iD;
	int deadZone;
	// the character it is assigned to
	Character *character;
	// only care about one button at a time
	Uint8 curButton;
	bool buttonPressed;
	// only care about left joystick x-axis values
	int xValue;

public:
	Controller(SDL_Joystick *joystick);
	~Controller();

	// update controller state
	void Update(SDL_Event &e);
	void AssignCharacter(Character *newCharacter);
	void RemoveCharacter();
	Character *GetCharacter();
	SDL_Joystick *GetJoystick();

	// get controller info
	bool ButtonDown();
	int XValue();
	SDL_JoystickID GetID();
};

#endif