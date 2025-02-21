#include "HPUI.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "direct3d.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObject.h"
#include "GameObject/GameObjectManager.h"
#include "ComponentRegister.h"
#include "PlayerStatus.h"
#include "Font.h"

REGISTER_COMPONENT(TMF::HPUI, "HPUI");

namespace TMF
{
	void HPUI::OnInitialize()
	{
		auto pTarget = GameObjectManager::Instance().GetGameObject(m_targetName);
		if (auto pLockpTarget = pTarget.lock())
		{
			auto pPlayerStatus = pLockpTarget->GetComponent<PlayerStatus>();
			if (auto pLockPlayerStatus = pPlayerStatus.lock())
			{
				m_maxHP = pLockPlayerStatus->GetHP();
				m_pPlayerStatus = pLockPlayerStatus;
			}
		}
		if (m_isFontDraw == true)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pFont = pLockOwner->GetComponent<Font>();
				if (auto pLockFont = pFont.lock())
				{
					m_pFont = pLockFont;
				}
			}
		}
		if (m_barTextureName != "")
		{
			auto wideString = std::wstring(m_barTextureName.begin(), m_barTextureName.end());
			DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), nullptr, wideString.c_str(), nullptr, m_pBarTexture.ReleaseAndGetAddressOf());
		}
	}
	void HPUI::OnFinalize()
	{
	}
	void HPUI::OnUpdate()
	{
	}
	void HPUI::OnLateUpdate()
	{
	}
	void HPUI::OnDraw()
	{
		auto pSpriteBatch = D3D::Get()->GetSpriteBatch();
		auto currentValue = 1.0f;
		if (auto pLockSpriteBatch = pSpriteBatch.lock())
		{
			if (auto pLockPlayerStatus = m_pPlayerStatus.lock())
			{
				currentValue = pLockPlayerStatus->GetHP();

				if (m_pBarTexture == nullptr)
				{
					return;
				}

				pLockSpriteBatch->Begin();
				// RECTで描画範囲を指定
				RECT barBackRect = {};
				barBackRect.left = 0;
				barBackRect.top = 0;
				barBackRect.right = static_cast<LONG>(m_barWidth);
				barBackRect.bottom = static_cast<LONG>(m_barHeight);
				// 背景を描画
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barBackRect, DirectX::Colors::Gray); // 位置を指定


				// 現在値の割合を計算
				float percentage = currentValue / m_maxHP; // 値の割合
				if (percentage > 1.0f)
				{
					percentage = 1.0f;  // 割合が1を超えないように制限
				}

				// バーの幅を計算（最大幅に割合を掛ける）
				float barWidth = m_barWidth * percentage;

				auto color = DirectX::Colors::Green;

				if (percentage < 0.1f)
				{
					color = DirectX::Colors::Red;
				}
				else if (percentage < 0.3f)
				{
					color = DirectX::Colors::Orange;
				}
				else if (percentage < 0.7f)
				{
					color = DirectX::Colors::Yellow;
				}

				// RECTで描画範囲を指定
				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);
				barRect.bottom = static_cast<LONG>(m_barHeight);

				// バーの描画
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barRect, color);

				pLockSpriteBatch->End();
			}
		}
		if (auto pLockFont = m_pFont.lock())
		{
			auto text = std::to_string((int)m_maxHP) + "/" + std::to_string((int)currentValue);
			pLockFont->SetText(text);
		}
	}
	void HPUI::OnDrawImGui()
	{
		char targetNameBuf[256] = "";
		strcpy_s(targetNameBuf, sizeof(targetNameBuf), m_targetName.c_str());
		auto targetNameLabel = StringHelper::CreateLabel("TargetName", m_uuID);
		if (ImGui::InputText(targetNameLabel.c_str(), targetNameBuf, 256))
		{
			m_targetName = targetNameBuf;
		}

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
		auto isFontDrawLabel = StringHelper::CreateLabel("IsFontDraw", m_uuID);
		if (ImGui::Checkbox(isFontDrawLabel.c_str(), &m_isFontDraw))
		{

		}
	}
	std::shared_ptr<Component> HPUI::OnClone() const
	{
		auto pClone = std::make_shared<HPUI>();
		return pClone;
	}
}