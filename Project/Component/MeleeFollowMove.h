#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class MeleeFollowMove : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

		float m_rotationOffset = 1.0f;
		DirectX::SimpleMath::Vector3 m_offsetPos = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pFollowTarget;

		SERIALIZE_COMPONENT(m_offsetPos, m_rotationOffset);
	};
}

