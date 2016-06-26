// include game objects
#include "AudioMaster.h"

// include libraries
#include <string>
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

// **********************************************************************************************
// classes used in implementation
// **********************************************************************************************

// Queue structure for all loaded sounds
class Sound{
public:
  uint32_t length;
  uint32_t lengthTrue;
  uint8_t *bufferTrue;
  uint8_t *buffer;
  uint8_t loop;
  uint8_t fade;
  uint8_t volume;

  SDL_AudioSpec audio;

  struct Sound *next;
};

// Definition for the game global sound device
class PrivateAudioDevice{
public:
  SDL_AudioDeviceID device;
  SDL_AudioSpec *want;
  uint8_t audioEnabled;
};

// **********************************************************************************************
// AudioMaster class
// **********************************************************************************************

// variable declarations
AudioMaster *AudioMaster::instance = NULL;
PrivateAudioDevice *AudioMaster::globalDevice= NULL;

// get the one, the only: AudioMaster
AudioMaster *AudioMaster::GetInstance(){
	if(instance == NULL){
		instance = new AudioMaster();
	}
	return instance;
}

AudioMaster::AudioMaster(){
	Sound * global;
  globalDevice = new PrivateAudioDevice();

  // has audio been enabled
  if(!(SDL_WasInit(SDL_INIT_AUDIO) & SDL_INIT_AUDIO)){
    std::cout << "Error: SDL_INIT_AUDIO not initialized" << std::endl;
    globalDevice->audioEnabled = 0;
    return;
  }else{ 
  	// audio has been enabled
    globalDevice->audioEnabled = 1;
  }

  globalDevice->want = new SDL_AudioSpec();

  // set up AudioSpec info
  (globalDevice->want)->freq = 48000;
  (globalDevice->want)->format = AUDIO_S16LSB;
  (globalDevice->want)->channels = 2;
  (globalDevice->want)->samples = 4096;
  (globalDevice->want)->callback = AudioCallback;
  (globalDevice->want)->userdata = new Sound();

  global = (Sound *)((globalDevice->want)->userdata);
  global->buffer = NULL;

  global->next = NULL;

  if((globalDevice->device = SDL_OpenAudioDevice(NULL, 0, globalDevice->want, NULL, SDL_AUDIO_ALLOW_ANY_CHANGE)) == 0)
  {
     std::cout << "Warning: failed to open audio device" << std::endl;
  }
  else
  {
    // Unpause active audio stream
    SDL_PauseAudioDevice(globalDevice->device, 0);
  }
}

AudioMaster::~AudioMaster(){
	// free required elements
  if(globalDevice->audioEnabled)
  {
    SDL_PauseAudioDevice(globalDevice->device, 1);

    FreeSound((Sound *) (globalDevice->want)->userdata);

    /* Close down audio */
    SDL_CloseAudioDevice(globalDevice->device);
  }

  delete globalDevice;
}

void AudioMaster::PlaySound(std::string filename, int volume)
{
  Sound *newSound;
  // if audio is not enabled, we cannot play anything
  if(!globalDevice->audioEnabled){
    return;
  }
  // create the sound that is wanting to be played
  newSound = CreateSound(filename, 0, volume);
  // lock audio device to add sound to it
  SDL_LockAudioDevice(globalDevice->device);
  // add sound
  AddSound((Sound *) (globalDevice->want)->userdata, newSound);
  // unlock once adding is done
  SDL_UnlockAudioDevice(globalDevice->device);
}

void AudioMaster::PlayMusic(std::string filename, int volume)
{
  Sound *global;
  Sound *newSound;
  uint8_t music;
  // if audio is not enabled we cannot play anything
  if(!globalDevice->audioEnabled){
    return;
  }

  music = 0;

  /* Create new music sound with loop */
  newSound = CreateSound(filename, 1, volume);

  /* Lock callback function */
  SDL_LockAudioDevice(globalDevice->device);
  global = ((Sound *) (globalDevice->want)->userdata)->next;

  /* Find any existing musics, 0, 1 or 2 */
  while(global != NULL){
    /* Phase out any current music */
    if(global->loop == 1 && global->fade == 0){
      if(music){
        global->length = 0;
        global->volume = 0;
      }

      global->fade = 1;
    }else if(global->loop == 1 && global->fade == 1){
    /* Set flag to remove any queued up music in favour of new music */
      music = 1;
    }

    global = global->next;
  }

  AddSound((Sound *) (globalDevice->want)->userdata, newSound);

  SDL_UnlockAudioDevice(globalDevice->device);
}

Sound *AudioMaster::CreateSound(std::string filename, uint8_t loop, int volume){
  Sound *newSound = new Sound();

  if(newSound == NULL){
    std::cout << "Error: Memory allocation error" << std::endl;
    return NULL;
  }

  newSound->next = NULL;
  newSound->loop = loop;
  newSound->fade = 0;
  newSound->volume = volume;

  if(SDL_LoadWAV(filename.c_str(), &(newSound->audio), &(newSound->bufferTrue), &(newSound->lengthTrue)) == NULL){
    std::cout << "Warning: failed to open wave file: " << filename << std::endl;
    delete newSound;
    return NULL;
  }

  newSound->buffer = newSound->bufferTrue;
  newSound->length = newSound->lengthTrue;
  (newSound->audio).callback = NULL;
  (newSound->audio).userdata = NULL;

  return newSound;
}

// play all elements in progress
void AudioMaster::AudioCallback(void * userdata, uint8_t * stream, int len){
  Sound * sound = (Sound *) userdata;
  Sound * previous = sound;
  int tempLength;
  uint8_t music = 0;

  /* Silence the main buffer */
  SDL_memset(stream, 0, len);

  /* First one is place holder */
  sound = sound->next;

  // play all sounds in list
  while(sound != NULL)
  {
    if(sound->length > 0)
    {
      if(sound->fade == 1 && sound->loop == 1)
      {
        music = 1;
        sound->volume--;

        if(sound->volume == 0)
        {
          sound->length = 0;
        }
      }

      if(music && sound->loop == 1 && sound->fade == 0)
      {
        tempLength = 0;
      }
      else
      {
        tempLength = (len > sound->length) ? sound->length : len;
      }

      SDL_MixAudioFormat(stream, sound->buffer, AUDIO_S16LSB, tempLength, sound->volume);

      sound->buffer += tempLength;
      sound->length -= tempLength;

      previous = sound;
      sound = sound->next;
    }
    else if(sound->loop == 1 && sound->fade == 0)
    {
      sound->buffer = sound->bufferTrue;
      sound->length = sound->lengthTrue;
    }
    else
    {
      previous->next = sound->next;
      SDL_FreeWAV(sound->bufferTrue);
      delete sound;

      sound = previous->next;
    }
  }
}

void AudioMaster::AddSound(Sound *root, Sound *newSound){
	// do nothing if no root list was given
  if(root == NULL){
    return;
  }

  // loop through all sounds to find last element
  while(root->next != NULL){
    root = root->next;
  }

  // add new sounds to list
  root->next = newSound;
}

void AudioMaster::FreeSound(Sound * sound){
  Sound * temp;

  // loop through all sounds to free them
  while(sound != NULL){
  	// free sounds
    SDL_FreeWAV(sound->bufferTrue);

    // update values
    temp = sound;
    sound = sound->next;

    // delete struct
    delete temp;
  }
}