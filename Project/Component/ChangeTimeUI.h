#pragma once
#include "WeponUI.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"

struct ID3D11ShaderResourceView;
namespace TMF
{
	class WeponBase;
	class ChangeTimeUI : public WeponUI
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnSetSelectWepon(std::weak_ptr<WeponBase> pWepon) override;

	private:
	};
}

