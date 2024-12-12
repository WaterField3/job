#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Rigidbody;
	class EnemyMove : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void Move();
	private:
		float m_moveSpeed = 1.0f;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Rigidbody> m_pPlayerRigidbody;

		SERIALIZE_COMPONENT(m_moveSpeed);
	};
}

