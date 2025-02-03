#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
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
		std::shared_ptr<Component> OnClone() const override;
		void Move();

		inline float GetPlayerDistance() const { return m_playerDistance; }

	private:
		float m_playerDistance = 0.0f;
		float m_moveSpeed = 1.0f;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pPlayerTransform;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::weak_ptr<Rigidbody> m_pPlayerRigidbody;

		SERIALIZE_COMPONENT(m_moveSpeed);
	};
}

