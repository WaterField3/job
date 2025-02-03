#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <SimpleMath.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Transform;
	class Camera : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		DirectX::SimpleMath::Matrix GetProjectionMatrix();
		DirectX::SimpleMath::Matrix GetViewMatrix();
		inline void SetTargetTransform(std::weak_ptr<Transform> target) { m_pTargetTransform = target; }

	private:
		DirectX::SimpleMath::Matrix MakeViewMatrix();
		std::string LabelChange(const char* labelName);

		float m_fov = 45.0f;
		float m_near = 0.1f;
		float m_far = 1000.0f;
		std::weak_ptr<Transform> m_pTargetTransform;

		SERIALIZE_COMPONENT(m_fov, m_near, m_far);
	};
}
