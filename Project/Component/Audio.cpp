#include "Audio.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"

REGISTER_COMPONENT(TMF::Audio, "Audio");

namespace TMF
{
	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
	}

	void Audio::OnInitialize()
	{
		m_pAudioEngine = std::make_unique<DirectX::AudioEngine>(DirectX::AudioEngine_Default);
		m_pSoundEffect = std::make_unique<DirectX::SoundEffect>(m_pAudioEngine.get(), ChangeWideString().c_str());
	}

	void Audio::OnFinalize()
	{
		if (m_pAudioEngine)
		{
			m_pAudioEngine->Suspend();
		}
		if (m_pSoundEffectInstance)
		{
			auto state = m_pSoundEffectInstance->GetState();
			if (state != DirectX::STOPPED)
			{
				m_pSoundEffectInstance->Stop();
			}
		}
	}

	void Audio::OnUpdate()
	{
		m_pAudioEngine->Update();
	}

	void Audio::OnLateUpdate()
	{

	}

	void Audio::OnDraw()
	{

	}

	void Audio::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_soundName.c_str());
		auto soundNameLabel = StringHelper::CreateLabel("SoundName", m_uuID);
		if (ImGui::InputText(soundNameLabel.c_str(), buf, 256))
		{
			m_soundName = buf;
		}
		auto volumeLabel = StringHelper::CreateLabel("Volume", m_uuID);
		if (ImGui::DragFloat(volumeLabel.c_str(), &m_volume, 0.1f, 0.0f))
		{
			m_pSoundEffectInstance->SetVolume(m_volume);
		}
		auto pitchLabel = StringHelper::CreateLabel("Pitch", m_uuID);
		if (ImGui::DragFloat(pitchLabel.c_str(), &m_pitch, 0.1f, -1.0f, 1.0f))
		{
			m_pSoundEffectInstance->SetPitch(m_pitch);
		}
		auto panLabel = StringHelper::CreateLabel("Pan", m_uuID);
		if (ImGui::DragFloat(panLabel.c_str(), &m_pan, 0.1f, -1.0f, 1.0f))
		{
			m_pSoundEffectInstance->SetPan(m_pan);
		}
		auto isLoopLabel = StringHelper::CreateLabel("IsLoop", m_uuID);
		if (ImGui::Checkbox(isLoopLabel.c_str(), &m_isLoop))
		{

		}
		if (ImGui::Button("Play"))
		{
			Play();
		}
		if (ImGui::Button("Load"))
		{
			Load(m_soundName);
		}
		if (ImGui::Button("Stop"))
		{
			Stop();
		}
		if (ImGui::Button("Pause"))
		{
			Pause();
		}
		if (ImGui::Button("Resume"))
		{
			Resume();
		}

	}

	void Audio::Load(std::string soundName)
	{
		m_pSoundEffect = std::make_unique<DirectX::SoundEffect>(m_pAudioEngine.get(), ChangeWideString().c_str());
	}

	void Audio::Play()
	{
		m_pSoundEffectInstance = m_pSoundEffect->CreateInstance();
		m_pSoundEffectInstance->SetVolume(m_volume);
		m_pSoundEffectInstance->SetPitch(m_pitch);
		m_pSoundEffectInstance->SetPan(m_pan);
		m_pSoundEffectInstance->Play(m_isLoop);
	}

	void Audio::Stop()
	{
		if (m_pSoundEffectInstance)
		{
			m_pSoundEffectInstance->Stop(true);
		}
	}

	void Audio::Pause()
	{
		if (m_pSoundEffectInstance)
		{
			m_pSoundEffectInstance->Pause();
		}
	}

	void Audio::Resume()
	{
		if (m_pSoundEffectInstance)
		{
			m_pSoundEffectInstance->Resume();
		}
	}

	std::wstring Audio::ChangeWideString()
	{
		auto wideString = std::wstring(m_soundName.begin(), m_soundName.end());
		return wideString;
	}
}