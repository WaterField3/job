#pragma once
#include "UIBase.h"

#include "ComponentCerealHelper.h"

namespace TMF
{
	class Font;
	class WeaponBase;
	class WeaponUI : public UIBase
	{
	public:
		void OnInitialize() override;
		void OnFinalize() override;
		void OnUpdate() override;
		void OnLateUpdate() override;
		void OnDraw() override;
		void OnDrawImGui() override;
		std::shared_ptr<Component> OnClone() const override;

		void SetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon);

	protected:
		virtual void OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon);

	protected:
		std::weak_ptr<WeaponBase> m_pWeapon;

		//SERIALIZE_COMPONENT();
	};
}

