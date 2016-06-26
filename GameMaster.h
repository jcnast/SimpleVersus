#ifndef GAMEMASTER_H
#define GAMEMASTER_H

// include game objects

// include necessary libraries
#include <SDL2/SDL.h>
#include <vector>

// forward declarations
class Object;
class Ground;
class Character;
class Bullet;
class Texture;
class ParticleSystem;

class GameMaster{
	static GameMaster *instance;
	bool inMenu;
	bool leavingMenu;
	float outOfMenuAt; // time when the game will start
	float gameStartTime; // how long the game takes to start
	bool gameOver;
	float winnerFlashTime; // how long the winner image flashes

// the constant to be used in game master updates
	const int screenWidth;
	const int screenHeight;

// the time since game start and time of last update
	float lastUpdate;
	float currentUpdate;

// the objects that will be updates through the game master
	SDL_Window *window; // the window we'll be rendering to
	SDL_Renderer *renderer; // the renderer to render images
	Texture *background; // the background image of the game
	Texture *title; // the title image
	Texture *controls; // the controls
	Texture *winner; // the winner sprite

	// number of players
	int numPlayers;

	std::vector<Ground *> ground; // all ground objects
	std::vector<Character *> characters; // the players within the game
	std::vector<Bullet *> bullets; // the bullets that are in the air
	std::vector<ParticleSystem *> particleSystems; // the particle system in progress

	// holders for objects that will be deleted at the end of the game loop
	std::vector<Object *> delObjects;
	std::vector<Character *> delCharacters;
	std::vector<ParticleSystem *> delParticleSystems;

public: // public methods
	static GameMaster *GetInstance(); // get singleton
	~GameMaster(); // destructor

// game update functions
	void SetNumPlayers(int players); // set the number of players (MAX OF 4)
	void SetUp(); // set up initial game state
	void GameLoop(); // update game state
	void RequestStartGame(); // start timer for game start
	void StartGame(); // start game function
	void EndGame(); // end game function
	void Restart(bool rematch = false); // restart game

// game master information
	SDL_Window *GetWindow();
	SDL_Surface *GetSurface();
	SDL_Renderer *GetRenderer();
	float DeltaTime();
	bool IsGameOver();

// GamePlay functions
	void AddGround(Ground *newGround);
	void RemoveGround(Ground *delGround);
	void AddCharacter(Character *newCharacter);
	void RemoveCharacter(Character *delCharacter);
	void DeleteCharacter(Character *delCharacter);
	void AddBullet(Bullet *newBullet);
	void RemoveBullet(Bullet *delBullet);
	void AddParticleSystem(ParticleSystem *newParticleSystem);
	void RemoveParticleSystem(ParticleSystem *delParticleSystem);
	void QueueDelete(Object *delObj);
	void QueueDelete(Character *delCharacter);
	void QueueDelete(ParticleSystem *delParticleSystem);
	void DeleteObject(Object *object);
	void DeleteObject(Character *character);

private: // private methods
	GameMaster(int width = 640, int height = 480); // constructor

	// menu update function
	void MenuUpdate(); // update the menu
	void GameOverUpdate(); // go to main menu when requested

	// game update functions
	void PhysicsUpdate(); // update the objects within the game
	void Update(); // apply player controls
	void Render(); // re-draw the screen to apply changes
	void Flush(); // delete all objects that need to be deleted

	// create game objects
	void SpawnGround();
	void SpawnCharacters();
	void SpawnBullets(); // this is for collision testing purposes
};

#endif