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

	void Component::TrigerEnter(GameObject* pGameObject)
	{
		OnTrigerEnter(pGameObject);
	}

	void Component::TrigerStay(GameObject* pGameObject)
	{
		OnTrigerStay(pGameObject);
	}

	void Component::TrigerExit(GameObject* pGameObject)
	{
		OnTrigerExit(pGameObject);
	}

	void Component::ChangeUUID()
	{
		m_uuID = boost::uuids::random_generator()();
		//OnChangeUUID();
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

	void Component::OnTrigerEnter(GameObject* pGameObject)
	{

	}

	void Component::OnTrigerStay(GameObject* pGameObject)
	{

	}

	void Component::OnTrigerExit(GameObject* pGameObject)
	{

	}
	void Component::OnChangeUUID()
	{
		auto str = "ïœçXëO:" + boost::uuids::to_string(m_uuID);
		//Log::DebugInfo(str);
		m_uuID = boost::uuids::random_generator()();
		str = "ïœçXå„:" + boost::uuids::to_string(m_uuID);
		//Log::DebugInfo(str);
	}
	std::shared_ptr<Component> Component::OnClone() const
	{
		return std::make_shared<Component>(*this);
	}
}
