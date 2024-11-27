#include "AudioManager.h"

namespace TMF
{
	void AudioManager::Initialize()
	{
		m_pAudioEngine = std::make_shared<DirectX::AudioEngine>();
	}
	void AudioManager::Finalize()
	{
		if (m_pAudioEngine)
		{
			m_pAudioEngine->Suspend();
		}
	}
	void AudioManager::Update()
	{
		if (m_pAudioEngine)
		{
			m_pAudioEngine->Update();
		}
	}
}