#pragma once
#include "Component/Component.h"

#include "SimpleMath.h"
#include <cereal/types/polymorphic.hpp>
namespace TMF
{
	class Transform : public Component
	{
	public:
		Transform();
		~Transform();

		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		void SetPos(DirectX::SimpleMath::Vector3 pos);
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		void SetRotation(DirectX::SimpleMath::Vector3 rotation);

		DirectX::SimpleMath::Matrix GetMatrixLocal();

	private:
		DirectX::SimpleMath::Vector3 m_scale;
		DirectX::SimpleMath::Vector3 m_position;
		DirectX::SimpleMath::Vector3 m_rotation;
	};
}
CEREAL_REGISTER_TYPE(TMF::Transform);

CEREAL_REGISTER_POLYMORPHIC_RELATION(TMF::Component, TMF::Transform)
