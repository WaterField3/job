#include "Camera.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "direct3d.h"

REGISTER_COMPONENT(TMF::Camera, "Camera");

namespace TMF
{
	void Camera::OnInitialize()
	{
		auto fovRadian = DirectX::XMConvertToRadians(m_fov);
		auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
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
			if (m_near >= m_far)
			{
				m_near = m_far - 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}
		label = LabelChange("Far");
		if (ImGui::DragFloat(label.c_str(), &m_far, 0.1f))
		{
			if (m_near >= m_far)
			{
				m_far = m_near + 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}
		label = LabelChange("Fov");
		if (ImGui::DragFloat(label.c_str(), &m_fov, 0.1f))
		{
			if (m_fov == 0)
			{
				m_fov = 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}

	}

	DirectX::SimpleMath::Matrix Camera::GetProjectionMatrix()
	{
		auto fovRadian = DirectX::XMConvertToRadians(m_fov);
		auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
		return projectionMatrix;
	}
	DirectX::SimpleMath::Matrix Camera::GetViewMatrix()
	{
		return MakeViewMatrix();
	}
	DirectX::SimpleMath::Matrix Camera::MakeViewMatrix()
	{
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto targetPos = DirectX::SimpleMath::Vector3::Zero;
		auto up = DirectX::SimpleMath::Vector3::UnitY;
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				pos = pLockTransform->GetWorldPosition();
				auto rotate = pLockTransform->GetRotation();
				auto forward = DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, rotate);
				if (auto pLockTargetTransform = m_pTargetTransform.lock())
				{
					targetPos = pLockTargetTransform->GetPosition() /*+ pLockTargetTransform->GetForward() * 4 + DirectX::SimpleMath::Vector3::UnitY * 3*/;
					up = pLockTargetTransform->GetUp();
				}
				else
				{
					targetPos = pos + forward;
				}
			}
		}
		auto viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(pos, targetPos, up);
		return viewMatrix;
	}
	std::string Camera::LabelChange(const char* labelName)
	{
		auto label = std::string(labelName);
		label += "## " + boost::uuids::to_string(m_uuID);
		return label;
	}
}