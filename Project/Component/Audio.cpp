#include "Audio.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "Utility/Log.h"
#include "AudioManager.h"

REGISTER_COMPONENT(TMF::Audio, "Audio");

namespace TMF
{
	Audio::Audio()
	{
	}

	Audio::~Audio()
	{
	}

	// �X�V�����Ă���V���O���g���̂悤�ɂȂ��Ă���̂ŕύX����

	void Audio::OnInitialize()
	{
		try
		{
			m_pAudioEngine = AudioManager::Instance().GetAudioEngine();
		}
		catch (const std::exception& e)
		{
			Log::Info("%s", e.what());
		}
		if (auto pLockAudioEngine = m_pAudioEngine.lock())
		{
			Load(m_soundName);
			auto wideStringSoundName = std::wstring(m_soundName.begin(), m_soundName.end());
			m_pSoundEffect = std::make_unique<DirectX::SoundEffect>(pLockAudioEngine.get(), wideStringSoundName.c_str());
			// �ŏ��ɂȂ炷�̂��ݒ肳��Ă��邩
			if (m_isStartPlay == true)
			{
				Play();
			}
		}

	}


	void Audio::OnFinalize()
	{

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
			if (m_pSoundEffectInstance)
			{
				m_pSoundEffectInstance->SetVolume(m_volume);
			}
		}
		auto pitchLabel = StringHelper::CreateLabel("Pitch", m_uuID);
		if (ImGui::DragFloat(pitchLabel.c_str(), &m_pitch, 0.1f, -1.0f, 1.0f))
		{
			if (m_pSoundEffectInstance)
			{
				m_pSoundEffectInstance->SetPitch(m_pitch);
			}
		}
		auto panLabel = StringHelper::CreateLabel("Pan", m_uuID);
		if (ImGui::DragFloat(panLabel.c_str(), &m_pan, 0.1f, -1.0f, 1.0f))
		{
			if (m_pSoundEffectInstance)
			{
				m_pSoundEffectInstance->SetPan(m_pan);
			}
		}
		auto isLoopLabel = StringHelper::CreateLabel("IsLoop", m_uuID);
		if (ImGui::Checkbox(isLoopLabel.c_str(), &m_isLoop))
		{

		}
		auto isStarPlayLabel = StringHelper::CreateLabel("IsStartPlay", m_uuID);
		if (ImGui::Checkbox(isStarPlayLabel.c_str(), &m_isStartPlay))
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

	std::shared_ptr<Component> Audio::OnClone() const
	{
		auto pClone = std::make_shared<Audio>();
		pClone->m_soundName = this->m_soundName;
		pClone->m_volume = this->m_volume;
		pClone->m_pitch = this->m_pitch;
		pClone->m_pan = this->m_pan;
		pClone->m_isLoop = this->m_isLoop;
		pClone->m_isStartPlay = this->m_isStartPlay;
		return move(pClone);
	}

	void Audio::Load(std::string soundName)
	{
		if (auto pLockAudioEngine = m_pAudioEngine.lock())
		{
			if (soundName != "")
			{
				m_soundName = soundName;
			}
			auto wideStringSoundName = std::wstring(m_soundName.begin(), m_soundName.end());
			m_pSoundEffect = std::make_unique<DirectX::SoundEffect>(pLockAudioEngine.get(), wideStringSoundName.c_str());
		}
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
}