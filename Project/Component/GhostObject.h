#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>

#include <boost/uuid/uuid_generators.hpp>
#include <memory>

#include "ComponentRegister.h"
#include "ComponentCerealHelper.h"

namespace TMF
{
	class GhostObject : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		boost::uuids::uuid OnGetUUID() override;
		void AddGhostObject(std::weak_ptr<btCollisionShape> col, DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion qua);
		void RemoveGhostObject();
		void SetGhostObjectTransform(DirectX::SimpleMath::Vector3 pos, DirectX::SimpleMath::Quaternion rotate);

	private:
		std::shared_ptr<btGhostObject> m_pGhostObject;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		SERIALIZE_COMPONENT(m_uuID);
	};
}

REGISTER_CEREAL_TYPE(TMF::GhostObject);
