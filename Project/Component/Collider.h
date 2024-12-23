#pragma once
#include "Component.h"

#include <memory>
#include <btBulletDynamicsCommon.h>
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
		void SetCollisionCenter(DirectX::SimpleMath::Vector3 setCenter);
		void SetCollisionScale(DirectX::SimpleMath::Vector3 setScale);
		inline DirectX::SimpleMath::Vector3 GetCollisionSize() const { return m_size; }
		inline DirectX::SimpleMath::Vector3 GetCollisionCenter() const { return m_center; }
	private:
		void UpdateShapeInfo();
		void AddRigidBody();
		void AddGhostObject();
		void SetCompoundCollisionShape();

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
		DirectX::SimpleMath::Vector3 m_editerOfsetRotation = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Quaternion m_OfsetRotation = DirectX::SimpleMath::Quaternion::Identity;
		std::shared_ptr<btCollisionShape> m_pCollisionShape;
		std::shared_ptr<btCompoundShape> m_pCompaundShape;
		Collider_Type m_collidrType = Collider_Type::BOX;

		SERIALIZE_COMPONENT(m_collidrType, m_size, m_center, m_editerOfsetRotation);
	};
}
