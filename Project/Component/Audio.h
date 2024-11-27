#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <SimpleMath.h>
#include <Audio.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Audio : public Component
	{
	public:
		Audio();
		~Audio();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Load(std::string soundName);
		void Play();
		void Stop();
		void Pause();
		void Resume();

	private:
		std::wstring ChangeWideString();
		std::weak_ptr<DirectX::AudioEngine> m_pAudioEngine;
		std::unique_ptr<DirectX::SoundEffect> m_pSoundEffect;
		std::unique_ptr<DirectX::SoundEffectInstance> m_pSoundEffectInstance;
		std::string m_soundName = "C:/Users/temet/Downloads/maou_game_battle21.wav";
		float m_volume = 1;
		float m_pitch = 0;
		float m_pan = 0;
		bool m_isLoop = false;
		SERIALIZE_COMPONENT(m_soundName, m_volume, m_pitch, m_pan, m_isLoop);

	};
}
