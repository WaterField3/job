#include "WeponUI.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Font.h"
#include "Melee.h"
#include "Shot.h"

REGISTER_COMPONENT(TMF::WeponUI, "WeponUI");

namespace TMF
{
	void WeponUI::OnInitialize()
	{
		// ¶¬‚³‚ê‚Ä‚¢‚È‚¯‚ê‚Î¶¬‚·‚é
		if (m_pWeponBulletNumFont.expired() == true && m_pWeponNameFont.expired() == true)
		{
			if (auto pLockOwner = m_pOwner.lock())
			{
				auto pFonts = pLockOwner->GetComponents<Font>();
				if (pFonts.size() >= 2)
				{
					if (auto pLockWeponFont = pFonts[0].lock())
					{
						m_pWeponNameFont = pLockWeponFont;
					}
					if (auto pLockBulletNumFont = pFonts[1].lock())
					{
						m_pWeponBulletNumFont = pLockBulletNumFont;
					}
				}
			}
		}
	}
	void WeponUI::OnFinalize()
	{
	}

	void WeponUI::OnUpdate()
	{
		if (auto pLockWepon = m_pWeponNameFont.lock())
		{
			pLockWepon->SetText(m_weponName);
		}
		if (auto pLockWepon = m_pWepon.lock())
		{
			if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockWepon))
			{
				m_bulletNum = pLockShot->GetBulletNum();
			}
		}
		if (auto pLockWeponBulletNumFont = m_pWeponBulletNumFont.lock())
		{
			// ‹ßÚ•Ší‚ª‘I‘ð‚³‚ê‚Ä‚¢‚é‚©
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

	void WeponUI::OnLateUpdate()
	{
	}

	void WeponUI::OnDraw()
	{
	}

	void WeponUI::OnDrawImGui()
	{
	}

	void WeponUI::SetSelectWepon(std::weak_ptr<Component> pWepon)
	{
		m_maxBulletNum = 1;
		m_bulletNum = 0;
		if (auto pLockWepon = pWepon.lock())
		{
			m_pWepon = pLockWepon;
			if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockWepon))
			{
				m_weponName = pLockMelee->GetMeleeObjectName();
				m_bulletNum = -1;
			}
			else if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockWepon))
			{
				m_weponName = pLockShot->GetShotObjectName();
				m_bulletNum = pLockShot->GetBulletNum();
				m_maxBulletNum = pLockShot->GetBulletMaxNum();
			}
		}
	}
}