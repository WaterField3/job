#include "WeponBase.h"

#include "GameObject/GameObject.h"
#include "Transform.h"
#include "Attack.h"

namespace TMF
{
	void WeponBase::OnInitialize()
	{
	}
	void WeponBase::OnFinalize()
	{
	}
	void WeponBase::OnUpdate()
	{
	}
	void WeponBase::OnLateUpdate()
	{
	}
	void WeponBase::OnDraw()
	{
	}
	void WeponBase::OnDrawImGui()
	{
	}
	std::shared_ptr<Component> WeponBase::OnClone() const
	{
		auto pClone = std::make_shared<WeponBase>();
		return move(pClone);
	}
	void WeponBase::Play()
	{
		OnAttack();
	}
	void WeponBase::OnAttack()
	{

	}
	void WeponBase::Select()
	{
		OnSelect();
	}
	void WeponBase::OnSelect()
	{
	}
	void WeponBase::OwnerWeponCheck()
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
							pLockParentAttack->WeponsUpdate();
						}
					}
				}
			}
		}
	}
}