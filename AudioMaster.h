#ifndef AUDIO_H
#define AUDIO_H

// include necessary libraries
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

class Sound;
class PrivateAudioDevice;

class AudioMaster{

	static AudioMaster *instance;
	static PrivateAudioDevice *globalDevice;

public:
	static AudioMaster *GetInstance();
	~AudioMaster();

	static void PlaySound(std::string path, int volume);
	static void PlayMusic(std::string path, int volume);

private:
	// construct the audio master
	AudioMaster();

	// add sound to queue to be played
	static void AddSound(Sound *root, Sound *newSound);

	// free all sounds given
	static void FreeSound(Sound *sound);

	// create a sound
	static Sound *CreateSound(std::string filename, uint8_t loop, int volume);

	// Audio callback function for OpenAudioDevice
	static void AudioCallback(void *userdata, uint8_t * stream, int len);

};

#endif