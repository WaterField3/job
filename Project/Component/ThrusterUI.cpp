#include "ThrusterUI.h"

#include <Imgui/imgui.h>
#include <WICTextureLoader.h>

#include "ComponentRegister.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Thruster.h"


REGISTER_COMPONENT(TMF::ThrusterUI, "ThrusterUI");

namespace TMF
{
	void ThrusterUI::OnInitialize()
	{
		auto pTargetObject = GameObjectManager::Instance().GetGameObject(m_targetName);
		if (auto pLockTargetObject = pTargetObject.lock())
		{
			m_pThruster = pLockTargetObject->GetComponent<Thruster>();
		}
		if (m_barTextureName != "")
		{
			auto wideString = std::wstring(m_barTextureName.begin(), m_barTextureName.end());
			DirectX::CreateWICTextureFromFile(D3D::Get()->GetDevice(), nullptr, wideString.c_str(), nullptr, m_pBarTexture.ReleaseAndGetAddressOf());
		}
	}
	void ThrusterUI::OnFinalize()
	{
	}
	void ThrusterUI::OnUpdate()
	{
	}
	void ThrusterUI::OnLateUpdate()
	{
	}
	void ThrusterUI::OnDraw()
	{
		auto pSpriteBatch = D3D::Get()->GetSpriteBatch();
		if (auto pLockSpriteBatch = pSpriteBatch.lock())
		{
			if (auto pLockThruster = m_pThruster.lock())
			{


				auto currentValue = pLockThruster->GetThrusterValue();
				auto maxValue = pLockThruster->GetMaxThrusterValue();

				if (currentValue == 0.0f)
				{
					return;
				}
				pLockSpriteBatch->Begin();

				if (currentValue < 0.0f)
				{
					currentValue = 0.0f;
				}
				if (maxValue <= 0.0f)
				{
					maxValue = 1.0f;
				}
				// RECTで描画範囲を指定
				RECT barBackRect = {};
				barBackRect.left = 0;
				barBackRect.top = 0;
				barBackRect.right = static_cast<LONG>(m_barWidth);
				barBackRect.bottom = static_cast<LONG>(m_barHeight);
				// 背景を描画
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barBackRect, DirectX::Colors::Gray); // 位置を指定


				// 現在値の割合を計算
				float percentage = currentValue / maxValue; // 値の割合
				if (percentage > 1.0f) percentage = 1.0f;  // 割合が1を超えないように制限

				// バーの幅を計算（最大幅に割合を掛ける）
				float barWidth = m_barWidth * percentage;

				auto color = DirectX::Colors::White;

				if (percentage > 0.7f)
				{
					color = DirectX::Colors::Orange;
				}
				else if (percentage > 0.3f)
				{
					color = DirectX::Colors::Yellow;
				}
				if (pLockThruster->GetIsOverHeat() == true)
				{
					color = DirectX::Colors::Red;
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
	}
	void ThrusterUI::OnDrawImGui()
	{
		char barTextureNameBuf[256] = "";
		strcpy_s(barTextureNameBuf, sizeof(barTextureNameBuf), m_barTextureName.c_str());
		auto barTextureNameLabel = StringHelper::CreateLabel("BarTextureName", m_uuID);
		if (ImGui::InputText(barTextureNameLabel.c_str(), barTextureNameBuf, 256))
		{
			m_barTextureName = barTextureNameBuf;
		}
		char targetNameBuf[256] = "";
		strcpy_s(targetNameBuf, sizeof(targetNameBuf), m_targetName.c_str());
		auto targetNameLabel = StringHelper::CreateLabel("TargetName", m_uuID);
		if (ImGui::InputText(targetNameLabel.c_str(), targetNameBuf, 256))
		{
			m_targetName = targetNameBuf;
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
	std::shared_ptr<Component> ThrusterUI::OnClone() const
	{
		auto pClone = std::make_shared<ThrusterUI>();
		pClone->m_barWidth = this->m_barWidth;
		pClone->m_barHeight = this->m_barHeight;
		pClone->m_barTextureName = this->m_barTextureName;
		pClone->m_targetName = this->m_targetName;
		pClone->m_drawPosition = this->m_drawPosition;
		return move(pClone);
	}
}