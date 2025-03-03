#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class PlayerStatus;
	class CameraMove : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		inline void SetTargetRigidbody(std::weak_ptr<Rigidbody> rigidbody) { m_pTargetRigidbody = rigidbody; }

	private:
		// ëOâÒÇÃà íuÇï€éù
		int prevMouseX = 0;
		int prevMouseY = 0;
		float m_raduis = 0.0f;
		float m_elevation = 0.0f;
		float m_azimuth = 0.0f;
		float m_rotationSpeed = 1.0f;
		bool a = false;
		std::string m_targetName = "";
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pPlayerTransform;
		std::weak_ptr<Rigidbody> m_pTargetRigidbody;
		std::weak_ptr<PlayerStatus> m_pTargetPlayerStatus;

		SERIALIZE_COMPONENT(m_raduis, m_elevation, m_azimuth, m_rotationSpeed, m_targetName);

	};
}

