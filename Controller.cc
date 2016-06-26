// include game objects
#include "Controller.h"

// include necessary libraries
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>


// create the controller
Controller::Controller(SDL_Joystick *joystick): joystick(joystick){
	// get that joystick's ID
	iD = SDL_JoystickInstanceID(joystick);
	deadZone = 8000;
	// character
	character = NULL;
	// default values
	buttonPressed = false;
	xValue = 0;
}

// destroy the controller
Controller::~Controller(){
	SDL_JoystickClose(joystick);
}

// update controller with a given event
void Controller::Update(SDL_Event &e){
	if(e.type == SDL_JOYAXISMOTION){
		// only care about x axis
		if(e.jaxis.axis == 0){
			xValue = e.jaxis.value;
		}
	}else if(e.type == SDL_JOYBUTTONDOWN){
		if(buttonPressed){
			// do nothing
		}else{
			curButton = e.jbutton.button;
			buttonPressed = true;
		}
	}else if( e.type == SDL_JOYBUTTONUP){
		if(buttonPressed && e.jbutton.button == curButton){
			buttonPressed = false;
		}else{
			// do nothing
		}
	}
}

// assign character to controller
void Controller::AssignCharacter(Character *newCharacter){
	// get ride of current character
	RemoveCharacter();
	// assign the new character
	character = newCharacter;
}

// remove the current character
void Controller::RemoveCharacter(){
	if(character != NULL){
		character = NULL;
	}
}

Character *Controller::GetCharacter(){
	return character;
}

SDL_Joystick *Controller::GetJoystick(){
	return joystick;
}

// if the button was pressed
bool Controller::ButtonDown(){
	return buttonPressed;
}

// get the x-axis value
int Controller::XValue(){
	xValue = SDL_JoystickGetAxis(joystick, 0);
 	if(xValue*xValue >= deadZone*deadZone){
		return xValue;
	}else{
		return 0;
	}
}

// get the joystick's ID
SDL_JoystickID Controller::GetID(){
	return iD;
}