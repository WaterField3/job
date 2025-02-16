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

				// RECT‚Å•`‰æ”ÍˆÍ‚ğw’è
				RECT barBackRect = {};
				barBackRect.left = 0;
				barBackRect.top = 0;
				barBackRect.right = static_cast<LONG>(m_barWidth);
				barBackRect.bottom = static_cast<LONG>(m_barHeight);
				// ”wŒi‚ğ•`‰æ
				pLockSpriteBatch->Draw(m_pBarTexture.Get(), m_drawPosition, &barBackRect, DirectX::Colors::Gray); // ˆÊ’u‚ğw’è

				// Œ»İ’l‚ÌŠ„‡‚ğŒvZ
				float percentage = currentValue / maxValue;
				// Š„‡‚ª1‚ğ’´‚¦‚È‚¢‚æ‚¤‚É§ŒÀ
				if (percentage > 1.0f)
				{
					percentage = 1.0f;
				}

				// ƒo[‚Ì•‚ğŒvZiÅ‘å•‚ÉŠ„‡‚ğŠ|‚¯‚éj
				float barWidth = m_barWidth * percentage;

				// RECT‚Å•`‰æ”ÍˆÍ‚ğw’è
				RECT barRect = {};
				barRect.left = 0;
				barRect.top = 0;
				barRect.right = static_cast<LONG>(barWidth);
				barRect.bottom = static_cast<LONG>(m_barHeight);

				// ƒo[‚Ì•`‰æ
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
	void CoolTimeUI::OnSetSelectWepon(std::weak_ptr<WeaponBase> pWepon)
	{
		m_pWeapon = pWepon;
	}
}