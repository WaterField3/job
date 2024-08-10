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
		if (m_isEnable)
		{
			OnUpdate();
		}
	}

	void Component::LateUpdate()
	{
		OnLateUpdate();
	}

	void Component::Draw()
	{
		if (m_isEnable)
		{
			OnDraw();
		}
	}
	void Component::DrawImGui()
	{
		OnDrawImGui();
	}

	void Component::CollisionEnter()
	{
		OnCollisionEnter();
	}

	void Component::CollisionStay()
	{
		OnCollisionStay();
	}

	void Component::CollisionExit()
	{
		OnCollisionExit();
	}

	void Component::TrigerEnter()
	{
		OnTrigerEnter();
	}

	void Component::TrigerStay()
	{
		OnTrigerStay();
	}

	void Component::TrigerExit()
	{
		OnTrigerExit();
	}

	boost::uuids::uuid Component::GetUUID()
	{
		return OnGetUUID();
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
	void Component::OnCollisionEnter()
	{
	}
	void Component::OnCollisionStay()
	{
	}
	void Component::OnCollisionExit()
	{
	}
	void Component::OnTrigerEnter()
	{
	}
	void Component::OnTrigerStay()
	{
	}
	void Component::OnTrigerExit()
	{
	}
	boost::uuids::uuid Component::OnGetUUID()
	{
		return boost::uuids::uuid();
	}
}
