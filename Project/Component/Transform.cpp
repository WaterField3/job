#include "Transform.h"

#include <Imgui/imgui.h>

#include "Rigidbody.h"
#include "GhostObject.h"

using namespace DirectX::SimpleMath;

namespace TMF
{
	REGISTER_COMPONENT(Transform);

	Transform::Transform()
	{

	}

	Transform::~Transform()
	{

	}

	void Transform::OnInitialize()
	{
		m_editorRotation = m_rotation.ToEuler();
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

	boost::uuids::uuid Transform::OnGetUUID()
	{
		return m_uuID;
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

	DirectX::SimpleMath::Matrix Transform::GetMatrixLocal()
	{
		// ��]�s��
		auto rotateMatrix = Matrix::CreateFromQuaternion(m_rotation);
		// �g�k�s��
		auto scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
		// �ړ��s��
		auto transformMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

		return scaleMatrix * rotateMatrix * transformMatrix;
	}
	DirectX::SimpleMath::Matrix Transform::GetMatrixRotation()
	{
		// ��]�s��
		return Matrix::CreateFromQuaternion(m_rotation);
	}
	void Transform::ChangeRigidBodyTransform()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto rigidBody = owner->GetComponent<Rigidbody>();
			if (auto rb = rigidBody.lock())
			{
				rb->SetRigidBodyTranform(m_position, m_rotation);
			}
		}
	}
	void Transform::ChangeGhostObjectTransform()
	{
		if (auto owner = m_pOwner.lock())
		{
			auto ghostObject = owner->GetComponent<GhostObject>();
			if (auto ghost = ghostObject.lock())
			{
				ghost->SetGhostObjectTransform(m_position, m_rotation);
			}
		}
	}
	void Transform::UpdateCollisionObjectTransform()
	{
		ChangeRigidBodyTransform();
		ChangeGhostObjectTransform();
	}
}
