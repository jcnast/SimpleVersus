// include game objects
#include "GameMaster.h"
#include "ControllerMaster.h"
#include "AudioMaster.h"

// include necessary libraries
#include <iostream>
#include <time.h>
#include <cstdlib>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_joystick.h>

// REMEMBER  << -lSDL2 -lSDL2_image >> FLAGS WHEN COMPILING

int main( int argc, char* args[] )
{
// initialize SDL video and joystick
	if(SDL_Init( SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK ) < 0){
		std::cout << "Could not initialize: SDL error: " << SDL_GetError() << std::endl;
		return 1;
	}else{
		// SDL has been initialized, continue with the rest of main()
		// std::cout << "SDL Initialized" << std::endl;
	}
	if(!(IMG_Init(IMG_INIT_PNG))){
    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
  }
  //Set texture filtering to linear
	if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
	{
		printf( "Warning: Linear texture filtering not enabled!" );
	}

/**** seed random to current time ****/
  srand(time(NULL));

/**** set up all objects needed for game to run ****/
  AudioMaster *audio = AudioMaster::GetInstance();

	SDL_SetHint(SDL_HINT_JOYSTICK_ALLOW_BACKGROUND_EVENTS,"1");
	SDL_JoystickEventState(SDL_ENABLE);
	ControllerMaster *controllers = ControllerMaster::GetInstance();

	GameMaster *game = GameMaster::GetInstance();
	game->SetNumPlayers(SDL_NumJoysticks());
	game->SetUp();

/**** Main Loop Handler ****/
	bool quit = false;

/**** Event Handler ****/
	SDL_Event e;

// *** update the game objects (keep playing) ***
	while(!quit){
		// handle any events raised
		while(SDL_PollEvent(&e) != 0){
		// user wants to quit
			if(e.type == SDL_QUIT){
				// std::cout << "Game Quit" << std::endl;
				quit = true;
			}else if(e.type == SDL_JOYAXISMOTION){
				// DO NOT NEED TO UPDATE WHEN THIS IS DONE, AM JUST READING AXIS VALUES
			}else if(e.type == SDL_JOYBUTTONDOWN){
				// update controller class
				controllers->Update(e, e.jbutton.which);
			}else if(e.type == SDL_JOYBUTTONUP){
				// update controller class
				controllers->Update(e, e.jbutton.which);
			}else if(e.type == SDL_JOYDEVICEADDED){
				// if game started, find player with no controller and assign them controller, otherwise ignore
				controllers->AddController(e, e.jdevice.which);
			}else if(e.type == SDL_JOYDEVICEREMOVED){
				// delete the associated controller class
				controllers->RemoveController(e, e.jdevice.which);			
			}
		}
		// game loop updates
		game->GameLoop();
	}
/**** Game is done being played, delete game before quitting ****/
	delete game;
	delete controllers;
	delete audio;
	IMG_Quit();
	SDL_Quit();
	return 0;
}