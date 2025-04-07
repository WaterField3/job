#pragma once
#include "Component.h"
#include "ComponentCerealHelper.h"
#include "MoveInfo.h"

namespace TMF
{
	class Transform;
	class Camera;
	class CameraLook : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

	private:
		std::string m_targetName = "";
		DirectX::SimpleMath::Vector3 m_offsetPos = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pLockTarget;
		std::weak_ptr<Camera> m_pCamera;
		MOVEDIRECTION m_moveDirection;

		SERIALIZE_COMPONENT(m_offsetPos, m_moveDirection, m_targetName);
	};
}

