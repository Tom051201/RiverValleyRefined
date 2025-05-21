#include "SoundHandler.h"
#include <iostream>

SoundHandler::SoundHandler() {
	activeMusicIndex = -1;
	
	musicAllowed = true;
	soundsAllowed = true;
}

SoundHandler::~SoundHandler() {

	for (const auto& pair : sounds) {
		if (pair.second != nullptr) {
			Mix_FreeChunk(pair.second);
		}
	}
	sounds.clear();

	for (const auto& pair : musics) {
		if (pair.second != nullptr) {
			Mix_FreeMusic(pair.second);
		}
	}
	musics.clear();

	std::cout << "SoundHandler cleaned!" << std::endl;
}

void SoundHandler::loadEffect(int id, const char* path) {
	SDL_RWops* file = SDL_RWFromFile(path, "rb");
	if (file == nullptr) {
		std::cerr << "Failed to open file with SDL_RWFromFile: " << SDL_GetError() << std::endl;
		return;
	}

	Mix_Chunk* sound = Mix_LoadWAV_RW(file, 1);
	if (sound == nullptr) {
		std::cerr << "Failed to load sound! " << Mix_GetError() << std::endl;
		SDL_FreeRW(file);
		return;
	}
	else {
		std::cout << "Successfully loaded effect: " << path << std::endl;
	}

	sounds[id] = sound;
}

void SoundHandler::playEffect(int soundIndex, int channel) {
	if (sounds.size() > soundIndex && channel <= Channels::MAX_CHANNELS && soundsAllowed) {
		Mix_PlayChannel(channel, sounds[soundIndex], 0);
	}
}

void SoundHandler::loopEffect(int soundIndex, int channel) {
	if (sounds.size() > soundIndex && channel <= Channels::MAX_CHANNELS && soundsAllowed) {
		Mix_PlayChannel(channel, sounds[soundIndex], -1);
	}
}

void SoundHandler::stopEffect(int channel) {
	if (channel <= Channels::MAX_CHANNELS) {
		Mix_HaltChannel(channel);
	}
}

void SoundHandler::pauseEffect(int channel) {
	if (channel <= Channels::MAX_CHANNELS) {
		Mix_Pause(channel);
	}
}

void SoundHandler::resumeEffect(int channel) {
	if (channel <= Channels::MAX_CHANNELS && soundsAllowed) {
		Mix_Resume(channel);
	}
}

void SoundHandler::setEffectVolume(int soundIndex, int volumePercent) {
	if (sounds.size() > soundIndex) {
		if (volumePercent < 0) volumePercent = 0;
		if (volumePercent > 100) volumePercent = 100;

		Mix_VolumeChunk(sounds[soundIndex], (volumePercent * MIX_MAX_VOLUME) / 100);
	}
}

void SoundHandler::setChannelVolume(int channel, int volumePercent) {
	if (channel <= Channels::MAX_CHANNELS) {
		if (volumePercent < 0) volumePercent = 0;
		if (volumePercent > 100) volumePercent = 100;

		Mix_Volume(channel, (volumePercent * MIX_MAX_VOLUME) / 100);
	}
}

void SoundHandler::fadeInChannel(int channel, int seconds) {

}

void SoundHandler::fadeOutChannel(int channel, int seconds) {

}

void SoundHandler::panChannel(int channel, int left, int right) {

}

void SoundHandler::setSoundPosition(int channel, int degrees, int distance) {

}

void SoundHandler::loadMusic(int id, const char* path) {
	SDL_RWops* file = SDL_RWFromFile(path, "rb");
	if (file == nullptr) {
		std::cerr << "Failed to open file with SDL_RWFromFile: " << SDL_GetError() << std::endl;
		return;
	}

	Mix_Music* music = Mix_LoadMUS_RW(file, 1);
	if (music == nullptr) {
		std::cerr << "Failed to load sound! " << Mix_GetError() << std::endl;
		SDL_FreeRW(file);
		return;
	}
	else {
		std::cout << "Successfully loaded sound: " << path << std::endl;
	}

	musics[id] = music;
}

void SoundHandler::playMusic(int soundIndex) {
	if (musics.size() > soundIndex && musicAllowed) {
		activeMusicIndex = soundIndex;
		if (soundIndex != -1) {
			Mix_PlayMusic(musics[activeMusicIndex], 0);
		}
	}
}

void SoundHandler::loopMusic(int soundIndex) {
	if (musics.size() > soundIndex && musicAllowed) {
		activeMusicIndex = soundIndex;
		if (soundIndex != -1) {
			Mix_PlayMusic(musics[activeMusicIndex], -1);
		}
	}
}

void SoundHandler::stopMusic() {
	Mix_HaltMusic();
}

void SoundHandler::pauseMusic() {
	Mix_PauseMusic();
}

void SoundHandler::resumeMusic() {
	if (musicAllowed) Mix_ResumeMusic();
}

void SoundHandler::setMusicVolume(int volumePercent) {
	if (volumePercent < 0) volumePercent = 0;
	if (volumePercent > 100) volumePercent = 100;

	Mix_VolumeMusic((volumePercent * MIX_MAX_VOLUME) / 100);
}

void SoundHandler::fadeInMusic(int seconds) {

}

void SoundHandler::fadeOutMusic(int seconds) {

}

bool SoundHandler::isChannelPlaying(int channel) {
	return Mix_Playing(channel);
}

bool SoundHandler::isMusicPlaying() {
	return Mix_PlayingMusic();
}

Mix_Chunk* SoundHandler::getEffectInChannel(int channel) {
	return nullptr;
}

int SoundHandler::getMusicTrack() { return activeMusicIndex; }

void SoundHandler::setMusicTrack(int index) { activeMusicIndex = index; }

void SoundHandler::allowSounds() { soundsAllowed = true; }
void SoundHandler::forbidSounds() { soundsAllowed = false; }
bool SoundHandler::isSoundsAllowed() { return soundsAllowed; }
void SoundHandler::allowMusic() { musicAllowed = true; }
void SoundHandler::forbidMusic () { musicAllowed = false; }
bool SoundHandler::isMusicAllowed() { return musicAllowed; }