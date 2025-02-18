#include "WeaponBase.h"

#include "GameObject/GameObject.h"
#include "Transform.h"
#include "Attack.h"
#include "EnemyAttack.h"

namespace TMF
{
	void WeaponBase::OnInitialize()
	{
	}
	void WeaponBase::OnFinalize()
	{
	}
	void WeaponBase::OnUpdate()
	{
	}
	void WeaponBase::OnLateUpdate()
	{
	}
	void WeaponBase::OnDraw()
	{
	}
	void WeaponBase::OnDrawImGui()
	{
	}
	std::shared_ptr<Component> WeaponBase::OnClone() const
	{
		auto pClone = std::make_shared<WeaponBase>();
		return move(pClone);
	}
	void WeaponBase::Play()
	{
		OnAttack();
	}
	float WeaponBase::GetMeleeTime()
	{
		return 0.0f;
	}
	void WeaponBase::OnAttack()
	{

	}
	void WeaponBase::Select()
	{
		OnSelect();
	}
	void WeaponBase::OnSelect()
	{
	}
	void WeaponBase::OwnerWeponCheck()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto pParent = pLockTransform->GetParent();
				if (auto pLockParent = pParent.lock())
				{
					auto pParentOwenr = pLockParent->GetOwner();
					if (auto pLockParentOwner = pParentOwenr.lock())
					{
						auto pParentAttack = pLockParentOwner->GetComponent<Attack>();
						if (auto pLockParentAttack = pParentAttack.lock())
						{
							pLockParentAttack->WeaponsUpdate();
						}
						else
						{
							auto pParentEnemyAttack = pLockParentOwner->GetComponent<EnemyAttack>();
							if (auto pLockParentEnemyAttack = pParentEnemyAttack.lock())
							{
								pLockParentEnemyAttack->WeaponsUpdate();
							}
						}
					}
				}
			}
		}
	}
}