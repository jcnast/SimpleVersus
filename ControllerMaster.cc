// include game objects
#include "ControllerMaster.h"
#include "Controller.h"
#include "Character.h"

// include necessary libraries
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

ControllerMaster *ControllerMaster::instance = NULL;
std::vector<Controller *> ControllerMaster::controllers;

// get singleton instance
ControllerMaster *ControllerMaster::GetInstance(){
	if(instance == NULL){
		instance = new ControllerMaster();
	}
	return instance;
}

// construct controller master
ControllerMaster::ControllerMaster(){
	// CONTROLERS ARE ADDED THROUGH DEVICEADDED EVENTS
}

// destruct controller master
ControllerMaster::~ControllerMaster(){
	std::vector<Controller *> tempControllers = controllers;
	for(std::vector<Controller *>::iterator c = tempControllers.begin(); c != tempControllers.end(); ++c) {
    delete *c;
	}
}

Controller *ControllerMaster::GetController(Character *character){
	for(std::vector<Controller *>::iterator c = controllers.begin(); c != controllers.end(); ++c) {
    if((*c)->GetCharacter() != NULL){
    	// this controller has a character and is therefore taken
    }else{
    	// this controller is free to be used
    	(*c)->AssignCharacter(character);
    	return *c;
    }
	}
	return NULL;
}

void ControllerMaster::Update(SDL_Event &e, SDL_JoystickID iD){
	for(std::vector<Controller *>::iterator c = controllers.begin(); c != controllers.end(); ++c) {
    if((*c)->GetID() == iD){
    	// update the desired controller
    	(*c)->Update(e);
    	break;
    }
	}
}

void ControllerMaster::AddController(SDL_Event &e, int index){ // DEVICEADDED event given the index of the joystick
	// add the new controller
	SDL_Joystick *curController = SDL_JoystickOpen(index);
	if(curController != NULL){
		controllers.push_back(new Controller(curController));
	}
}

void ControllerMaster::RemoveController(SDL_Event &e, SDL_JoystickID iD){ // DEVICEREMOVED event gives instanceID of joystick
	for(std::vector<Controller *>::iterator c = controllers.begin(); c != controllers.end(); ++c) {
    if((*c)->GetID() == iD){
    	// remove the joystick from it's character
    	Character *character = (*c)->GetCharacter();
    	if(character != NULL){
    		character->RemoveController();
    	}
    	controllers.erase(c);
    	break;
    }
	}
}

int ControllerMaster::TotalControllers(){
	return controllers.size();
}