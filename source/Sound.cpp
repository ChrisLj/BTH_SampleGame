#include "Sound.h"
#include "ResourceManager.h"

extern ResourceManager gResourceManager;

Sound::Sound(const char* _filePath, FMOD::System* _system)
{
	m_soundChunk = gResourceManager.LoadSound(_filePath, (void*)_system);
	m_fSound = 0;
	m_soundState = SOUND_PLAY_STATE::LOADING;

	m_currTime = 0;
	m_totTime = 0;
}

Sound::~Sound()
{
	m_fSound->release();
	m_fSound = 0;
}

void Sound::UpdateSound(float dt)
{
	if (m_soundState == SOUND_PLAY_STATE::LOADING && m_soundChunk._Is_ready())
	{
		m_fSound = static_cast<FMOD::Sound*>(m_soundChunk.get());
		m_soundState = SOUND_PLAY_STATE::READY;

		unsigned int tmp = 0;
		m_fSound->getLength(&tmp, FMOD_TIMEUNIT_MS);
		m_totTime = tmp;
	}
	else if (m_soundState == SOUND_PLAY_STATE::PLAYING && m_soundChunk._Is_ready())
	{
		m_currTime += dt;
		if (m_currTime >= m_totTime+100) // Give it some extra time
			m_soundState = SOUND_PLAY_STATE::FINISHED;
	}
}

FMOD::Sound * Sound::GetSound()
{
	return m_fSound;
}

Sound::SOUND_PLAY_STATE Sound::GetPlayState()
{
	return m_soundState;
}

void Sound::SetPlaying(SOUND_PLAY_STATE _value)
{
	m_soundState = _value;
}
