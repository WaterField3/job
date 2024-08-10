#include "GhostObject.h"
#include "Collider.h"

namespace TMF
{
	REGISTER_COMPONENT(GhostObject);

	void GhostObject::OnInitialize()
	{
		m_pGhostObject = std::make_shared<btGhostObject>();
		if (auto owner = m_pOwner.lock())
		{
			auto collider = owner->GetComponent<Collider>();
			if (auto col = collider.lock())
			{
				auto collisionShape = col->GetCollisionShape();
				if (auto shape = collisionShape.lock())
				{
					m_pGhostObject->setCollisionShape(shape.get());
					m_pGhostObject->setCollisionFlags(m_pGhostObject->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
				}
			}
		}
	}

	void GhostObject::OnFinalize()
	{
	}

	void GhostObject::OnUpdate()
	{
	}

	void GhostObject::OnLateUpdate()
	{
	}

	void GhostObject::OnDraw()
	{
	}

	void GhostObject::OnDrawImGui()
	{
	}

}