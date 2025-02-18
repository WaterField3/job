#pragma once
#include "WeaponUI.h"

#include <wrl.h>

#include "ComponentCerealHelper.h"

struct ID3D11ShaderResourceView;
namespace TMF
{
	class WeaponBase;
	class ChangeTimeUI : public WeaponUI
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
		void SetLateChangeTime(float lateChangeTime) { m_lateChangeTime = lateChangeTime; }

	private:
		float m_lateChangeTime = 0.0f;
		bool m_isDrawUI = false;
	};
}

