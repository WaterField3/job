#include "GameObject.h"

#include <Imgui/imgui.h>

#include <string.h>

namespace TMF
{
	void GameObject::Initialize()
	{
		if (m_uuID.is_nil())
		{
			m_uuID = boost::uuids::random_generator()();
		}
	}

	void GameObject::Finalize()
	{

	}

	void GameObject::Update()
	{
		for (auto& component : m_pComponents)
		{
			component->Update();
		}
	}

	void GameObject::Draw()
	{
		for (auto& component : m_pComponents)
		{
			component->Draw();
		}
	}

	void GameObject::DrawImGui()
	{
		//ImGui::Text(m_name.c_str());
		char buf[256] = {};
		strcpy_s(buf, sizeof(buf), m_name.c_str());
		ImGui::InputText("Name", buf, 256);
		//m_name = buf;
		for (auto& component : m_pComponents)
		{
			component->DrawImGui();
		}
	}

	void GameObject::LateUpdate()
	{
		for (auto& component : m_pComponents)
		{
			component->LateUpdate();
		}
	}
}