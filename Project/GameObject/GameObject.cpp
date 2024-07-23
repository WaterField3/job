#include "GameObject.h"

void TMF::GameObject::Initialize()
{

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