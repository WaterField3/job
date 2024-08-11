#pragma once
#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <SimpleMath.h>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

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
		void OnCollisionEnter() override;
		void OnCollisionStay() override;
		void OnCollisionExit() override;
		void OnTrigerEnter() override;
		void OnTrigerStay() override;
		void OnTrigerExit() override;
		boost::uuids::uuid OnGetUUID() override;
		void SetPosition(DirectX::SimpleMath::Vector3 pos);
		void SetRotation(DirectX::SimpleMath::Quaternion rotation);
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		DirectX::SimpleMath::Matrix GetMatrixLocal();
		DirectX::SimpleMath::Matrix GetMatrixRotation();
		void ChangeRigidBodyTransform();
		void ChangeGhostObjectTransform();
		void UpDateCollisionObjectTransform();

		inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
		inline DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
		inline DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }

	private:
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_scale;
		DirectX::SimpleMath::Quaternion m_rotation = DirectX::SimpleMath::Quaternion::Identity;
		DirectX::SimpleMath::Vector3 m_editorRotation;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();

		SERIALIZE_COMPONENT(m_position, m_scale, m_rotation, m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Transform);
