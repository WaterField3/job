#include "Component.h"

#include "GameObject/GameObject.h"

namespace TMF
{
	Component::Component()
	{

	}

	Component::~Component()
	{

	}

	void Component::Initialize(std::weak_ptr<GameObject> pOwner)
	{
		m_pOwner = pOwner;
		OnInitialize();
	}

	void Component::Finalize()
	{
		OnFinalize();
	}

	void Component::Update()
	{
		OnUpdate();
	}

	void Component::LateUpdate()
	{
		OnLateUpdate();
	}

	void Component::Draw()
	{
		OnDraw();
	}
	void Component::DrawImGui()
	{
		OnDrawImGui();
	}
	void Component::OnInitialize()
	{

	}
	void Component::OnFinalize()
	{

	}
	void Component::OnUpdate()
	{

	}
	void Component::OnLateUpdate()
	{

	}
	void Component::OnDraw()
	{

	}
	void Component::OnDrawImGui()
	{

	}
}
