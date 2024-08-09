#pragma once

#include "Component/Component.h"

#include <cereal/types/polymorphic.hpp>
#include <SimpleMath.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "Component/ComponentCerealHelper.h"
#include "Component/ComponentRegister.h"

namespace TMF
{
	class Camera : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		boost::uuids::uuid OnGetUUID() override;

		DirectX::SimpleMath::Matrix GetProjectionMatrix();
		DirectX::SimpleMath::Matrix GetViewMatrix();

	private:
		DirectX::SimpleMath::Matrix MakeViewMatrix();
		std::string LabelChange(const char* labelName);
		float m_fov;
		float m_near;
		float m_far;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();

		SERIALIZE_COMPONENT(m_fov, m_near, m_far, m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Camera);
