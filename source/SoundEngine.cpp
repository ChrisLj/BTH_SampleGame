#include "SoundEngine.h"
#include "PoolAllocatorInterface.h"

#define MAX_SOUNDS 32

extern ResourceManager gResourceManager;

SoundEngine::~SoundEngine()
{
}

bool SoundEngine::Init()
{
	FMOD_RESULT		result;
	unsigned int	version;
	void*			extradriverdata = 0;


	result = FMOD::System_Create(&m_system);
	if (result != FMOD_OK)
		return false;

	result = m_system->getVersion(&version);
	if (result != FMOD_OK || version < FMOD_VERSION)
		return false;

	result = m_system->init(MAX_SOUNDS, FMOD_INIT_NORMAL, extradriverdata);
	if (result != FMOD_OK)
		return false;

	//m_soundsHandle = InitializePoolAllocator(sizeof(Sound), MAX_SOUNDS, POOL_ALLOCATOR_DEFAULT_ALIGNMENT);
	m_channel = 0;
	m_currDelay = 0;
	m_delay = 2000;

	return true;
}

bool SoundEngine::Clean()
{
	FMOD_RESULT	result;

	for (int i = 0; i < m_sounds.size(); i++)
		delete m_sounds[i];

	m_sounds.clear();
	//	pDelete(m_soundsHandle, m_sounds[i]);

	//ShutdownPoolAllocator(m_soundsHandle);

	result = m_system->close();
	if (result != FMOD_OK)
		return false;

	result = m_system->release();
	if (result != FMOD_OK)
		return false;

	return true;
}

void SoundEngine::Update(float dt)
{
	m_system->update();

	// Remove sounds which are finished
	for (int i = m_sounds.size() - 1; i >= 0; i--)
	{
		if (m_sounds[i]->GetPlayState() == Sound::SOUND_PLAY_STATE::FINISHED)
		{
			delete m_sounds[i];
			m_sounds.erase(m_sounds.begin() + i);
		}
	}

	// Update sounds
	for (int i = 0; i < m_sounds.size(); i++)
		m_sounds[i]->UpdateSound(dt);


	for (int i = 0; i < m_sounds.size(); i++)
	{
		if (m_sounds[i]->GetPlayState() != Sound::SOUND_PLAY_STATE::READY)
			continue;

			m_sounds[i]->SetPlaying(Sound::SOUND_PLAY_STATE::PLAYING);
			m_system->playSound(m_sounds[i]->GetSound(), 0, false, &m_channel);
	}

	m_currDelay += dt;
}

void SoundEngine::AddSound(const char * _filePath)
{
	if (m_currDelay >= m_delay)
	{
		m_sounds.push_back(new Sound(_filePath, m_system));
		m_currDelay = 0.f;
	}
}
