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
				float movementSpeed = 5.0f; // �ړ����x
				btVector3 velocity = direction * movementSpeed;
				btVector3 newPosition = currentPosition + velocity;

				// �g�����X�t�H�[�����X�V
				currentTransform.setRotation(currentRotation);
				currentTransform.setOrigin(newPosition);

				// ���[�V�����X�e�[�g�ƍ��̂Ƀg�����X�t�H�[���𔽉f
				pLockRigidbody->SetBtTransform(currentTransform);
				pLockRigidbody->SetLinearVelocity(velocity);

				//auto playerPos = pLockPlayerRigidbody->GetBtTransform().getOrigin();

				//// ���݈ʒu�ƃ^�[�Q�b�g�ʒu
				//auto currentPosition = pLockRigidbody->GetBtTransform().getOrigin();
				//auto currentVelocity = pLockRigidbody->GetLinearVelocity();

				//auto direction = playerPos - currentPosition;
				//direction.normalize();

				//// ���݂̑O�����Ɖ�]�̌v�Z
				//auto currentForward = pLockRigidbody->GetBtTransform().getBasis() * btVector3(0, 1, 0);
				//if (!direction.fuzzyZero())
				//{

				//	btQuaternion rotation = btQuaternion(currentForward.cross(direction).normalize(),currentForward.angle(direction));
				//	btTransform transform = pLockRigidbody->GetBtTransform();
				//	//pLockRigidbody->SetRotation(rotation);
				//	transform.setRotation(rotation);
				//	pLockRigidbody->SetBtTransform(transform);
				//}
				//// �ړ����x�̐ݒ�
				//btVector3 velocity = direction * m_moveSpeed;
				//velocity.setY(currentVelocity.y);
				//pLockRigidbody->SetLinearVelocity(velocity);

			}
		}
	}
}
