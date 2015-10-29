#ifndef SOUND_H
#define SOUND_H
#include "stdafx.h"
#include "fmod\fmod.hpp"
#include <future>


class Sound
{
public:
	enum SOUND_PLAY_STATE
	{
		LOADING,
		READY,
		PLAYING,
		FINISHED
	};

private:
	std::future<void*> m_soundChunk;
	FMOD::Sound* m_fSound;
	SOUND_PLAY_STATE m_soundState;

	float m_currTime;
	float m_totTime;

public:
	Sound(const char* _filePath, FMOD::System* _system);
	~Sound();
	void UpdateSound(float dt);
	FMOD::Sound* GetSound();
	SOUND_PLAY_STATE GetPlayState();
	void SetPlaying(SOUND_PLAY_STATE _value);
};

#endif