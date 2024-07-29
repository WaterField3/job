#include "Transform.h"

#include <Imgui/imgui.h>

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
		label += "## " + boost::uuids::to_string(m_uuID);
		if (ImGui::DragFloat3(label.c_str(), &m_position.x, 0.1f))
		{

		}
		label = "Scale";
		label += "## " + boost::uuids::to_string(m_uuID);
		if (ImGui::DragFloat3(label.c_str(), &m_scale.x, 0.1f))
		{

		}
		label = "Rotation";
		label += "## " + boost::uuids::to_string(m_uuID);
		if (ImGui::DragFloat3(label.c_str(), &m_rotation.x, 0.1f))
		{

		}
	}

	void Transform::SetPosition(DirectX::SimpleMath::Vector3 pos)
	{

	}

	void Transform::SetScale(DirectX::SimpleMath::Vector3 scale)
	{

	}

	void Transform::SetRotation(DirectX::SimpleMath::Vector3 rotation)
	{

	}

	DirectX::SimpleMath::Matrix Transform::GetMatrixLocal()
	{
		// âÒì]çsóÒ
		auto rotateMatrixX = DirectX::SimpleMath::Matrix::CreateRotationX(m_rotation.x);
		auto rotateMatrixY = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.y);
		auto rotateMatrixZ = DirectX::SimpleMath::Matrix::CreateRotationY(m_rotation.z);

		auto rotateMatrix = rotateMatrixX * rotateMatrixY * rotateMatrixZ;
		// ägèkçsóÒ
		auto scaleMatrix = DirectX::SimpleMath::Matrix::CreateScale(m_scale);
		// à⁄ìÆçsóÒ
		auto transformMatrix = DirectX::SimpleMath::Matrix::CreateTranslation(m_position);

		return scaleMatrix * rotateMatrix * transformMatrix;
	}
}
