#include "WeaponInfoUI.h"

#include "Imgui/imgui.h"

#include "GameObject/GameObject.h"
#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "WeaponBase.h"
#include "Font.h"

REGISTER_COMPONENT(TMF::WeaponInfoUI, "WeaponInfoUI");

namespace TMF
{
	void WeaponInfoUI::OnInitialize()
	{
		// 生成されていなければ生成する
		if (m_pWeaponBulletNumFont.expired() == true && m_pWeaponNameFont.expired() == true)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pFonts = pLockOwner->GetComponents<Font>();
				if (pFonts.size() >= 2)
				{
					if (auto pLockWeponFont = pFonts[0].lock())
					{
						m_pWeaponNameFont = pLockWeponFont;
					}
					if (auto pLockBulletNumFont = pFonts[1].lock())
					{
						m_pWeaponBulletNumFont = pLockBulletNumFont;
					}
				}
			}
		}
	}
	void WeaponInfoUI::OnFinalize()
	{
	}
	void WeaponInfoUI::OnUpdate()
	{
		if (auto pLockWepon = m_pWeaponNameFont.lock())
		{
			pLockWepon->SetText(m_weaponName);
		}
		if (auto pLockWepon = m_pWeapon.lock())
		{
			m_bulletNum = pLockWepon->GetBulletNum();
		}
		if (auto pLockWeponBulletNumFont = m_pWeaponBulletNumFont.lock())
		{
			// 近接武器が選択されているか
			if (m_bulletNum == -1)
			{
				pLockWeponBulletNumFont->SetText("");
			}
			else
			{
				auto bulletNumText = std::to_string(m_bulletNum) + "/" + std::to_string(m_maxBulletNum);
				pLockWeponBulletNumFont->SetText(bulletNumText);
			}
		}
	}
	void WeaponInfoUI::OnLateUpdate()
	{
	}
	void WeaponInfoUI::OnDraw()
	{
	}
	void WeaponInfoUI::OnDrawImGui()
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
	std::shared_ptr<Component> WeaponInfoUI::OnClone() const
	{
		auto pClone = std::make_shared<WeaponInfoUI>();
		return move(pClone);
	}
	void WeaponInfoUI::OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWepon)
	{
		m_maxBulletNum = 1;
		m_bulletNum = 0;
		if (auto pLockWepon = pWepon.lock())
		{
			// 武器の情報を取得
			m_pWeapon = pLockWepon;
			m_bulletNum = pLockWepon->GetBulletNum();
			m_maxBulletNum = pLockWepon->GetBulletMaxNum();
			// オブジェクトの名前を取得
			auto pWeponOwner = pLockWepon->GetOwner();
			if (auto pLockWeponOwner = pWeponOwner.lock())
			{
				m_weaponName = pLockWeponOwner->GetName();
			}
		}
	}
}