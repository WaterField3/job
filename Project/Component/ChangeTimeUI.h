#pragma once
#include "Component.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"

struct ID3D11ShaderResourceView;
namespace TMF
{
	class WeponBase;
	class ChangeTimeUI : public Component
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void SetSelectWepon(std::weak_ptr<WeponBase> pWepon);

	private:
		float m_barWidth = 100.0f;
		float m_barHeight = 40.0f;
		std::string m_barTextureName = "";
		DirectX::SimpleMath::Vector2 m_drawPosition = DirectX::SimpleMath::Vector2::Zero;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pBarTexture;

		std::weak_ptr<WeponBase> m_pWepon;

		SERIALIZE_COMPONENT(m_barWidth, m_barHeight, m_barTextureName, m_drawPosition);
	};
}

