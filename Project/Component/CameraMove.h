#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class CameraMove : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		// ëOâÒÇÃà íuÇï€éù
		int prevMouseX = 0;
		int prevMouseY = 0;
		float m_raduis = 0.0f;
		float m_elevation = 0.0f;
		float m_azimuth = 0.0f;
		float m_rotationSpeed = 1.0f;
		std::string m_targetName = "";
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pPlayerTransform;

		SERIALIZE_COMPONENT(m_raduis, m_elevation, m_azimuth, m_rotationSpeed, m_targetName);

	};
}

