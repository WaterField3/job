#include "WeponBase.h"
namespace TMF
{
	void WeponBase::OnInitialize()
	{
	}
	void WeponBase::OnFinalize()
	{
	}
	void WeponBase::OnUpdate()
	{
	}
	void WeponBase::OnLateUpdate()
	{
	}
	void WeponBase::OnDraw()
	{
	}
	void WeponBase::OnDrawImGui()
	{
	}
	std::shared_ptr<Component> WeponBase::OnClone() const
	{
		auto pClone = std::make_shared<WeponBase>();
		return move(pClone);
	}
	void WeponBase::Attack()
	{
		OnAttack();
	}
	void WeponBase::OnAttack()
	{

	}
	void WeponBase::Select()
	{
		OnSelect();
	}
	void WeponBase::OnSelect()
	{
	}
}