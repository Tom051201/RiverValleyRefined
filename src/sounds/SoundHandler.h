#pragma once
#include <vector>
#include "SDL.h"
#include "SDL_mixer.h"
#include "Sounds.h"
#include <map>

class Channels {
public:
	static const int MAX_CHANNELS = 8;

	static const int GUI_INTERACTION = 0;
	static const int GAME_SOUNDS = 1;
	static const int AMBIENT_SOUNDS = 2;

};

class SoundHandler {
public:

	static const int MIN_PANNING = 0;
	static const int MAX_PANNING = 255;

	SoundHandler();
	~SoundHandler();

	void loadEffect(int id, const char* path);
	void playEffect(int soundIndex, int channel);
	void loopEffect(int soundIndex, int channel);
	void stopEffect(int channel);
	void pauseEffect(int channel);
	void resumeEffect(int channel);
	void setEffectVolume(int index, int volumePercent);
	void setChannelVolume(int channel, int volumePercent);
	void fadeInChannel(int channel, int seconds);
	void fadeOutChannel(int channel, int seconds);
	void panChannel(int channel, int left, int right);
	void setSoundPosition(int channel, int degrees, int distance);

	void loadMusic(int id, const char* path);
	void playMusic(int soundIndex);
	void loopMusic(int soundIndex);
	void stopMusic();
	void pauseMusic();
	void resumeMusic();
	void setMusicVolume(int volumePercent);
	void fadeInMusic(int seconds);
	void fadeOutMusic(int seconds);

	bool isChannelPlaying(int channel);
	bool isMusicPlaying();
	Mix_Chunk* getEffectInChannel(int channel);
	int getMusicTrack();
	void setMusicTrack(int index);

	void allowSounds();
	void forbidSounds();
	bool isSoundsAllowed();
	void allowMusic();
	void forbidMusic();
	bool isMusicAllowed();

private:

	int activeMusicIndex;

	std::map<int, Mix_Chunk*> sounds;
	std::map<int, Mix_Music*> musics;

	bool soundsAllowed, musicAllowed;

};