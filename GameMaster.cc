// include game objects
#include "GameMaster.h"
#include "Texture.h"
#include "Object.h"
#include "Ground.h"
#include "Character.h"
#include "Bullet.h"
#include "Collision.h"
#include "ParticleSystem.h"
#include "AudioMaster.h"

// include necessary libraries
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <SDL2/SDL.h>
#include <SDL2/SDL_joystick.h>

// setting the default instance
GameMaster *GameMaster::instance = NULL;

// get the one, the only: GameMaster
GameMaster *GameMaster::GetInstance(){
	if(instance == NULL){
		instance = new GameMaster();
	}
	return instance;
}

// create the GameMaster
GameMaster::GameMaster(int width, int height):\
screenWidth(width), screenHeight(height){

// set state
	inMenu = true;
	leavingMenu = false;
	gameStartTime = 3;
	gameOver = false;
	winnerFlashTime = 100;

// set the time for updates
	lastUpdate = SDL_GetTicks();
	currentUpdate = SDL_GetTicks();

// set texture filtering to linear
	if(!(SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))){
		std::cout << "Warning: Linear texture filtering not enabled" << std::endl;
	}

// set the window, screen and background image of the game master
	this->window = SDL_CreateWindow("Simple Versus", \
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, \
		width, height, SDL_WINDOW_SHOWN);
	if(window == NULL){
		std::cout << "Window could not be created: " << SDL_GetError() << std::endl;
	}else{
		this->renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
		SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	}

	// set background
	this->background = new Texture(screenWidth, screenHeight);
	background->SetRenderer(this->renderer);
	background->SetImage("./Images/Background.png");
	background->Darken(true);

	// set title image
	this->title = new Texture(400, 100);
	title->SetRenderer(this->renderer);
	title->SetImage("./Images/Title.png");
	title->ColourChange(217, 0, 0);

	// set controls image
	this->controls = new Texture(400, 300);
	controls->SetRenderer(this->renderer);
	controls->SetImage("./Images/Controls.png");

	// set winner image
	this->winner = new Texture(400, 200);
	winner->SetRenderer(this->renderer);
	winner->SetImage("./Images/Winner.png");


// set the objects up for the game
	numPlayers = 0;

	ground;
	characters;
	bullets;

	delObjects;
	delCharacters;
	delParticleSystems;
}

// delete the GameMaster
GameMaster::~GameMaster(){
// flush out object queue to remove objects that should be deleted
	Flush();
// delete all objects in the game
	std::vector<Bullet *> tempBullets = bullets;
	for(std::vector<Bullet *>::iterator b = tempBullets.begin(); b != tempBullets.end(); ++b) {
    delete *b;
	}

	std::vector <Character *> tempCharacters = characters;
	for(std::vector<Character *>::iterator c = tempCharacters.begin(); c != tempCharacters.end(); ++c) {
    delete *c;
	}

	std::vector <Ground *> tempGround = ground;
	for(std::vector<Ground *>::iterator g = tempGround.begin(); g != tempGround.end(); ++g) {
    delete *g;
	}

// delete background
	delete background;
// delete title
	delete title;
// delete controls
	delete controls;
// delete winner
	delete winner;

// delete all SDL objects
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
}

// set the total number of players
void GameMaster::SetNumPlayers(int players){
	// max of 4 players
	if(players > 4){
		players = 4;
	}
	numPlayers = players;
}

// set up initial game state
void GameMaster::SetUp(){
	SpawnGround(); // spawn ground
	SpawnCharacters(); // spawn characters
	SpawnBullets(); // spawn bullets

	// start background music
	AudioMaster::PlayMusic("./Sounds/BackgroundMusic.wav", 20);
}

// update game state
void GameMaster::GameLoop(){
	lastUpdate = currentUpdate;
	currentUpdate = SDL_GetTicks();

	PhysicsUpdate();
	if(leavingMenu){
		if(SDL_GetTicks() - outOfMenuAt >= 0){
			StartGame();
		}
	}else if(inMenu){
		MenuUpdate();
	}else if(gameOver){
		GameOverUpdate();
	}else{
		Update();
	}
	Flush();
	Render();
}

// start game timer
void GameMaster::RequestStartGame(){
	// make sure at least 2 players are active before starting
	int activeCharacters = 0;
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
		if((*c)->IsActive()){
			activeCharacters++;
		}
	}
	if(activeCharacters >= 2){
		// start counting down to game start
		leavingMenu = true;
		outOfMenuAt = SDL_GetTicks() + (gameStartTime * 1000);
		title->StartFlash(254, 254, 254, gameStartTime, 0.2);
		controls->StartFade(gameStartTime);

		// sound effect
		AudioMaster::PlaySound("./Sounds/GameCountDown.wav", 30);
	}
}

