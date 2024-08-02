#include "Transform.h"

#include <Imgui/imgui.h>

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

		}
		label = "Rotation";
		label += "## " + uuidStr;
		if (ImGui::DragFloat3(label.c_str(), &m_editorRotation.x, 0.1f))
		{
			m_rotation = Quaternion::CreateFromYawPitchRoll(m_editorRotation.y, m_editorRotation.x, m_editorRotation.z);
		}
		label = "Scale";
		label += "## " + uuidStr;
		if (ImGui::DragFloat3(label.c_str(), &m_scale.x, 0.1f))
		{

		}
	}

	void Transform::SetPosition(DirectX::SimpleMath::Vector3 pos)
	{

	}

	void Transform::SetScale(DirectX::SimpleMath::Vector3 scale)
	{

	}

	void Transform::SetRotation(DirectX::SimpleMath::Quaternion rotation)
	{
		m_rotation = rotation;
		m_editorRotation = m_rotation.ToEuler();
	}

	DirectX::SimpleMath::Matrix Transform::GetMatrixLocal()
	{
		// âÒì]çsóÒ
		auto rotateMatrix = Matrix::CreateFromQuaternion(m_rotation);
		// ägèkçsóÒ
		auto scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
		// à⁄ìÆçsóÒ
		auto transformMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

		return scaleMatrix * rotateMatrix * transformMatrix;
	}
	DirectX::SimpleMath::Matrix Transform::GetMatrixRotation()
	{
		// âÒì]çsóÒ
		auto rotateMatrixX = DirectX::SimpleMath::Matrix::CreateRotationX(m_rotation.x);
		auto rotateMatrixY = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y);
		auto rotateMatrixZ = DirectX::SimpleMath::Matrix::CreateRotationZ(m_rotation.z);

		auto rotateMatrix = rotateMatrixX * rotateMatrixY * rotateMatrixZ;

		return rotateMatrix;
	}
}
