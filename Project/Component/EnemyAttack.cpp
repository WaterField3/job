#include "EnemyAttack.h"

#include <random>

#include "ComponentRegister.h"
#include "WeaponBase.h"
#include "Shot.h"
#include "Melee.h"

REGISTER_COMPONENT(TMF::EnemyAttack, "EnemyAttack");

namespace TMF
{
	void EnemyAttack::OnInitialize()
	{
		CheckWepons();
	}
	void EnemyAttack::OnFinalize()
	{

	}
	void EnemyAttack::OnUpdate()
	{

	}
	void EnemyAttack::OnLateUpdate()
	{

	}
	void EnemyAttack::OnDraw()
	{

	}
	void EnemyAttack::OnDrawImGui()
	{

	}

	std::shared_ptr<Component> EnemyAttack::OnClone() const
	{
		auto pClone = std::make_shared<EnemyAttack>();
		return move(pClone);
	}

	void EnemyAttack::SelectShot()
	{
		auto weponSize = static_cast<int>(m_pWeapons.size());
		if (weponSize <= 0)
		{
			return;
		}

		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());

		// ここの処理を無限ループさせるか指定回数ループさせるか

		for (auto i = 0; i < weponSize; i++)
		{
			std::uniform_int_distribution<int> dist(0, weponSize - 1);
			m_selectIndex = dist(gen);
			if (auto pLockWeapon = m_pWeapons[m_selectIndex].lock())
			{
				if (pLockWeapon->GetWeaponType() == WeaponBase::SHOT)
				{
					pLockWeapon->Select();
					break;
				}
			}
		}

	}

	void EnemyAttack::SelectMelee()
	{
		auto weponSize = static_cast<int>(m_pWeapons.size());
		if (weponSize <= 0)
		{
			return;
		}

		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());

		for (auto i = 0; i < weponSize; i++)
		{
			std::uniform_int_distribution<int> dist(0, weponSize - 1);
			m_selectIndex = dist(gen);
			if (auto pLockWeapon = m_pWeapons[m_selectIndex].lock())
			{
				if (pLockWeapon->GetWeaponType() == WeaponBase::MELEE)
				{
					pLockWeapon->Select();
					break;
				}
			}
		}
	}

	bool EnemyAttack::Play()
	{
		auto weponSize = static_cast<int>(m_pWeapons.size());
		if (weponSize <= 0)
		{
			return false;
		}
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			return pLockSelectComponent->Play();
		}
		return false;
	}

	void EnemyAttack::WeaponsUpdate()
	{
		CheckWepons();
	}

	bool EnemyAttack::GetIsMeleeEnd()
	{
		auto weponSize = static_cast<int>(m_pWeapons.size());
		if (weponSize <= 0)
		{
			return false;
		}
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			if (pLockSelectComponent->GetWeaponType() == WeaponBase::MELEE)
			{
				return pLockSelectComponent->GetIsMeleeEnd();
			}
		}
		return false;
	}

	float EnemyAttack::GetEndTime()
	{
		if (auto pLockSelectComponent = m_pWeapons[m_selectIndex].lock())
		{
			return pLockSelectComponent->GetEndTime();
		}
		return 0.0f;
	}

	void EnemyAttack::CheckWepons()
	{
		// 自身を保持しているゲームオブジェクトがあるか確認
		if (auto pLockOwner = m_pOwner.lock())
		{
			// 保持している武器をクリア
			m_pWeapons.clear();
			// 自身の子オブジェクトを取得
			auto pChildren = pLockOwner->GetChildren();
			// 子オブジェクトの数分ループ
			for (auto& pChild : pChildren)
			{
				// 子オブジェクトがSharedに変換できるか確認
				if (auto pLockChild = pChild.lock())
				{
					// 子オブジェクトからWeponBaseを取得する
					auto pWepon = pLockChild->GetComponent<WeaponBase>();
					// 取得したWeponBaseがSharedに変換できる(存在している)か確認
					if (auto pLockWepon = pWepon.lock())
					{
						// 武器として格納
						m_pWeapons.push_back(pLockWepon);
					}
				}
			}
		}
	}
}