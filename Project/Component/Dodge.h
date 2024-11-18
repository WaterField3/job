#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Dodge : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

		enum Direction
		{
			FOWARD,
			RIGHT,
			LEFT,
			BACK
		};

		void DodgeStart(Direction direction);
	private:
		bool m_isDodgeMove = false;
		float m_dodgeTime = 1.0f;
		float m_timer = 0.0f;
		float m_moveMagnification = 1.0f;
		DirectX::SimpleMath::Vector3 m_dodgeMoveVector = DirectX::SimpleMath::Vector3::Zero;
		Direction m_dodgeDirection = Direction::FOWARD;
		std::string m_fowardDodge = "";
		std::string m_rightDodge = "";
		std::string m_leftDodge = "";
		std::string m_backDodge = "";

		SERIALIZE_COMPONENT(m_fowardDodge, m_rightDodge, m_leftDodge, m_backDodge, m_moveMagnification);
	};
}

