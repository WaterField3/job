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

		inline bool GetIsPlay() const { return m_isPlay; }

	private:
		bool m_isPlay = false;
		float m_moveSpeed = 1.0f;
		float m_rotationOffset = 1.0f;
		float m_playerUp = 1.0f;
		float m_time = 0.0f;
		float m_rotationSpeed = 1.0f;
		DirectX::SimpleMath::Vector3 m_targetRotationOffset = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_rotate = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_endPosition = DirectX::SimpleMath::Vector3::Zero;
		DirectX::SimpleMath::Vector3 m_moveVector = DirectX::SimpleMath::Vector3::Zero;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pParent;

		SERIALIZE_COMPONENT(m_moveSpeed, m_rotate, m_rotationOffset, m_rotationSpeed, m_targetRotationOffset);
	};
}

