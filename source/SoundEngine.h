#ifndef SOUNDENGINE_H
#define SOUNDENGINE_H

#include "stdafx.h"
#include "fmod\fmod.hpp"
#include "ResourceManager.h"
#include "Sound.h"


class SoundEngine
{
private:
	FMOD::System*	m_system;
	FMOD::Channel*	m_channel;
	std::vector<Sound*> m_sounds;
	std::vector<const char*> m_soundFiles;

	bool m_muted;
	char m_soundsHandle;
	float m_delay;
	float m_currDelay;
	float m_volume;

public:
	SoundEngine() {};
	~SoundEngine();

	bool Init();
	bool Clean();
	void Update(float dt);
	void AddSound(const char* _filePath);
	void ChangeVolume(float _value);

};

#endif