#include "UIBase.h"

namespace TMF
{
	void UIBase::OnInitialize()
	{
	}
	void UIBase::OnFinalize()
	{
	}
	void UIBase::OnUpdate()
	{
	}
	void UIBase::OnLateUpdate()
	{
	}
	void UIBase::OnDraw()
	{
	}
	void UIBase::OnDrawImGui()
	{
	}
	std::shared_ptr<Component> UIBase::OnClone() const
	{
		auto pClone = std::make_shared<UIBase>();
		return move(pClone);
	}
}