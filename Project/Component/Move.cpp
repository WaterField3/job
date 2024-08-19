#include "Move.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"

namespace TMF
{
	REGISTER_COMPONENT(Move);
	Move::Move()
	{
	}
	Move::~Move()
	{
	}
	void Move::OnInitialize()
	{
	}
	void Move::OnFinalize()
	{
	}
	void Move::OnUpdate()
	{
		auto kb = Input::Instance().GetKeyState();
		auto tracker = Input::Instance().GetTracker();
		tracker->Update(kb);
		auto movePos = DirectX::SimpleMath::Vector3::Zero;
		auto jump = false;
		if (kb.W)
		{
			movePos += DirectX::SimpleMath::Vector3::Forward * m_moveSpeed;
		}
		if (kb.S)
		{
			movePos += DirectX::SimpleMath::Vector3::Backward * m_moveSpeed;
		}
		if (kb.A)
		{
			movePos += DirectX::SimpleMath::Vector3::Left * m_moveSpeed;
		}
		if (kb.D)
		{
			movePos += DirectX::SimpleMath::Vector3::Right * m_moveSpeed;
		}
		if (tracker->pressed.Space)
		{
			jump = true;
		}

		if (auto owner = m_pOwner.lock())
		{
			auto rigidBodyComponent = owner->GetComponent<Rigidbody>();
			if (auto rb = rigidBodyComponent.lock())
			{
				if (movePos != DirectX::SimpleMath::Vector3::Zero)
				{
					//rb->SetLinearVelocity(movePos);
					rb->ApplyCentralForce(movePos);
				}
				if (jump)
				{
					auto jumpPos = DirectX::SimpleMath::Vector3::Up * m_jumpPower;
					rb->ApplyImpulse(jumpPos, DirectX::SimpleMath::Vector3::Zero);
				}
			}
		}
	}
	void Move::OnLateUpdate()
	{
	}
	void Move::OnDraw()
	{
	}
	void Move::OnDrawImGui()
	{
		if (ImGui::DragFloat("MoveSpeed", &m_moveSpeed, 0.1f, 0.0f))
		{

		}
		if (ImGui::DragFloat("JumpPower", &m_jumpPower, 0.1f, 0.0f))
		{

		}
	}
	boost::uuids::uuid Move::OnGetUUID()
	{
		return m_uuID;
	}
}