#include "WeponUI.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "Font.h"
#include "WeponBase.h"

REGISTER_COMPONENT(TMF::WeponUI, "WeponUI");

namespace TMF
{
	void WeponUI::OnInitialize()
	{

	}
	void WeponUI::OnFinalize()
	{
	}

	void WeponUI::OnUpdate()
	{
	}

	void WeponUI::OnLateUpdate()
	{
	}

	void WeponUI::OnDraw()
	{
	}

	void WeponUI::OnDrawImGui()
	{
	}

	std::shared_ptr<Component> WeponUI::OnClone() const
	{
		auto pClone = std::make_shared<WeponUI>();
		return move(pClone);
	}

	void WeponUI::SetSelectWepon(std::weak_ptr<WeponBase> pWepon)
	{
		OnSetSelectWepon(pWepon);
	}
	void WeponUI::OnSetSelectWepon(std::weak_ptr<WeponBase> wepon)
	{

	}
}