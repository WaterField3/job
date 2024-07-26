#include "GameObject.h"

#include <Imgui/imgui.h>
#include <string.h>

#include "Component/ComponentManager.h"

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

		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_name.c_str());
		if (ImGui::InputText("Name", buf, 256))
		{
			m_name = buf;
		}
		auto index = 0;
		for (auto& component : m_pComponents)
		{
			index++;
			auto componentName = ComponentManager::Instance().GetComponentName(typeid(*component));
			auto label = componentName;
			label += "## " + std::to_string(index);
			if (ImGui::CollapsingHeader(label.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
			{
				if (ImGui::IsItemClicked(ImGuiMouseButton_Right))
				{
					ImGui::OpenPopup("ComponentMenuPopup");

				}
				if (ImGui::BeginPopup("ComponentMenuPopup"))
				{
					if (ImGui::Button("RemoveComponent"))
					{
						ComponentManager::Instance().RemoveComponent(componentName, shared_from_this());
						ImGui::CloseCurrentPopup();
						ImGui::EndPopup();
						continue;
					}
					ImGui::EndPopup();
				}
				component->DrawImGui();
			}
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