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
		void MakeCollision();
		std::weak_ptr<btCollisionShape> GetCollisionShape();
		inline DirectX::SimpleMath::Vector3 GetCollsionCenter() const { return m_center; }
		inline DirectX::SimpleMath::Vector3 GetCollisionSize() const { return m_size; }

	private:
		void UpdateShapeInfo();
		void AddRigidBody();
		void AddGhostObject();

	private:
		enum class Collider_Type
		{
			BOX,
			CAPSULE,
			SPHERE,
			CYLINDER,
			CONE,
			PLANE,
			CONVEXHULL,
			TERRAIN,
			MAX,
		};

		int m_poiygonNum = 0;
		DirectX::SimpleMath::Vector3 m_size = DirectX::SimpleMath::Vector3::One;
		DirectX::SimpleMath::Vector3 m_center = DirectX::SimpleMath::Vector3::Zero;
		std::shared_ptr<btCollisionShape> m_pCollisionShape;
		Collider_Type m_collidrType = Collider_Type::BOX;

		SERIALIZE_COMPONENT(m_collidrType, m_size, m_center);
	};
}
