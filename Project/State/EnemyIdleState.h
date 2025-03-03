#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class Transform;
	class EnemyMove;
	class EnemyAttack;
	class EnemyIdleState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		std::weak_ptr<Transform> m_pPlayerTransform;
		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<EnemyMove> m_pEnemyMove;
		std::weak_ptr<EnemyAttack> m_pEnemyAttack;
	};
}

