#pragma once
#include "Component.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class GameObject;
	class GenerateWepon : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
	private:
		void Load();
		void Generate();
		float m_timer = 0.0f;
		float m_generateRate = 5.0f;
		DirectX::SimpleMath::Vector3 m_generateRange = DirectX::SimpleMath::Vector3::One;
		std::vector<std::shared_ptr<GameObject>> m_pOriginWepons;
		std::string m_file = "WeponDatas";

		SERIALIZE_COMPONENT(m_file);
	};
}
