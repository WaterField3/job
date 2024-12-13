#include "EnemyMove.h"

#include "Imgui/imgui.h"

#include "GameObject/GameObjectManager.h"
#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "Rigidbody.h"

REGISTER_COMPONENT(TMF::EnemyMove, "EnemyMove");

namespace TMF
{
	void EnemyMove::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			auto playerObjects = GameObjectManager::Instance().GetGameObjects(GameObject::Tag::Player);
			auto size = playerObjects.size();
			for (auto i = 0; i < size; i++)
			{
				if (auto pPlayerObject = playerObjects[i].lock())
				{
					if (pLockOwner != pPlayerObject)
					{
						m_pPlayerRigidbody = pPlayerObject->GetComponent<Rigidbody>();
					}
				}
			}
		}
	}
	void EnemyMove::OnFinalize()
	{

	}
	void EnemyMove::OnUpdate()
	{
	}
	void EnemyMove::OnLateUpdate()
	{
		Move();
	}
	void EnemyMove::OnDraw()
	{

	}
	void EnemyMove::OnDrawImGui()
	{
		auto moveSpeedLabel = StringHelper::CreateLabel("MoveSpeed", m_uuID);
		if (ImGui::DragFloat(moveSpeedLabel.c_str(), &m_moveSpeed, 0.1f))
		{

		}
	}
	void EnemyMove::Move()
	{

		if (auto pLockRigidbody = m_pRigidbody.lock())
		{
			if (auto pLockPlayerRigidbody = m_pPlayerRigidbody.lock())
			{
				auto currentTransform = pLockRigidbody->GetBtTransform();
				
				auto targetPosition = pLockPlayerRigidbody->GetBtTransform().getOrigin();
				// 現在の位置を取得
				auto currentPosition = currentTransform.getOrigin();

				// ターゲット方向を計算
				btVector3 direction = targetPosition - currentPosition;
				direction.setY(0); // Y軸の変位を無視して水平面上で計算
				if (direction.length2() < 0.0001f) return; // 近ければ終了

				direction.normalize();

				// 現在のY軸に対する回転角度を計算
				auto s = currentTransform.getBasis();
				btVector3 currentForward = currentTransform.getBasis() * btVector3(0, 0, -1); // 前方向ベクトル
				currentForward.setY(0); // Y成分を無視して水平面上で計算
				currentForward.normalize();

				float angle = atan2(direction.getZ(), direction.getX()) - atan2(currentForward.getZ(), currentForward.getX());
				angle = btNormalizeAngle(angle); // -π ~ π の範囲に正規化

				// 回転補間を計算
				float rotationSpeed = 1.0f; // 回転速度
				float interpolatedAngle = angle  * rotationSpeed;
				btQuaternion rotationDelta(btVector3(0, -1, 0), interpolatedAngle);

				// 現在の回転を更新
				btQuaternion currentRotation = currentTransform.getRotation();
				currentRotation = rotationDelta * currentRotation; // 回転を更新
				currentRotation.normalize();

				// 新しい位置を計算
				btVector3 velocity = direction * m_moveSpeed;
				btVector3 newPosition = currentPosition + velocity;

				// トランスフォームを更新
				currentTransform.setRotation(currentRotation);
				currentTransform.setOrigin(newPosition);

				// モーションステートと剛体にトランスフォームを反映
				pLockRigidbody->SetBtTransform(currentTransform);
				pLockRigidbody->SetLinearVelocity(velocity);
			}
		}
	}
}
