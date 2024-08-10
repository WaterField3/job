#pragma once
#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <bullet/btBulletDynamicsCommon.h>

#include "Component/ComponentCerealHelper.h"
#include "Component/ComponentRegister.h"

namespace TMF
{
    class Rigidbody : public Component
    {
    public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		boost::uuids::uuid OnGetUUID() override;

		void SetRigidBodyTranform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua);
		void RemoveRigidBody();
		void AddRigidBody(std::weak_ptr<btCollisionShape> col);
		void AddRigidBody(std::weak_ptr<btCollisionShape> col, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua);
	private:
		btVector3 MakebtVector3(DirectX::SimpleMath::Vector3 vec);
		btQuaternion MakebtQuaternion(DirectX::SimpleMath::Quaternion qua);
		btTransform MakebtTransform(DirectX::SimpleMath::Vector3 vec, DirectX::SimpleMath::Quaternion qua);
		std::string LabelChange(const char* labelName);
		float m_mass = 0.0f;
		float m_drag = 0.0f;
		float m_angularDrag = 0.0f;
		bool m_useGravity = false;
		bool m_isKinematic = false;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		std::unique_ptr<btDefaultMotionState> m_pMotionState;
		std::shared_ptr<btRigidBody> m_pRigidBody;

		SERIALIZE_COMPONENT(m_mass, m_drag, m_angularDrag, m_useGravity, m_isKinematic, m_uuID);
    };
}
REGISTER_CEREAL_TYPE(TMF::Rigidbody);
