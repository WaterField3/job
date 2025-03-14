#pragma once
#include "State.h"

#include <memory>

#include "MoveInfo.h"

namespace TMF
{
	class PlayerJump;
	class Transform;
	class Jump;
	class PlayerJumpState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		std::unique_ptr<PlayerJump> m_pPlayerJump;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Jump> m_pJump;
		MoveDirection m_moveDirection = MoveDirection::FOWARD;
	};
}

