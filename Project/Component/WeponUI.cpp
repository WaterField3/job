#include "WeponUI.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Font.h"
#include "WeponBase.h"

REGISTER_COMPONENT(TMF::WeponUI, "WeponUI");

namespace TMF
{
	void WeponUI::OnInitialize()
	{
		// 生成されていなければ生成する
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
			m_bulletNum = pLockWepon->GetBulletNum();
		}
		if (auto pLockWeponBulletNumFont = m_pWeponBulletNumFont.lock())
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

	void WeponUI::OnLateUpdate()
	{
	}

	void WeponUI::OnDraw()
	{
	}

	void WeponUI::OnDrawImGui()
	{
	}

	std::shared_ptr<Component> WeponUI::OnClone() const
	{
		auto pClone = std::make_shared<WeponUI>();
		return move(pClone);
	}

	void WeponUI::SetSelectWepon(std::weak_ptr<WeponBase> pWepon)
	{
		m_maxBulletNum = 1;
		m_bulletNum = 0;
		if (auto pLockWepon = pWepon.lock())
		{
			// 武器の情報を取得
			m_pWepon = pLockWepon;
			pLockWepon->GetBulletMaxNum();
			pLockWepon->GetBulletNum();
			m_bulletNum = pLockWepon->GetBulletNum();
			m_maxBulletNum = pLockWepon->GetBulletMaxNum();
			// オブジェクトの名前を取得
			auto pWeponOwner = pLockWepon->GetOwner();
			if (auto pLockWeponOwner = pWeponOwner.lock())
			{
				m_weponName = pLockWeponOwner->GetName();
			}
		}
	}
}