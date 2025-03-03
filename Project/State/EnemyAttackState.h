#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class Transform;
	class EnemyAttack;
	class EnemyMove;
	class EnemyAttackState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		float m_endTime = 1.0f;
		float m_timer = 0.0f;
		float m_shotDistance = 3.0f;
		bool m_isPlay = false;

		std::weak_ptr<Transform> m_pTransform;
		std::weak_ptr<Transform> m_pPlayerTransform;
		std::weak_ptr<EnemyAttack> m_pEnemyAttack;
		std::weak_ptr<EnemyMove> m_pEnemyMove;
	};
}

