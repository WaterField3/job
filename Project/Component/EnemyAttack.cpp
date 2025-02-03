#include "EnemyAttack.h"

#include <random>

#include "ComponentRegister.h"
#include "Shot.h"
#include "Melee.h"

REGISTER_COMPONENT(TMF::EnemyAttack, "EnemyAttack");

namespace TMF
{
	void EnemyAttack::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto index = 0;
			auto pShots = pLockOwner->GetComponents<Shot>();
			for (auto pShot : pShots)
			{
				m_pWepons.push_back(pShot);
				m_shotIndices.push_back(index);
				++index;
			}
			auto pMelees = pLockOwner->GetComponents<Melee>();
			for (auto pMelee : pMelees)
			{
				m_pWepons.push_back(pMelee);
			}
		}
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
		auto size = static_cast<int>(m_shotIndices.size());
		if (size > 0)
		{
			--size;
		}
		//std::random_device randomDevice;
		//std::mt19937 gen(randomDevice());

		//std::uniform_int_distribution<int> dist(0, size);

		//m_selectIndex = dist(gen);

	}

	void EnemyAttack::SelectMelee()
	{
		auto shotSize = static_cast<int>(m_shotIndices.size());
		auto weponSize = static_cast<int>(m_pWepons.size());
		if (weponSize < shotSize)
		{
			return;
		}
		if (shotSize > 0)
		{
			--shotSize;
		}
		if (weponSize > 0)
		{
			--weponSize;
		}

		std::random_device randomDevice;
		std::mt19937 gen(randomDevice());

		std::uniform_int_distribution<int> dist(shotSize, weponSize);

		m_selectIndex = dist(gen);
		if (auto pLockWepon = m_pWepons[m_selectIndex].lock())
		{
			if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockWepon))
			{
				pLockMelee->Select();
			}
		}
	}

	void EnemyAttack::Play()
	{
		if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
		{
			// Shotクラスに変換できるか確認
			if (auto pLockShot = std::dynamic_pointer_cast<Shot>(pLockSelectComponent))
			{
				pLockShot->Play();

			}
			// Meleeクラスに変換できるか確認
			//else if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockSelectComponent))
			//{
			//	pLockMelee->Play();
			//}
		}
	}

	bool EnemyAttack::GetIsMeleeEnd()
	{
		if (auto pLockSelectComponent = m_pWepons[m_selectIndex].lock())
		{
			if (auto pLockMelee = std::dynamic_pointer_cast<Melee>(pLockSelectComponent))
			{
				return pLockMelee->GetIsMeleeEnd();
			}
		}
		return false;
	}


}