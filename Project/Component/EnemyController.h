#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class StateMachine;
	class EnemyController : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		std::shared_ptr<StateMachine> m_pStateMachine;
		//SERIALIZE_COMPONENT()
	};
}

