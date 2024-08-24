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
		inline bool IsRemovable() override { return false; }

		void SetPosition(DirectX::SimpleMath::Vector3 pos);
		void SetRotation(DirectX::SimpleMath::Quaternion rotation);
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		DirectX::SimpleMath::Matrix GetLocalMatrix();
		DirectX::SimpleMath::Matrix GetWorldMatrix();
		DirectX::SimpleMath::Matrix GetMatrixRotation();
		void ChangeRigidBodyTransform();
		void ChangeGhostObjectTransform();
		void UpdateCollisionObjectTransform();

		inline DirectX::SimpleMath::Vector3 GetPosition() const { return m_position; }
		inline DirectX::SimpleMath::Quaternion GetRotation() const { return m_rotation; }
		inline DirectX::SimpleMath::Vector3 GetScale() const { return m_scale; }
		inline std::weak_ptr<Transform> GetParent() const { return m_pParent; }
		inline void SetParent(std::weak_ptr<Transform> transform) { m_pParent = transform; }

	private:
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_scale = DirectX::SimpleMath::Vector3::One;
		DirectX::SimpleMath::Quaternion m_rotation = DirectX::SimpleMath::Quaternion::Identity;
		DirectX::SimpleMath::Vector3 m_editorRotation;
		std::weak_ptr<Transform> m_pParent;

		SERIALIZE_COMPONENT(m_position, m_scale, m_rotation, m_pParent);
	};
}
