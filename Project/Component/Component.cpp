#include "Component.h"

#include "GameObject/GameObject.h"
#include "Utility/Log.h"

CEREAL_REGISTER_TYPE(TMF::Component);

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

	void Component::CollisionEnter(GameObject* pGameObject)
	{
		OnCollisionEnter(pGameObject);
	}

	void Component::CollisionStay(GameObject* pGameObject)
	{
		OnCollisionStay(pGameObject);
	}

	void Component::CollisionExit(GameObject* pGameObject)
	{
		OnCollisionExit(pGameObject);
	}

	void Component::TriggerEnter(GameObject* pGameObject)
	{
		OnTriggerEnter(pGameObject);
	}

	void Component::TriggerStay(GameObject* pGameObject)
	{
		OnTriggerStay(pGameObject);
	}

	void Component::TriggerExit(GameObject* pGameObject)
	{
		OnTriggerExit(pGameObject);
	}

	std::shared_ptr<Component> Component::Clone() const
	{
		auto pClone = OnClone();
		return move(pClone);
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

	void Component::OnCollisionEnter(GameObject* pGameObject)
	{

	}

	void Component::OnCollisionStay(GameObject* pGameObject)
	{

	}

	void Component::OnCollisionExit(GameObject* pGameObject)
	{

	}

	void Component::OnTriggerEnter(GameObject* pGameObject)
	{

	}

	void Component::OnTriggerStay(GameObject* pGameObject)
	{

	}

	void Component::OnTriggerExit(GameObject* pGameObject)
	{

	}

	std::shared_ptr<Component> Component::OnClone() const
	{
		return std::make_shared<Component>(*this);
	}
}