// start the game
void GameMaster::StartGame(){
	if(inMenu){
		inMenu = false;
		leavingMenu = false;
		// lighten background
		background->Darken(false);

		// lighten all the grounds
		for(std::vector<Ground *>::iterator g = ground.begin(); g != ground.end(); ++g) {
	   	(*g)->Darken(false);
		}

		// sound effect
		AudioMaster::PlaySound("./Sounds/GameStart.wav", 100);
	}
}

// end the game
void GameMaster::EndGame(){
	if(!gameOver && !inMenu){
		gameOver = true;
		winner->StartFlash(229, 221, 0, winnerFlashTime, 0.1);

		// spawn particles
		ParticleSystem *leftParticles = new ParticleSystem(255, 229, 248, 221, 69, 0, 6, 2, screenWidth/2 - 200, 200, 50, -30, 40, 10, 8, 1, true, 50);
		AddParticleSystem(leftParticles);
		// spawn particles
		ParticleSystem *rightParticles = new ParticleSystem(255, 229, 248, 221, 69, 0, 6, 2, screenWidth/2 + 200, 200, 30, -50, 40, 10, 8, 1, true, 50);
		AddParticleSystem(rightParticles);

		// sound effects
		AudioMaster::PlaySound("./Sounds/Fireworks.wav", 128);
	}
}

// restart game
void GameMaster::Restart(bool rematch){
	if(gameOver){
		gameOver = false;
		inMenu = true;
		int index = 0;
		for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
			// dead characters are character that were active
			if((*c)->Dead()){
				(*c)->SetActive(true);
			}
			(*c)->Reset((*c)->IsActive());
			if(rematch && (*c)->IsActive()){
				(*c)->SetPosition((3-index)/2, screenWidth/5 + screenWidth/5*index, screenHeight-150);
			}else{
				(*c)->SetPosition((3-index)/2, screenWidth/5 + screenWidth/5*index, screenHeight-150);
			}

			index++;
		}

		if(rematch){
			RequestStartGame();
		}
	}
}

// get the window controlled by the GameMaster
SDL_Window *GameMaster::GetWindow(){
	return window;
}

// get the renderer controlled by the GameMaster
SDL_Renderer *GameMaster::GetRenderer(){
	return renderer;
}

// the time between updates
float GameMaster::DeltaTime(){
	return ((currentUpdate - lastUpdate)/1000);
}

// is the game over
bool GameMaster::IsGameOver(){
	return gameOver;
}

// add ground to keep track of
void GameMaster::AddGround(Ground *newGround){
	ground.push_back(newGround);
}

// remove ground from those being kept track of
void GameMaster::RemoveGround(Ground *delGround){
	std::vector<Ground *>::iterator pos = std::find(ground.begin(), ground.end(), delGround);
	if(pos != ground.end()){ // == myVector.end() means the element was not found
    ground.erase(pos);
  }
}

// add character to keep track of
void GameMaster::AddCharacter(Character *newCharacter){
	characters.push_back(newCharacter);
}

// remove character from those being kept track of
void GameMaster::RemoveCharacter(Character *delCharacter){
	int activeCharacters = 0;
	// set 'dead' character to inactive, and count remaining active players
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
		if((*c) == delCharacter){
			(*c)->SetActive(false);
		}else{
			if((*c)->IsActive()){
				activeCharacters++;
			}
		}
	}
  if(activeCharacters == 1){
  	EndGame();
  }
}

// delete character from game
void GameMaster::DeleteCharacter(Character *delCharacter){
	std::vector<Character *>::iterator pos = std::find(characters.begin(), characters.end(), delCharacter);
	if(pos != characters.end()){ // == myVector.end() means the element was not found
    characters.erase(pos);
  }
}

// add bullet to keep track of
void GameMaster::AddBullet(Bullet *newBullet){
	bullets.push_back(newBullet);
}

// remove bullet from those being kept track of
void GameMaster::RemoveBullet(Bullet *delBullet){
	std::vector<Bullet *>::iterator pos = std::find(bullets.begin(), bullets.end(), delBullet);
	if(pos != bullets.end()){ // == myVector.end() means the element was not found
    bullets.erase(pos);
  }
}

// add particle system to keep track of
void GameMaster::AddParticleSystem(ParticleSystem *newParticleSystem){
	particleSystems.push_back(newParticleSystem);
}

