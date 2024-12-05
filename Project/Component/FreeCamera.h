#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class FreeCamera : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		DirectX::SimpleMath::Matrix GetProjectionMatrix();
		DirectX::SimpleMath::Matrix GetViewMatrix();

		inline void SetTargetTransform(std::weak_ptr<Transform> target) { m_pTargetTransform = target; }


	private:

		DirectX::SimpleMath::Matrix MakeViewMatrix();

		int	 prevMouseX;
		int	 prevMouseY;
		float m_fov = 45.0f;
		float m_near = 0.1f;
		float m_far = 1000.0f;
		float m_raduis = 0.0f;
		float m_elevation = 0.0f;
		float m_azimuth = 0.0f;
		float m_rotationSpeed = 1.0f;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pTargetTransform;

		SERIALIZE_COMPONENT(m_fov, m_near, m_far, m_raduis, m_elevation, m_azimuth);

	};
}

