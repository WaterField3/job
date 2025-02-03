#include "Transform.h"

#include <Imgui/imgui.h>

#include "Rigidbody.h"
#include "GhostObject.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/Log.h"

using namespace DirectX::SimpleMath;

REGISTER_COMPONENT(TMF::Transform, "Transform");

namespace TMF
{
	Transform::Transform()
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::OnInitialize()
	{
		m_editorRotation = m_rotation.ToEuler();
		auto str = "TransformUUID:" + boost::uuids::to_string(m_uuID);
		Log::Info("Transform:OnInitialize\n");
	}

	void Transform::OnFinalize()
	{

	}

	void Transform::OnUpdate()
	{

	}

	void Transform::OnLateUpdate()
	{

	}

	void Transform::OnDraw()
	{

	}

	void Transform::OnDrawImGui()
	{
		auto componentName = std::string("Position");
		auto label = componentName;
		auto uuidStr = boost::uuids::to_string(m_uuID);
		label += "## " + uuidStr;
		if (ImGui::DragFloat3(label.c_str(), &m_position.x, 0.1f))
		{
			UpdateCollisionObjectTransform();
		}
		label = "Rotation";
		label += "## " + uuidStr;
		if (ImGui::DragFloat3(label.c_str(), &m_editorRotation.x, 0.1f))
		{
			m_rotation = Quaternion::CreateFromYawPitchRoll(m_editorRotation.y, m_editorRotation.x, m_editorRotation.z);
			UpdateCollisionObjectTransform();

		}
		label = "Scale";
		label += "## " + uuidStr;
		if (ImGui::DragFloat3(label.c_str(), &m_scale.x, 0.1f))
		{

		}
	}

	std::shared_ptr<Component> Transform::OnClone() const
	{
		auto clone = std::make_shared<Transform>();
		clone->m_position = this->m_position;
		clone->m_rotation = this->m_rotation;
		clone->m_editorRotation = this->m_editorRotation;
		clone->m_scale = this->m_scale;
		return move(clone);
	}

	void Transform::SetPosition(DirectX::SimpleMath::Vector3 pos)
	{
		m_position = pos;
	}

	void Transform::SetScale(DirectX::SimpleMath::Vector3 scale)
	{
		m_scale = scale;
	}

	void Transform::SetRotation(DirectX::SimpleMath::Quaternion rotation)
	{
		m_rotation = rotation;
		m_editorRotation = m_rotation.ToEuler();
	}

	DirectX::SimpleMath::Matrix Transform::GetLocalMatrix()
	{
		// ‰ñ“]s—ñ
		auto rotateMatrix = Matrix::CreateFromQuaternion(m_rotation);
		// Šgks—ñ
		auto scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
		// ˆÚ“®s—ñ
		auto transformMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

		return scaleMatrix * rotateMatrix * transformMatrix;
	}
	DirectX::SimpleMath::Matrix Transform::GetWorldMatrix()
	{
		auto matrix = GetLocalMatrix();
		if (auto pParent = m_pParent.lock())
		{
			matrix *= pParent->GetWorldMatrix();
		}
		return matrix;

	}
	DirectX::SimpleMath::Matrix Transform::GetMatrixRotation()
	{
		// ‰ñ“]s—ñ
		return Matrix::CreateFromQuaternion(m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetWorldPosition()
	{
		auto position = m_position;
		if (auto pParent = m_pParent.lock())
		{
			position += pParent->GetWorldPosition();
		}
		return position;
	}
	DirectX::SimpleMath::Quaternion Transform::GetWorldRotation()
	{
		auto rotation = m_rotation;
		if (auto pParent = m_pParent.lock())
		{
			rotation *= pParent->GetWorldRotation();
		}
		return rotation;
	}
	DirectX::SimpleMath::Vector3 Transform::GetForward()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Forward, m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetRight()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Right, m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetLeft()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Left, m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetBack()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Backward, m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetUp()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Up, m_rotation);
	}
	DirectX::SimpleMath::Vector3 Transform::GetDown()
	{
		return DirectX::SimpleMath::Vector3::Transform(DirectX::SimpleMath::Vector3::Down, m_rotation);
	}
	void Transform::ChangeRigidBodyTransform()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			if (auto pLockRigidbody = pRigidbody.lock())
			{
				pLockRigidbody->SetRigidBodyTranform(m_position, m_rotation);
			}
		}
	}
	void Transform::ChangeGhostObjectTransform()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pGhostObject = pLockOwner->GetComponent<GhostObject>();
			if (auto pLockGhostObject = pGhostObject.lock())
			{
				pLockGhostObject->SetGhostObjectTransform(m_position, m_rotation);
			}
		}
	}
	void Transform::UpdateCollisionObjectTransform()
	{
		ChangeRigidBodyTransform();
		ChangeGhostObjectTransform();
	}
}
