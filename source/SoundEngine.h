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
	FMOD::Sound*	m_sound;
	FMOD::Channel*	m_channel;
	char m_soundsHandle;
	std::vector<Sound*> m_sounds;

	float m_delay;
	float m_currDelay;

	//std::future<unsigned char*> m_sounds; //custom allocator here?

public:
	SoundEngine() {};
	~SoundEngine();

	bool Init();
	bool Clean();
	void Update(float dt);
	void AddSound(const char* _filePath);

};

#endif