#pragma once

#include "Component.h"

#include <memory>
#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/CollisionDispatch/btGhostObject.h>
#include <boost/uuid/random_generator.hpp>

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

	private:
		std::shared_ptr<btGhostObject> m_pGhostObject;
		boost::uuids::uuid m_uuID = boost::uuids::random_generator()();
		SERIALIZE_COMPONENT(m_uuID);
	};
}

REGISTER_CEREAL_TYPE(TMF::GhostObject);
