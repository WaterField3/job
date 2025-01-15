#include "EnemyMove.h"

#include "Imgui/imgui.h"

#include "ComponentRegister.h"
#include "Utility/StringHelper.h"
#include "GameObject/GameObjectManager.h"
#include "Rigidbody.h"
#include "Transform.h"

REGISTER_COMPONENT(TMF::EnemyMove, "EnemyMove");

namespace TMF
{
	void EnemyMove::OnInitialize()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			m_pRigidbody = pLockOwner->GetComponent<Rigidbody>();
			m_pTransform = pLockOwner->GetComponent<Transform>();
			auto playerObjects = GameObjectManager::Instance().GetGameObjects(GameObject::Tag::Player);
			auto size = playerObjects.size();
			for (auto i = 0; i < size; i++)
			{
				if (auto pPlayerObject = playerObjects[i].lock())
				{
					if (pLockOwner != pPlayerObject)
					{
						m_pPlayerRigidbody = pPlayerObject->GetComponent<Rigidbody>();
						m_pPlayerTransform = pPlayerObject->GetComponent<Transform>();
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
		if (auto pLockTransform = m_pTransform.lock())
		{
			if (auto pLockPlayerTransform = m_pPlayerTransform.lock())
			{
				auto thisPosition = pLockTransform->GetPosition();
				auto playerPosition = pLockPlayerTransform->GetPosition();
				m_playerDistance = DirectX::SimpleMath::Vector3::Distance(thisPosition, playerPosition);
			}
		}
	}
	void EnemyMove::OnLateUpdate()
	{

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
				// ���݂̈ʒu���擾
				auto currentPosition = currentTransform.getOrigin();

				// �^�[�Q�b�g�������v�Z
				btVector3 direction = targetPosition - currentPosition;
				direction.setY(0); // Y���̕ψʂ𖳎����Đ����ʏ�Ōv�Z
				if (direction.length2() < 0.0001f) return; // �߂���ΏI��

				direction.normalize();

				// ���݂�Y���ɑ΂����]�p�x���v�Z
				auto s = currentTransform.getBasis();
				btVector3 currentForward = currentTransform.getBasis() * btVector3(0, 0, -1); // �O�����x�N�g��
				currentForward.setY(0); // Y�����𖳎����Đ����ʏ�Ōv�Z
				currentForward.normalize();

				float angle = atan2(direction.getZ(), direction.getX()) - atan2(currentForward.getZ(), currentForward.getX());
				angle = btNormalizeAngle(angle); // -�� ~ �� �͈̔͂ɐ��K��

				// ��]��Ԃ��v�Z
				float rotationSpeed = 1.0f; // ��]���x
				float interpolatedAngle = angle  * rotationSpeed;
				btQuaternion rotationDelta(btVector3(0, -1, 0), interpolatedAngle);

				// ���݂̉�]���X�V
				btQuaternion currentRotation = currentTransform.getRotation();
				currentRotation = rotationDelta * currentRotation; // ��]���X�V
				currentRotation.normalize();

				// �V�����ʒu���v�Z
				btVector3 velocity = direction * m_moveSpeed;
				btVector3 newPosition = currentPosition + velocity;

				// �g�����X�t�H�[�����X�V
				currentTransform.setRotation(currentRotation);
				currentTransform.setOrigin(newPosition);

				// ���[�V�����X�e�[�g�ƍ��̂Ƀg�����X�t�H�[���𔽉f
				pLockRigidbody->SetBtTransform(currentTransform);
				pLockRigidbody->SetLinearVelocity(velocity);

				//pLockPlayerRigidbody->SetRigidBodyTranform(DirectX::SimpleMath::Vector3(newPosition), DirectX::SimpleMath::Quaternion(currentRotation));
			}
		}
	}
}
