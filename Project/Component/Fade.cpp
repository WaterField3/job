#include "Fade.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Utility/Log.h"
#include "direct3d.h"
#include "Timer.h"

REGISTER_COMPONENT(TMF::Fade, "Fade");

namespace TMF
{
	void Fade::OnInitialize()
	{
		m_pSpriteBatch = std::make_unique<DirectX::SpriteBatch>(D3D::Get()->GetContext());
		// �ǂݍ��ݎ��s
		auto context = D3D::Get()->GetContext();
		DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), context, L"asset/textures/sunset.jpg", nullptr, m_pFadeTexture.ReleaseAndGetAddressOf());
		context->PSSetShaderResources(0, 1, m_pFadeTexture.GetAddressOf());
	}
	void Fade::OnFinalize()
	{
	}
	void Fade::OnUpdate()
	{
		if (m_isFade == true)
		{
			auto deltaTime = Timer::Instance().deltaTime.count();
			switch (m_fadeType)
			{
			case TMF::Fade::FADEIN:
				// �A���t�@�l�����X�ɕύX (�t�F�[�h�C��)
				m_fadeAlpha +=  deltaTime * 0.5f;
				m_fadeAlpha = std::clamp(m_fadeAlpha, 0.0f, 1.0f); // 0�`1�͈̔͂ɐ���
				break;
			case TMF::Fade::FADEOUT:
				// �A���t�@�l�����X�ɕύX (�t�F�[�h�A�E�g)
				m_fadeAlpha -= deltaTime * 0.5f;
				m_fadeAlpha = std::clamp(m_fadeAlpha, 0.0f, 1.0f); // 0�`1�͈̔͂ɐ���
				break;
			default:
				break;
			}
			if (m_fadeAlpha == 1.0f || m_fadeAlpha == 0.0f)
			{
				m_isFade = false;
				if (m_fadeType == FadeType::FADEIN)
				{
					m_isFadeInEnd = true;
					m_fadeType = FADEOUT;
				}
				else
				{
					m_isFadeOutEnd = true;
					m_fadeType = FADEIN;
				}
			}

		}
	}
	void Fade::OnLateUpdate()
	{
	}
	void Fade::OnDraw()
	{
		auto pSpriteBatch = D3D::Get()->GetSpriteBatch();
		if (auto pLockSpriteBatch = pSpriteBatch.lock())
		{
			DirectX::SimpleMath::Vector4 color(0, 0, 0, m_fadeAlpha); // RGBA
			pLockSpriteBatch->Begin();
			pLockSpriteBatch->Draw(m_pFadeTexture.Get(), D3D::Get()->GetFullScreenRect(), color);
			pLockSpriteBatch->End();
		}



	}
	void Fade::OnDrawImGui()
	{
		// �t�F�[�h�̑��x
		auto fadeSpeedLabel = StringHelper::CreateLabel("FadeSpeed", m_uuID);
		if (ImGui::DragFloat(fadeSpeedLabel.c_str(), &m_fadeSpeed))
		{

		}
		// �t�F�[�h�C�����t�F�[�h�A�E�g���I��
		auto fadeTypeLabel = StringHelper::CreateLabel("FadeType", m_uuID);
		const char* types[] = { "FadeIn","FadeOut" };
		int selectIndex = (int)m_fadeType;
		if (ImGui::BeginCombo(fadeTypeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_fadeType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_fadeType = FadeType(i);
					selectIndex = i;
				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}
		// �ŏ�����t�F�[�h���n�߂邩
		auto isFadeLabel = StringHelper::CreateLabel("IsFade", m_uuID);
		if (ImGui::Checkbox(isFadeLabel.c_str(), &m_isFade))
		{

		}
	}
	void Fade::FadeStart()
	{
		m_isFade = true;
	}
}