#include "ChangeTimeUI.h"

#include <WICTextureLoader.h>
#include <Imgui/imgui.h>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "direct3d.h"
#include "Shot.h"
#include "Melee.h"

REGISTER_COMPONENT(TMF::ChangeTimeUI, "ChangeTimeUI");

namespace TMF
{
	void ChangeTimeUI::OnInitialize()
	{
		if (m_barTextureName != "")
		{
			auto wideString = std::wstring(m_barTextureName.begin(), m_barTextureName.end());
			DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), nullptr, wideString.c_str(), nullptr, m_pBarTexture.ReleaseAndGetAddressOf());
		}
	}
	void ChangeTimeUI::OnFinalize()
	{

	}
	void ChangeTimeUI::OnUpdate()
	{

	}
	void ChangeTimeUI::OnLateUpdate()
	{

	}
	void ChangeTimeUI::OnDraw()
	{
		auto pSpriteBatch = D3D::Get()->GetSpriteBatch();
		if (auto pLockSpriteBatch = pSpriteBatch.lock())
		{
			if (auto pLockWepon = m_pWepon.lock())
			{
				auto currentValue = 10.0f;
				auto maxValue = 10.0f;
				if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockWepon))
				{
					currentValue = pLockMelee->GetCurrentChangeTime();
					maxValue = pLockMelee->GetChangeTime();
				}
				else if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockWepon))
				{
					currentValue = pLockShot->GetCurrentChangeTime();
					maxValue = pLockShot->GetChangeTime();
				}


				if (currentValue < 0.0f)
				{
					currentValue = 0.0f;
				}
				if (maxValue <= 0.0f)
				{
					maxValue = 1.0f;
				}

				if (currentValue == 0.0f)
				{
					return;
				}

				// ���ݒl�̊������v�Z
				float percentage = currentValue / maxValue; // �l�̊���
				if (percentage > 1.0f)
				{
					percentage = 1.0f;  // ������1�𒴂��Ȃ��悤�ɐ���
					return;
				}
				pLockSpriteBatch->Begin();

				// RECT�ŕ`��͈͂��w��
				RECT barBackRect = {};
				barBackRect.left = 0;
				barBackRect.top = 0;
				barBackRect.right = static_cast<LONG>(m_barWidth);
				barBackRect.bottom = static_cast<LONG>(m_barHeight);
				// �w�i��`��
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barBackRect, DirectX::Colors::Gray); // �ʒu���w��

				// �o�[�̕����v�Z�i�ő啝�Ɋ������|����j
				float barWidth = m_barWidth * percentage;

				// RECT�ŕ`��͈͂��w��
				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);
				barRect.bottom = static_cast<LONG>(m_barHeight);

				// �o�[�̕`��
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barRect, DirectX::Colors::Red);

				pLockSpriteBatch->End();

			}
		}
	}
	void ChangeTimeUI::OnDrawImGui()
	{
		char barTextureNameBuf[256] = "";
		strcpy_s(barTextureNameBuf, sizeof(barTextureNameBuf), m_barTextureName.c_str());
		auto barTextureNameLabel = StringHelper::CreateLabel("BarTextureName", m_uuID);
		if (ImGui::InputText(barTextureNameLabel.c_str(), barTextureNameBuf, 256))
		{
			m_barTextureName = barTextureNameBuf;
		}
		auto drawPositionLabel = StringHelper::CreateLabel("DrawPosition", m_uuID);
		if (ImGui::DragFloat2(drawPositionLabel.c_str(), &m_drawPosition.x))
		{

		}
		auto barWidthLabel = StringHelper::CreateLabel("BarWidth", m_uuID);
		if (ImGui::DragFloat(barWidthLabel.c_str(), &m_barWidth))
		{

		}
		auto barHeightLabel = StringHelper::CreateLabel("BarHeight", m_uuID);
		if (ImGui::DragFloat(barHeightLabel.c_str(), &m_barHeight))
		{

		}
	}
	void ChangeTimeUI::SetSelectWepon(std::weak_ptr<Component> pWepon)
	{
		m_pWepon = pWepon;
	}
}