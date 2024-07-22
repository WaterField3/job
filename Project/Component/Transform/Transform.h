#pragma once
#include "Component/Component.h"

#include "SimpleMath.h"

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
		void SetPos(DirectX::SimpleMath::Vector3 pos);
		void SetScale(DirectX::SimpleMath::Vector3 scale);
		void SetRotation(DirectX::SimpleMath::Vector3 rotation);

	private:
		DirectX::SimpleMath::Vector3 scale;
		DirectX::SimpleMath::Vector3 position;
		DirectX::SimpleMath::Vector3 rotation;
	};
}

