#include "ColorChange.h"

#include "ComponentRegister.h"
#include "PrimitiveMesh.h"

REGISTER_COMPONENT(TMF::ColorChange, "ColorChange");

namespace TMF
{
	void ColorChange::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pPrimitiveMesh = pLockOwner->GetComponent<PrimitiveMesh>();
		}
	}
	void ColorChange::OnFinalize()
	{
	}
	void ColorChange::OnUpdate()
	{
	}
	void ColorChange::OnLateUpdate()
	{
	}
	void ColorChange::OnDraw()
	{
	}
	void ColorChange::OnDrawImGui()
	{
	}
	void ColorChange::OnCollisionEnter(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Ground)
		{
			return;
		}
		if (auto pLockPrimitiveMesh = m_pPrimitiveMesh.lock())
		{
			pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	void ColorChange::OnCollisionStay(GameObject* pGameObject)
	{
	}
	void ColorChange::OnCollisionExit(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Ground)
		{
			return;
		}
		if (auto pLockPrimitiveMesh = m_pPrimitiveMesh.lock())
		{
			pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
	void ColorChange::OnTrigerEnter(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Ground)
		{
			return;
		}
		if (auto pLockPrimitiveMesh = m_pPrimitiveMesh.lock())
		{
			pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(1.0f, 0.0f, 0.0f, 1.0f));
		}
	}
	void ColorChange::OnTrigerStay(GameObject* pGameObject)
	{
	}
	void ColorChange::OnTrigerExit(GameObject* pGameObject)
	{
		if (pGameObject->GetTag() == GameObject::Tag::Ground)
		{
			return;
		}
		if (auto pLockPrimitiveMesh = m_pPrimitiveMesh.lock())
		{
			pLockPrimitiveMesh->SetColor(DirectX::SimpleMath::Color(1.0f, 1.0f, 1.0f, 1.0f));
		}
	}
}