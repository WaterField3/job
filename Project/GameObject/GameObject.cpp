#include "GameObject.h"

void TMF::GameObject::Initialize()
{
	if (m_uuID.is_nil())
	{
		m_uuID = boost::uuids::random_generator()();
	}
}

void TMF::GameObject::Finalize()
{

}

void TMF::GameObject::Update()
{
	for (auto& component : m_pComponents)
	{
		component->Update();
	}
}

void TMF::GameObject::Draw()
{
	for (auto& component : m_pComponents)
	{
		component->Draw();
	}
}

void TMF::GameObject::LateUpdate()
{
	for (auto& component : m_pComponents)
	{
		component->LateUpdate();
	}
}