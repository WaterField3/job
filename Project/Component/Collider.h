#pragma once
#include "Component.h"

#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <SimpleMath.h>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Collider : public Component
	{
	public:
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
		void MakeCollision();
		std::weak_ptr<btCollisionShape> GetCollisionShape();
	private:
		enum class Collider_Type
		{
			BOX,
			CAPSULE,
			SPHERE,
			CYLINDER,
			CONE,
			MAX,
		};
		void UpdateShapeInfo();
		void AddRigidBody();
		std::shared_ptr<btCollisionShape> m_pCollisionShape;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		Collider_Type m_collidrType = Collider_Type::BOX;

		SERIALIZE_COMPONENT(m_collidrType , m_uuID);
	};
}
REGISTER_CEREAL_TYPE(TMF::Collider);
