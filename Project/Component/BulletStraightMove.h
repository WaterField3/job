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
		inline void SetMoveVector(DirectX::SimpleMath::Vector3 set) { m_moveVector = set; }

	private:
		float m_moveSpeed = 0.0f;
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_transform;
	};
}

