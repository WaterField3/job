#pragma once

#include <memory>
#include <string>
#include <Audio.h>

namespace TMF
{
	class AudioManager
	{
	public:
		void Initialize();
		void Finalize();
		void Update();

		static AudioManager& Instance()
		{
			static AudioManager instance;
			return instance;
		}

		inline std::weak_ptr<DirectX::AudioEngine> GetAudioEngine() { return m_pAudioEngine; }

	private:
		std::shared_ptr<DirectX::AudioEngine> m_pAudioEngine;
	};
}

