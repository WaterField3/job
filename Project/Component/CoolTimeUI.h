#pragma once
#include "WeponUI.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"
#include "direct3d.h"

namespace TMF
{
	class WeponBase;
	class CoolTimeUI : public WeponUI
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnSetSelectWepon(std::weak_ptr<WeponBase> wepon) override;

	private:


	};
}

