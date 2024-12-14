#include "FreeCamera.h"

#include <Imgui/imgui.h>

#include "Utility/StringHelper.h"
#include "ComponentRegister.h"
#include "direct3d.h"
#include "Transform.h"
#include "Input.h"

//REGISTER_COMPONENT(TMF::FreeCamera, "FreeCamera");

namespace TMF
{
	void FreeCamera::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = m_pTransform.lock())
			{
				m_azimuth = m_azimuth + DirectX::XM_PI / 180.0f;
				auto pos = DirectX::SimpleMath::Vector3::Zero;
				pos.x = m_raduis * sin(m_elevation) * cos(m_azimuth);
				pos.y = m_raduis * cos(m_elevation);
				pos.z = m_raduis * sin(m_elevation) * sin(m_azimuth);
				pLockTransform->SetPosition(pos);
			}
		}
	}
	void FreeCamera::OnFinalize()
	{

	}
	void FreeCamera::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			//m_azimuth = m_azimuth + DirectX::XM_PI / 180.0f;
			auto azimuth = m_azimuth + DirectX::XM_PI / 180.0f;
			auto pos = DirectX::SimpleMath::Vector3::Zero;
			pos.x = m_raduis * sin(m_elevation) * cos(azimuth);
			pos.y = m_raduis * cos(m_elevation);
			pos.z = m_raduis * sin(m_elevation) * sin(azimuth);
			pLockTransform->SetPosition(pos);
		}
	}
	void FreeCamera::OnLateUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
		{
			auto mouseState = Input::Instance().GetMouseState();
			mouseState.positionMode = DirectX::Mouse::Mode::MODE_ABSOLUTE;
			auto mouseDelta = DirectX::SimpleMath::Vector2(mouseState.x, mouseState.y);
			// 現在の絶対位置
			int currentMouseX = mouseState.x;
			int currentMouseY = mouseState.y;
			// 前回の位置との差分を計算（相対移動量）
			int deltaX = currentMouseX - prevMouseX;
			int deltaY = currentMouseY - prevMouseY;
			m_azimuth += deltaX * m_rotationSpeed * 0.001f;
			m_elevation -= deltaY * m_rotationSpeed * 0.001f;

			// 現在の位置を次回のために保存
			prevMouseX = currentMouseX;
			prevMouseY = currentMouseY;
		}
	}
	void FreeCamera::OnDraw()
	{

	}
	void FreeCamera::OnDrawImGui()
	{
		auto nearLabel = StringHelper::CreateLabel("Near", m_uuID);
		if (ImGui::DragFloat(nearLabel.c_str(), &m_near, 0.1f))
		{
			if (m_near >= m_far)
			{
				m_near = m_far - 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}

		auto farLabel = StringHelper::CreateLabel("Far", m_uuID);
		if (ImGui::DragFloat(farLabel.c_str(), &m_far, 0.1f))
		{
			if (m_near >= m_far)
			{
				m_far = m_near + 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}

		auto fovLabel = StringHelper::CreateLabel("Fov", m_uuID);
		if (ImGui::DragFloat(fovLabel.c_str(), &m_fov, 0.1f))
		{
			if (m_fov == 0)
			{
				m_fov = 0.1f;
			}
			auto fovRadian = DirectX::XMConvertToRadians(m_fov);
			auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
			D3D::Get()->SettingEffect(MakeViewMatrix(), projectionMatrix);
		}

		auto radiusLabel = StringHelper::CreateLabel("Radius", m_uuID);
		if (ImGui::DragFloat(radiusLabel.c_str(), &m_raduis, 0.1f))
		{

		}
		auto elevationLabel = StringHelper::CreateLabel("Elevation", m_uuID);
		if (ImGui::DragFloat(elevationLabel.c_str(), &m_elevation, 0.1f))
		{

		}
		auto azimuthLabel = StringHelper::CreateLabel("Azimuth", m_uuID);
		if (ImGui::DragFloat(azimuthLabel.c_str(), &m_azimuth, 0.1f))
		{

		}
		auto rotationSpeedLabel = StringHelper::CreateLabel("RotationSpeed", m_uuID);
		if (ImGui::DragFloat(rotationSpeedLabel.c_str(), &m_rotationSpeed))
		{

		}
	}
	DirectX::SimpleMath::Matrix FreeCamera::GetProjectionMatrix()
	{
		auto fovRadian = DirectX::XMConvertToRadians(m_fov);
		auto projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(fovRadian, float(1024) / float(576), m_near, m_far);
		return projectionMatrix;
	}
	DirectX::SimpleMath::Matrix FreeCamera::GetViewMatrix()
	{
		return MakeViewMatrix();
	}
	DirectX::SimpleMath::Matrix FreeCamera::MakeViewMatrix()
	{
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto targetPos = DirectX::SimpleMath::Vector3::Zero;
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
					//targetPos = pLockTargetTransform->GetPosition() + pLockTargetTransform->GetForward() * 100;
					targetPos = pLockTargetTransform->GetPosition();
				}
				else
				{
					targetPos = pos + forward;
				}

			}
		}
		auto viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(pos, targetPos, DirectX::SimpleMath::Vector3::UnitY);
		return viewMatrix;
	}
}