// remove particle system from those being kept track of
void GameMaster::RemoveParticleSystem(ParticleSystem *delParticleSystem){
	std::vector<ParticleSystem *>::iterator pos = std::find(particleSystems.begin(), particleSystems.end(), delParticleSystem);
	if(pos != particleSystems.end()){
		particleSystems.erase(pos);
	}
}

// queue up objects to be deleted at the end of the current game loop
void GameMaster::QueueDelete(Object *delObj){
	std::vector<Object *>::iterator pos = std::find(delObjects.begin(), delObjects.end(), delObj);
	if(pos == delObjects.end()){
		delObjects.push_back(delObj);
	}
}

void GameMaster::QueueDelete(Character *delCharacter){
	delCharacters.push_back(delCharacter);
}

// queue up particle system for deletion
void GameMaster::QueueDelete(ParticleSystem *delParticleSystem){
	delParticleSystems.push_back(delParticleSystem);
}

// update the menu
void GameMaster::MenuUpdate(){
	// see what players are active
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
    (*c)->MenuUpdate(DeltaTime());
	}
}

// update when game is over
void GameMaster::GameOverUpdate(){
	// according to player input
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
		if((*c)->IsActive()){
    	(*c)->GameOverUpdate(DeltaTime());
    }
	}
}

// update the objects within the game
void GameMaster::PhysicsUpdate(){
	// collisions for current game loop
	Collision *collision;

	// update ground physics (CURRENTLY GROUND OBJECTS DO NOT MOVE)
	// for(std::vector<Ground *>::iterator g = ground.begin(); g != ground.end(); ++g) {
   	// (*g)->PhysicsUpdate(DeltaTime());
	// }

	// update players physics
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {

		// only update if we are in the menu, or the character is active
		if((*c)->IsActive() || inMenu){
	    (*c)->PhysicsUpdate(DeltaTime());
	    bool groundColl = false;

	    // check if character collides with the ground
	   	for(std::vector<Ground *>::iterator g = ground.begin(); g != ground.end(); ++g) {
		    collision = (*c)->CollisionCheck(*g);
		    if(collision != NULL){
		    	(*c)->OnCollision(*g, collision);
		    	(*g)->OnCollision(*c, collision);
		    	groundColl = true;
		    	delete collision;
		    }
			}
			// if the character does not collide with any ground, they are considered to be 'in the air'
			if(!groundColl){
				(*c)->SetInAir();
				(*c)->SetNotOnWall();
			}

			// or with a bullet
			for(std::vector<Bullet *>::iterator b = bullets.begin(); b != bullets.end(); ++b) {
				// do not check bullets that the character fired
				if((*b)->GetCharacter() != *c){
			    collision = (*c)->CollisionCheck(*b);
			    if(collision != NULL && !((*c)->Dead())){
			    	(*c)->OnCollision(*b, collision);
			    	(*b)->OnCollision(*c, collision);
			    	delete collision;
			    }
			  }
			}

			// or with another character (starting with the first character that has not been checked)
			for(std::vector<Character *>::iterator oc = c+1; oc != characters.end(); ++oc){
				if(!(*oc)->Dead() && !(*c)->Dead()){
					collision = (*c)->CollisionCheck(*oc);
					if(collision != NULL){
						(*c)->OnCollision(*oc, collision);
						(*oc)->OnCollision(*c, collision);
						delete collision;
					}
				}
			}
		}
	}

	// updates bullets physics
	for(std::vector<Bullet *>::iterator b = bullets.begin(); b != bullets.end(); ++b) {
    (*b)->PhysicsUpdate(DeltaTime());

    // check if bullet collides with the ground
   	for(std::vector<Ground *>::iterator g = ground.begin(); g != ground.end(); ++g) {
	    collision = (*b)->CollisionCheck(*g);
	    if(collision != NULL){
	    	(*b)->OnCollision(*g, collision);
	    	(*g)->OnCollision(*b, collision);
	    	delete collision;
	    	// only hit one ground tile
	    	break;
	    }
		}
	}

	for(std::vector<ParticleSystem *>::iterator p = particleSystems.begin(); p != particleSystems.end(); ++p){
		(*p)->PhysicsUpdate(DeltaTime());
	}
}

// update players according to their controls
void GameMaster::Update(){
	// update players
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
    (*c)->Update(DeltaTime());
	}
}

