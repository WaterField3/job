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

		// �����̏����𖳌����[�v�����邩�w��񐔃��[�v�����邩

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
		// ���g��ێ����Ă���Q�[���I�u�W�F�N�g�����邩�m�F
		if (auto pLockOwner = m_pOwner.lock())
		{
			// �ێ����Ă��镐����N���A
			m_pWeapons.clear();
			// ���g�̎q�I�u�W�F�N�g���擾
			auto pChildren = pLockOwner->GetChildren();
			// �q�I�u�W�F�N�g�̐������[�v
			for (auto& pChild : pChildren)
			{
				// �q�I�u�W�F�N�g��Shared�ɕϊ��ł��邩�m�F
				if (auto pLockChild = pChild.lock())
				{
					// �q�I�u�W�F�N�g����WeponBase���擾����
					auto pWepon = pLockChild->GetComponent<WeaponBase>();
					// �擾����WeponBase��Shared�ɕϊ��ł���(���݂��Ă���)���m�F
					if (auto pLockWepon = pWepon.lock())
					{
						// ����Ƃ��Ċi�[
						m_pWeapons.push_back(pLockWepon);
					}
				}
			}
		}
	}
}