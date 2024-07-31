#pragma once
#include "Component/Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>

#include "SimpleMath.h"
#include "Component/ComponentCerealHelper.h"
#include "Component/ComponentRegister.h"

namespace TMF
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void SetPosition(DirectX::SimpleMath::Vector3 pos);
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		void SetRotation(DirectX::SimpleMath::Vector3 rotation);
		DirectX::SimpleMath::Matrix GetMatrixLocal();
		DirectX::SimpleMath::Matrix GetMatrixRotation();

	private:
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_scale;
		DirectX::SimpleMath::Vector3 m_rotation;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();

		SERIALIZE_COMPONENT(m_position, m_scale, m_rotation, m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Transform);
