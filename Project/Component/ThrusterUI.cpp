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

				pLockSpriteBatch->Begin();
				// 背景を描画
				//pLockSpriteBatch->Draw(backgroundTexture.Get(), DirectX::XMFLOAT2(50, 50)); // 位置を指定
				//pLockSpriteBatch->Draw(m_pBarTexture.Get(), DirectX::XMFLOAT2(50, 50));

				// バー部分を描画
				float percentage = pLockThruster->GetMaxThrusterValue() - pLockThruster->GetThrusterValue();
				auto barWidth = m_barWidth * percentage; // 例: 最大幅 200px

				// 切り取った矩形を指定してバーを描画
				//auto a = DirectX::SimpleMath::Rectangle(0, 0, static_cast<int>(barWidth), static_cast<int>(barHeight));
				//pLockSpriteBatch->Draw(m_pBarTexture.Get(), DirectX::XMFLOAT2(55, 55)); // 幅を指定

				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);  // 現在の幅
				barRect.bottom = static_cast<LONG>(m_barHeight);

				pLockSpriteBatch->Draw(
					m_pBarTexture.Get(),          // 描画するテクスチャ
					m_drawPosition, // 描画位置 (左上)
					&barRect                    // 描画する範囲
				);

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
}