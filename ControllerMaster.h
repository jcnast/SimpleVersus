#ifndef CONTROLLERMASTER_H
#define CONTROLLERMASTER_H

//include game objects

// include necessary libraries
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

class Character;
class Controller;

class ControllerMaster{
	static ControllerMaster *instance; // controller master instance

	static std::vector<Controller *> controllers; // all controllers

public: // public methods
	static ControllerMaster *GetInstance(); // get singleton
	~ControllerMaster(); // destructor

// provide an open controller
	static Controller *GetController(Character *character);
// update a specific controller based on a given event
	void Update(SDL_Event &e, SDL_JoystickID iD);
	void AddController(SDL_Event &e, int index);
	void RemoveController(SDL_Event &e, SDL_JoystickID iD);

// info about the controllers
	int TotalControllers();

private: // private methods
	ControllerMaster(); // constructor
};

#endif