#include "Camera.h"

#include <Imgui/imgui.h>

#include "Component/Transform.h"
#include "direct3d.h"

namespace TMF
{
	REGISTER_COMPONENT(Camera);
	void Camera::OnInitialize()
	{
		auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(1024) / float(576), m_near, m_far);
		D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
	}
	void Camera::OnFinalize()
	{

	}
	void Camera::OnUpdate()
	{

	}
	void Camera::OnLateUpdate()
	{

	}
	void Camera::OnDraw()
	{

	}
	void Camera::OnDrawImGui()
	{
		auto label = LabelChange("Near");
		if (ImGui::DragFloat(label.c_str(), &m_near, 0.1f))
		{
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}
		label = LabelChange("Far");
		if (ImGui::DragFloat(label.c_str(), &m_far, 0.1f))
		{
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}
		label = LabelChange("Fov");
		if (ImGui::DragFloat(label.c_str(), &m_fov, 0.1f))
		{
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}

	}
	DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
	{
		auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(m_fov, float(1024) / float(576), m_near, m_far);
		return projectionMatrix;
	}
	DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
	{
		return MakeViewMatrix();
	}
	DirectX::SimpleMath::Matrix Camera::MakeViewMatrix()
	{
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		if (auto owner = m_pOwner.lock())
		{
			auto transformComponent = owner->GetComponent<Transform>();
			if (auto transform = transformComponent.lock())
			{
				pos = transform->GetPosition();
			}
		}

		auto viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(pos, pos.Forward, DirectX::SimpleMath::Vector3::UnitY);;

		return viewMatrix;
	}
	std::string Camera::LabelChange(const char* labelName)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(m_uuID);
		return label;
	}
}