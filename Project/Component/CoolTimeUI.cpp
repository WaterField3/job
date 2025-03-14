#include "CoolTimeUI.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "WeaponBase.h"

REGISTER_COMPONENT(TMF::CoolTimeUI, "CoolTimeUI");

namespace TMF
{
	void CoolTimeUI::OnInitialize()
	{
		if (m_barTextureName != "")
		{
			auto wideString = std::wstring(m_barTextureName.begin(), m_barTextureName.end());
			DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), nullptr, wideString.c_str(), nullptr, m_pBarTexture.ReleaseAndGetAddressOf());
		}
	}
	void CoolTimeUI::OnFinalize()
	{

	}
	void CoolTimeUI::OnUpdate()
	{

	}
	void CoolTimeUI::OnLateUpdate()
	{

	}
	void CoolTimeUI::OnDraw()
	{
		auto pSpriteBatch = D3D::Get()->GetSpriteBatch();
		if (auto pLockSpriteBatch = pSpriteBatch.lock())
		{
			if (auto pLockWepon = m_pWeapon.lock())
			{
				auto currentValue = pLockWepon->GetCurrentCollTime();;
				auto maxValue = pLockWepon->GetCoolTime();

				if (currentValue == 0.0f)
				{
					return;
				}

				if (currentValue < 0.0f)
				{
					currentValue = 0.0f;
				}
				if (maxValue <= 0.0f)
				{
					maxValue = 1.0f;
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
				float percentage = currentValue / maxValue;
				// 割合が1を超えないように制限
				if (percentage > 1.0f)
				{
					percentage = 1.0f;
				}

				// バーの幅を計算（最大幅に割合を掛ける）
				float barWidth = m_barWidth * percentage;

				// RECTで描画範囲を指定
				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);
				barRect.bottom = static_cast<LONG>(m_barHeight);

				// バーの描画
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barRect, DirectX::Colors::Red);

				pLockSpriteBatch->End();

			}
		}
	}
	void CoolTimeUI::OnDrawImGui()
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
	std::shared_ptr<Component> CoolTimeUI::OnClone() const
	{
		auto pClone = std::make_shared<CoolTimeUI>();
		pClone->m_barWidth = this->m_barWidth;
		pClone->m_barHeight = this->m_barHeight;
		pClone->m_barTextureName = this->m_barTextureName;
		pClone->m_drawPosition = this->m_drawPosition;
		return move(pClone);
	}
	void CoolTimeUI::OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon)
	{
		m_pWeapon = pWeapon;
	}
}