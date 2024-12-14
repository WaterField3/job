#pragma once
#include "State.h"

#include <memory>

namespace TMF
{
	class Transform;
	class EnemyIdleState : public State
	{
	public:
		void OnInitialize() override;
		void OnEnter() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnExit() override;

	private:
		std::weak_ptr<Transform> m_pPlayerTransfrom;
	};
}

