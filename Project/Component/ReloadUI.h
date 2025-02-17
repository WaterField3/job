#pragma once
#include "WeaponUI.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"
#include "direct3d.h"

namespace TMF
{
	class WeaponBase;
	class ReloadUI : public WeaponUI
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;
		void OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon) override;
	private:
	};
}

