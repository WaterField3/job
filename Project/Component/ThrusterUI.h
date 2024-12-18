#pragma once
#include "Component.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"
#include "direct3d.h"

namespace TMF
{
	class Thruster;
	class ThrusterUI : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;

	private:
		float m_barWidth = 100.0f;
		float m_barHeight = 40.0f;
		std::string m_barTextureName = "";
		std::string m_targetName = "";
		DirectX::SimpleMath::Vector2 m_drawPosition = DirectX::SimpleMath::Vector2::Zero;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBarTexture;

		std::weak_ptr<Thruster> m_pThruster;

		SERIALIZE_COMPONENT(m_barWidth, m_barHeight, m_barTextureName, m_targetName, m_drawPosition);

	};
}
