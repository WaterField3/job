#include "CameraMove.h"

#include <Imgui/imgui.h>
#include <iostream>

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Transform.h"
#include "Input.h"
#include "Timer.h"
#include "Camera.h"
#include "Rigidbody.h"

REGISTER_COMPONENT(TMF::CameraMove, "CameraMove");

namespace TMF
{
	void CameraMove::OnInitialize()
	{
		if (m_targetName != "")
		{
			auto pTargetObject = GameObjectManager::Instance().GetGameObject(m_targetName);
			if (auto pLockTargetObject = pTargetObject.lock())
			{
				auto playerPos = DirectX::SimpleMath::Vector3::Zero;
				m_pPlayerTransform = pLockTargetObject->GetComponent<Transform>();
				if (auto pLockPlayerTransform = m_pPlayerTransform.lock())
				{
					playerPos = pLockPlayerTransform->GetPosition();
				}
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
					auto pCamera = pLockOwner->GetComponent<Camera>();
					if (auto pLockCamera = pCamera.lock())
					{
						pLockCamera->SetTargetTransform(m_pPlayerTransform);
					}
					m_pTargetRigidbody = pLockTargetObject->GetComponent<Rigidbody>();
				}
			}
		}
	}
	void CameraMove::OnFinalize()
	{

	}
	void CameraMove::OnUpdate()
	{
		if (auto pLockTransform = m_pTransform.lock())
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

				m_elevation = std::clamp(m_elevation, 0.5f, 2.0f);

				auto azimuth = m_azimuth + DirectX::XM_PI / 180.0f;
				auto pos = DirectX::SimpleMath::Vector3::Zero;
				pos.x = m_raduis * sin(m_elevation) * cos(azimuth);
				pos.y = m_raduis * cos(m_elevation);
				pos.z = m_raduis * sin(m_elevation) * sin(azimuth);
				pLockTransform->SetPosition(pos);

				// 現在の位置を次回のために保存
				prevMouseX = currentMouseX;
				prevMouseY = currentMouseY;
			}
		}
	}
	void CameraMove::OnLateUpdate()
	{
		if (auto pLockTargetRigidbody = m_pTargetRigidbody.lock())
		{
			btTransform transform =  pLockTargetRigidbody->GetBtTransform();
			btQuaternion currentRotation = transform.getRotation();
			btScalar yaw = currentRotation.getAngle();
			btScalar deltaAngle = -m_azimuth  - yaw + 1.5f;
			btQuaternion rotationDelta(btVector3(0, 1, 0), deltaAngle);
			btQuaternion newRotation = currentRotation * rotationDelta;
			transform.setRotation(newRotation);
			pLockTargetRigidbody->SetBtTransform(transform);
		}
	}


	void CameraMove::OnDraw()
	{

	}
	void CameraMove::OnDrawImGui()
	{
		auto radiusLabel = StringHelper::CreateLabel("Radius", m_uuID);
		if (ImGui::DragFloat(radiusLabel.c_str(), &m_raduis, 0.1f, 0.1f))
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

		auto targetLabel = StringHelper::CreateLabel("Target", m_uuID);
		char targetBuf[256] = "";
		strcpy_s(targetBuf, sizeof(targetBuf), m_targetName.c_str());
		if (ImGui::InputText(targetLabel.c_str(), targetBuf, 256))
		{
			m_targetName = targetBuf;
		}
	}
}