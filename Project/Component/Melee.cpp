#include "Melee.h"

#include <Imgui/imgui.h>

#include "Animater.h"
#include "GameObject/GameObjectManager.h"
#include "Utility/StringHelper.h"
#include "Transform.h"
#include "PrimitiveMesh.h"
#include "BulletStraightMove.h"

REGISTER_COMPONENT(TMF::Melee, "Melee");

namespace TMF
{
	void Melee::OnInitialize()
	{
	}
	void Melee::OnFinalize()
	{
	}
	void Melee::OnUpdate()
	{
	}
	void Melee::OnLateUpdate()
	{
	}
	void Melee::OnDraw()
	{
	}
	void Melee::OnDrawImGui()
	{
		char buf[256] = "";
		strcpy_s(buf, sizeof(buf), m_meleeAnimation.c_str());
		auto meleeAnimPath = StringHelper::CreateLabel("AnimPath", m_uuID);
		if (ImGui::InputText(meleeAnimPath.c_str(), buf, 256))
		{
			m_meleeAnimation = buf;
		}

		auto playLabel = StringHelper::CreateLabel("Play", m_uuID);
		if (ImGui::Button(playLabel.c_str()))
		{
			Play();
		}
	}
	void Melee::Play()
	{
		//if (auto pLockOwner = m_pOwner.lock())
		//{
		//	auto thisObjPosition = DirectX::SimpleMath::Vector3::Zero;
		//	auto pTransform = pLockOwner->GetComponent<Transform>();

		//	if (auto pLockTransrom = pTransform.lock())
		//	{
		//		thisObjPosition = pLockTransrom->GetPosition();

		//	}
		//	// 当たり判定用のオブジェクトを生成
		//	auto pHitObj = GameObjectManager::Instance().CreateGameObject();
		//	if (auto pLockHitObj = pHitObj.lock())
		//	{
		//		auto pHitObjTransform = pLockHitObj->GetComponent<Transform>();
		//		if (auto pLockHitObjTransform = pHitObjTransform.lock())
		//		{
		//			pLockHitObjTransform->SetPosition(thisObjPosition);
		//		}
		//		pLockHitObj->SetName("hitobj");
		//		pLockHitObj->AddComponent<Melee>();
		//		pLockHitObj->AddComponent<PrimitiveMesh>();
		//		auto pHitStraightMove = pLockHitObj->AddComponent<BulletStraightMove>();
		//		if (auto pLockHitStraightMove = pHitStraightMove.lock())
		//		{
		//			pLockHitStraightMove->SetMoveVector(DirectX::SimpleMath::Vector3::Backward);
		//		}
		//	}
		//}
		GameObjectManager::Instance().LoadObject("obj");
	}
}