// draw all objects to the screen
void GameMaster::Render(){
	// clear screen
	SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(renderer);

	// render background
	background->Render(0, 0);

	// render ground
	for(std::vector<Ground *>::iterator g = ground.begin(); g != ground.end(); ++g) {
   	(*g)->Render();
	}

	// render players
	for(std::vector<Character *>::iterator c = characters.begin(); c != characters.end(); ++c) {
		// only render if the character is active or we are in the menu
    if((*c)->IsActive() || inMenu){
    	(*c)->Render();
    }
	}

	// render particle systems
	for(std::vector<ParticleSystem *>::iterator p = particleSystems.begin(); p != particleSystems.end(); ++p){
		(*p)->Render();
	}

	// render bullets
	for(std::vector<Bullet *>::iterator b = bullets.begin(); b != bullets.end(); ++b) {
    (*b)->Render();
	}

	if(inMenu || leavingMenu){
		title->Render(screenWidth/2 - 200, 75);
		controls->Render(screenWidth/2 - 200, 250);
	}

	if(gameOver){
		winner->Render(screenWidth/2 - 200, 100);
	}

	SDL_RenderPresent(renderer);
}

// flush out all objects queued for destruction
void GameMaster::Flush(){
	for(std::vector<Object *>::iterator o = delObjects.begin(); o != delObjects.end(); ++o){
		DeleteObject(*o);
	}
	delObjects.clear();

	for(std::vector<Character *>::iterator c = delCharacters.begin(); c != delCharacters.end(); ++c){
		DeleteObject(*c);
	}
	delCharacters.clear();

	for(std::vector<ParticleSystem *>::iterator p = particleSystems.begin(); p != particleSystems.end(); ++p){
		(*p)->Flush();
	}

	for(std::vector<ParticleSystem *>::iterator p = delParticleSystems.begin(); p != delParticleSystems.end(); ++p){
		delete (*p);
	}
	delParticleSystems.clear();
}

// delete objects
void GameMaster::DeleteObject(Object *object){
	delete object;
}

// characters do not get 'deleted', just removed from being active
void GameMaster::DeleteObject(Character *character){
	RemoveCharacter(character);
}

// spawn the ground
void GameMaster::SpawnGround(){
	Ground *leftWall = new Ground(-50, 0, false, 50, screenHeight);
	AddGround(leftWall);
	Ground *rightWall = new Ground(screenWidth, 0, false, 50, screenHeight);
	AddGround(rightWall);

	Ground *ceiling = new Ground(0, -50, false, screenWidth);
	AddGround(ceiling);
	Ground *bottomGround = new Ground(0, screenHeight-25, true, screenWidth);
	AddGround(bottomGround);

	// make all platforms dark to start
	Ground *leftPlatform = new Ground(100, screenHeight - screenHeight/4 - 50, true, 300);
	leftPlatform->Darken(true);
	AddGround(leftPlatform);
	Ground *rightPlatform = new Ground(screenWidth - 400, screenHeight - screenHeight/4 - 50, true, 300);
	rightPlatform->Darken(true);
	AddGround(rightPlatform);
	Ground *middlePlatform = new Ground(screenWidth/2 - 150, screenHeight/2 - 50, true, 300);
	middlePlatform->Darken(true);
	AddGround(middlePlatform);
	Ground *topLeftPlatform = new Ground(0, screenHeight/4, true, 300);
	topLeftPlatform->Darken(true);
	AddGround(topLeftPlatform);
	Ground *topRightPlatform = new Ground(screenWidth - 300, screenHeight/4, true, 300);
	topRightPlatform->Darken(true);
	AddGround(topRightPlatform);
}

// spawn the characters
void GameMaster::SpawnCharacters(){
	for(int i = 0; i < numPlayers; i++){
		Character *newCharacter = new Character((3-i)/2, screenWidth/5 + screenWidth/5*i, screenHeight-150, 0, 0);
		// set character's colour
		Uint8 red = 255;
		if(i == 0 || i == 3){
			red = 255;
		}else if(i <= 3){
			red = 0;
		}else{
			red = rand()%255;
		}
		Uint8 green = 255;
		if(i == 1 || i == 3){
			green = 255;
		}else if(i <= 3){
			green = 0;
		}else{
			green = rand()%255;
		}
		Uint8 blue = 255;
		if(i == 2){
			blue = 255;
		}else if(i <= 3){
			blue = 0;
		}else{
			blue = rand()%255;
		}
		newCharacter->SetColor(red, green, blue);

		// keep track of character
		AddCharacter(newCharacter);
	}
}

void GameMaster::SpawnBullets(){
	// no bullets get spawned at game start
}