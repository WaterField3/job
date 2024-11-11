#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class BulletStraightMove : public Component
	{
	private:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	public:
		void MoveStart(DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation);
		inline void SetMoveVector(DirectX::SimpleMath::Vector3 set) { m_moveVector = set; }
		inline void SetEndTime(float setTime) { m_endTime = setTime; }
		inline void ResetTimer() { m_timer = 0.0f; }

	private:
		float m_moveSpeed = 0.0f;
		float m_damage = 1.0f;
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pParent;
		float m_timer = 0;
		float m_endTime = 1.0f;

		SERIALIZE_COMPONENT(m_moveSpeed, m_moveVector, m_endTime);
	};
}

