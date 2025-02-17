#include "WeaponUI.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Font.h"
#include "WeaponBase.h"

REGISTER_COMPONENT(TMF::WeaponUI, "WeaponUI");

namespace TMF
{
	void WeaponUI::OnInitialize()
	{

	}
	void WeaponUI::OnFinalize()
	{
	}

	void WeaponUI::OnUpdate()
	{
	}

	void WeaponUI::OnLateUpdate()
	{
	}

	void WeaponUI::OnDraw()
	{
	}

	void WeaponUI::OnDrawImGui()
	{
	}

	std::shared_ptr<Component> WeaponUI::OnClone() const
	{
		auto pClone = std::make_shared<WeaponUI>();
		return move(pClone);
	}

	void WeaponUI::SetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon)
	{
		OnSetSelectWeapon(pWeapon);
	}
	void WeaponUI::OnSetSelectWeapon(std::weak_ptr<WeaponBase> pWeapon)
	{

	}
}