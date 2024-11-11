#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Transform;
	class MeleeMove : public Component
	{
	public:
		MeleeMove();
		~MeleeMove();
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		enum MoveType
		{
			DEFAULT,
			SPECIAL,
		};
		void Play(MoveType type, DirectX::SimpleMath::Vector3 position, DirectX::SimpleMath::Quaternion rotation);

	private:
		void OnTrigerEnter(GameObject* pGameObject) override;

	private:
		float m_moveSpeed = 1.0f;
		DirectX::SimpleMath::Vector3 m_rotate = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_endPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_pTransform;

		SERIALIZE_COMPONENT(m_moveSpeed, m_rotate);
	};
}

