#pragma once
#include "Component.h"

namespace TMF
{
	class Transform;
	class GetWepon : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void OnCollisionEnter(GameObject* pGameObject) override;

		std::shared_ptr<Component> OnClone() const override;

	private:
		std::weak_ptr<Transform> m_pTransform;
	};
}

