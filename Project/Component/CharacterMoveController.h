#pragma once

#include "Component.h"

#include <cereal/types/polymorphic.hpp>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <d3d11.h>
#include <SimpleMath.h>
#include <string>

#include "ComponentCerealHelper.h"
#include "ComponentRegister.h"

namespace TMF
{
	class Transform;
	class Rigidbody;
	class StateMachine;
	class CharacterMoveController : public Component
	{
	public:
		CharacterMoveController();
		~CharacterMoveController();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		float m_moveSpeed = 1;
		float m_jumpPower = 1;
		DirectX::SimpleMath::Vector3 checkvec3= DirectX::SimpleMath::Vector3::One;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Rigidbody> m_pRigidbody;
		std::shared_ptr<StateMachine> m_pStateMachine;

		SERIALIZE_COMPONENT(m_moveSpeed, m_jumpPower, checkvec3);
	};
}
