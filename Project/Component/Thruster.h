#pragma once

#include "Component.h"
#include "ComponentCerealHelper.h"
#include "MoveInfo.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class Thruster : public Component
	{
	public:
		Thruster();
		~Thruster();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void FastMovement(MoveDirection moveDirection);
		void StopFastMovement();
		void UseThruster(float useMagnification);
		inline bool GetIsOverHeat() const { return m_isOverHeat; }
	private:

		bool m_isThruster = false;
		bool m_isOverHeat = false;
		bool m_isDontUseThruster = false;
		float m_firstUseMagnification = 1.0f;
		float m_maxThrusterValue = 10.0f;
		float m_thrusterValue = 0.0;
		float m_thrusterMoveSpeedMagnification = 4.0f;
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		MoveDirection m_moveDirection = MoveDirection::FOWARD;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Rigidbody> m_pRigidBody;
		SERIALIZE_COMPONENT(m_thrusterValue, m_firstUseMagnification, m_thrusterMoveSpeedMagnification);
	};
}