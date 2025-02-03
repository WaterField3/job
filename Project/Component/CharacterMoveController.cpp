#include "CharacterMoveController.h"

#include <Imgui/imgui.h>

#include "Transform.h"
#include "Rigidbody.h"
#include "Input.h"
#include "Melee.h"
#include "Shot.h"
#include "Dodge.h"
#include "PlayerStatus.h"
#include "MoveInfo.h"
#include "Thruster.h"
#include "Jump.h"
#include "Camera.h"
#include "GameObject/GameObjectManager.h"
#include "FreeCamera.h"
#include "CameraMove.h"
#include "EventSystem/EventSystem.h"
#include "PlayerMove.h"
#include "State/StateMachine.h"

REGISTER_COMPONENT(TMF::CharacterMoveController, "CharacterMoveController");

namespace TMF
{
	CharacterMoveController::CharacterMoveController()
	{
	}
	CharacterMoveController::~CharacterMoveController()
	{
	}
	void CharacterMoveController::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pTransform = pLockOwner->GetComponent<Transform>();
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			m_pStateMachine = std::make_shared<StateMachine>(pLockOwner);
			if (m_pStateMachine)
			{
				m_pStateMachine->ChangeState("PlayerIdleState");
			}
		}
	}
	void CharacterMoveController::OnFinalize()
	{
	}
	void CharacterMoveController::OnUpdate()
	{
		auto kb = Input::Instance().GetKeyState();
		auto tracker = Input::Instance().GetKeyboardTracker();
		tracker->Update(kb);


		m_pStateMachine->Update();

		// åªç›éûä‘
		auto now = duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());

		if (tracker->pressed.V == true)
		{
			// àÍíËéûä‘ì‡Ç…ï°êîâÒâüÇ≥ÇÍÇƒÇ¢ÇÈÇ©
			if (Input::Instance().PluralGetKeyDiwn(now) == true)
			{
				m_pStateMachine->ChangeState("PlayerDodgeState");
			}
		}

		if (tracker->pressed.H)
		{
			GameObjectManager::Instance().LoadObject("box");
		}

		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pPlayerStatus = pLockOwner->GetComponent<PlayerStatus>();
			auto isInvincible = false;
			if (auto pLockPlayerStatus = pPlayerStatus.lock())
			{
				isInvincible = pLockPlayerStatus->GetIsInvincible();
			}
		}

	}
	void CharacterMoveController::OnLateUpdate()
	{
	}
	void CharacterMoveController::OnDraw()
	{
	}
	void CharacterMoveController::OnDrawImGui()
	{
		if (ImGui::DragFloat("MoveSpeed", &m_moveSpeed, 0.1f, 0.0f))
		{

		}
		if (ImGui::DragFloat("JumpPower", &m_jumpPower, 0.1f, 0.0f))
		{

		}
		if (ImGui::DragFloat3("checkVec3", &checkvec3.x))
		{

		}
	}
	std::shared_ptr<Component> CharacterMoveController::OnClone() const
	{
		auto pClone = std::make_shared<CharacterMoveController>();
		pClone->m_moveSpeed = this->m_moveSpeed;
		pClone->m_jumpPower = this->m_jumpPower;
		return move(pClone);
	}
}