#pragma once
#include "Component.h"

#include "Component/ComponentCerealHelper.h"

namespace TMF
{
	class PrimitiveMesh;
	class ColorChange : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnCollisionEnter(GameObject* pGameObject) override;
		void OnCollisionStay(GameObject* pGameObject) override;
		void OnCollisionExit(GameObject* pGameObject) override;
		void OnTrigerEnter(GameObject* pGameObject) override;
		void OnTrigerStay(GameObject* pGameObject) override;
		void OnTrigerExit(GameObject* pGameObject) override;

	private:
		std::weak_ptr<PrimitiveMesh> m_pPrimitiveMesh;
		//SERIALIZE_COMPONENT();
	};
}

