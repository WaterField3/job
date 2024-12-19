#include "CoolTimeUI.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Shot.h"
#include "Melee.h"

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
			if (auto pLockWepon = m_pWepon.lock())
			{
				auto currentValue = 10.0f;
				auto maxValue = 10.0f;
				if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockWepon))
				{
					currentValue = pLockMelee->GetCurrentCollTime();
					maxValue = pLockMelee->GetCoolTime();
				}
				else if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockWepon))
				{
					currentValue = pLockShot->GetCurrentCollTime();
					maxValue = pLockShot->GetCoolTime();
				}
				pLockSpriteBatch->Begin();
				// 背景を描画
				//pLockSpriteBatch->Draw(backgroundTexture.Get(), DirectX::XMFLOAT2(50, 50)); // 位置を指定
				//pLockSpriteBatch->Draw(m_pBarTexture.Get(), DirectX::XMFLOAT2(50, 50));

				if (currentValue < 0.0f) currentValue = 0.0f;
				if (maxValue <= 0.0f) maxValue = 1.0f; // maxValueが0以下の場合を回避

				// 現在値の割合を計算
				float percentage = currentValue / maxValue; // 値の割合
				if (percentage > 1.0f) percentage = 1.0f;  // 割合が1を超えないように制限

				// バーの幅を計算（最大幅に割合を掛ける）
				float barWidth = m_barWidth * percentage;

				// RECTで描画範囲を指定
				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);
				barRect.bottom = static_cast<LONG>(m_barHeight);

				// バーの描画
				pLockSpriteBatch->Draw(
					m_pBarTexture.Get(),          // テクスチャ
					m_drawPosition, // 描画位置
					&barRect                    // 描画範囲
				);

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
	void CoolTimeUI::SetSelectWepon(std::weak_ptr<Component> pWepon)
	{
		m_pWepon = pWepon;
	}
}