#include "Collider.h"

#include <Imgui/imgui.h>
// windows‚Ìmin max‚ðŽg—p‚µ‚È‚¢‚æ‚¤‚É‚·‚é
#undef min
#undef max
#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
#include <random>

#include "Transform.h"
#include "Rigidbody.h"
#include "GhostObject.h"
#include "Utility/StringHelper.h"
#include "Utility/Log.h"

REGISTER_COMPONENT(TMF::Collider, "Collider");

namespace TMF
{

	void Collider::OnInitialize()
	{
		//AddRigidBody();
		m_pCompaundShape = std::make_shared<btCompoundShape>();
		MakeCollision();
	}
	void Collider::OnFinalize()
	{
		auto pLockOwner = m_pOwner.lock();
		auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
		if (auto pLockRigidbody = pRigidbody.lock())
		{
			pLockRigidbody->RemoveRigidbody();
		}
		auto pGhostObject = pLockOwner->GetComponent<GhostObject>();
		if (auto pLockGhostObject = pGhostObject.lock())
		{
			pLockGhostObject->RemoveGhostObject();
		}
	}
	void Collider::OnUpdate()
	{

	}
	void Collider::OnLateUpdate()
	{

	}
	void Collider::OnDraw()
	{

	}
	void Collider::OnDrawImGui()
	{
		const char* types[] = { "Box", "Capsule", "Sphere", "Cylinder", "Cone", "Plane","ConvexHull", "Terrain" };
		int selectIndex = (int)m_collidrType;
		auto shapeLabel = StringHelper::CreateLabel("ColliderType", m_uuID);
		if (ImGui::BeginCombo(shapeLabel.c_str(), types[selectIndex]))
		{
			for (int i = 0; i < IM_ARRAYSIZE(types); i++)
			{
				auto selected = ((int)m_collidrType == i);
				if (ImGui::Selectable(types[i], selected))
				{
					m_collidrType = Collider_Type(i);
					selectIndex = i;
					UpdateShapeInfo();

				}
				if (selected)
				{
					ImGui::SetItemDefaultFocus();
				}
			}
			ImGui::EndCombo();
		}

		auto sizeLabel = StringHelper::CreateLabel("Size", m_uuID);
		if (ImGui::DragFloat3(sizeLabel.c_str(), &m_size.x, 0.1f, 0.0f))
		{
			UpdateShapeInfo();
		}
		auto centerLabel = StringHelper::CreateLabel("Center", m_uuID);
		if (ImGui::DragFloat3(centerLabel.c_str(), &m_center.x, 0.1f))
		{
			UpdateShapeInfo();
		}

		if (ImGui::Button("Update"))
		{
			UpdateShapeInfo();
		}
	}

	void Collider::MakeCollision()
	{
		auto btscale = btVector3(1.0f, 1.0f, 1.0f);
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto scale = pLockTransform->GetScale();
				scale.x = std::abs(scale.x);
				scale.y = std::abs(scale.y);
				scale.z = std::abs(scale.z);
				scale *= m_size;
				btscale.setValue(scale.x, scale.y, scale.z);
			}
		}
		btScalar radius = btscale.getX();
		btScalar height = btscale.getY();
		auto btNormal = btVector3(0.0f, 1.0f, 0.0f);
		std::vector<btVector3> vertices;
		vertices.push_back(btVector3(-1, 0, -1));
		vertices.push_back(btVector3(1, 0, -1));
		vertices.push_back(btVector3(1, 0, 1));
		vertices.push_back(btVector3(-1, 0, 1));

		vertices.push_back(btVector3(0, 2, 0));

		const int width = 8;
		const int length = 8;
		float heightData[width * length] = { 1, 1, 2, 3,
											2, 2, 1, 1,
											1, 1, 2, 2,
											1, 1, 3, 1,
											1, 1, 2, 3,
											2, 2, 1, 1,
											1, 1, 2, 2,
											1, 1, 3, 1
		};
		//	for (int x = 0; x < width; ++x) {

	//for (int y = 0; y < length; ++y) {
//		heightData[y * width + x] = std::sin(x * 0.1f) * std::cos(y * 0.1f) ; // U•10‚Ì”gó’nŒ`
//	}
//}
		// m_pCollisionShape‚Ìì¬
		switch (m_collidrType)
		{
		case TMF::Collider::Collider_Type::BOX:
			m_pCollisionShape = std::make_shared<btBoxShape>(btscale);
			break;
		case TMF::Collider::Collider_Type::CAPSULE:
			m_pCollisionShape = std::make_shared<btCapsuleShape>(radius, height);
			break;
		case TMF::Collider::Collider_Type::SPHERE:
			m_pCollisionShape = std::make_shared<btSphereShape>(radius);
			break;
		case TMF::Collider::Collider_Type::CYLINDER:
			m_pCollisionShape = std::make_shared<btCylinderShape>(btscale);
			break;
		case TMF::Collider::Collider_Type::CONE:
			m_pCollisionShape = std::make_shared<btConeShape>(radius, height);
			break;
		case TMF::Collider::Collider_Type::PLANE:
			m_pCollisionShape = std::make_shared<btStaticPlaneShape>(btNormal, 1.0f);
			break;
		case TMF::Collider::Collider_Type::CONVEXHULL:
		{
			m_pCollisionShape = std::make_shared<btConvexHullShape>();
			auto pVoidCollision = static_cast<void*>(m_pCollisionShape.get());
			for (const auto& vertex : vertices)
			{
				static_cast<btConvexHullShape*>(pVoidCollision)->addPoint(vertex);
			}
			break;
		}
		case TMF::Collider::Collider_Type::TERRAIN:
		{
			m_pCollisionShape = std::make_shared<btHeightfieldTerrainShape>(width, length, heightData, 1.0f, -1.0f, 1.0f, 1, PHY_FLOAT, false);
			auto pVoidCollision = static_cast<void*>(m_pCollisionShape.get());
			static_cast<btHeightfieldTerrainShape*>(pVoidCollision)->setLocalScaling(btVector3(3.0f, 1.0f, 3.0f));
			break;
		}
		default:
			break;
		}

		SetCompoundCollisionShape();
	}
	std::weak_ptr<btCollisionShape> Collider::GetCollisionShape()
	{
		if (m_pCollisionShape.get() == nullptr)
		{
			MakeCollision();
		}
		return m_pCompaundShape;
	}

	void Collider::SetCollisionCenter(DirectX::SimpleMath::Vector3 setCenter)
	{
		m_center = setCenter;
		UpdateShapeInfo();
	}

	void Collider::SetCollisionScale(DirectX::SimpleMath::Vector3 setScale)
	{
		m_size = setScale;
		UpdateShapeInfo();
	}

	void Collider::UpdateShapeInfo()
	{
		MakeCollision();
		SetCompoundCollisionShape();
	}
	void Collider::AddRigidBody()
	{
		auto pLockOwner = m_pOwner.lock();
		auto pTransform = pLockOwner->GetComponent<Transform>();
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto rotate = DirectX::SimpleMath::Quaternion::Identity;
		if (auto pLockTransform = pTransform.lock())
		{
			pos = pLockTransform->GetWorldPosition();
			pos += m_center;
			rotate = pLockTransform->GetRotation();
		}
		auto pRigidbody = pLockOwner->GetComponent<Rigidbody>();
		if (auto pLockRigidbody = pRigidbody.lock())
		{
			//rb->RemoveRigidBody();
			MakeCollision();
			//rb->AddRigidBody(m_pCompaundShape, pos, rotate);
		}
	}
	void Collider::AddGhostObject()
	{
		auto pLockOwner = m_pOwner.lock();
		auto pTransform = pLockOwner->GetComponent<Transform>();
		auto pos = DirectX::SimpleMath::Vector3::Zero;
		auto rotate = DirectX::SimpleMath::Quaternion::Identity;
		if (auto pLockTransform = pTransform.lock())
		{
			pos = pLockTransform->GetWorldPosition();
			pos += m_center;
			rotate = pLockTransform->GetRotation();
		}
		auto pGhostObject = pLockOwner->GetComponent<GhostObject>();
		if (auto pLockGhostObject = pGhostObject.lock())
		{
			pLockGhostObject->RemoveGhostObject();
			MakeCollision();
			pLockGhostObject->AddGhostObject(m_pCollisionShape, pos, rotate);
		}
	}

	// m_pCompoundShape‚Ém_pColliderShape‚ð“o˜^ˆêŒÂ‚Ì‚Ý
	void Collider::SetCompoundCollisionShape()
	{
		if (auto pLockOwner = m_pOwner.lock())
		{
			auto pTransform = pLockOwner->GetComponent<Transform>();
			if (auto pLockTransform = pTransform.lock())
			{
				auto rotate = pLockTransform->GetRotation();
				btTransform btTrans;
				btTrans.setIdentity();
				btTrans.setOrigin(btVector3(m_center.x, m_center.y, m_center.z));
				auto size = m_pCompaundShape->getNumChildShapes();
				if (size != 0)
				{
					for (auto i = 0; i < size; i++)
					{
						m_pCompaundShape->removeChildShapeByIndex(i);
					}
				}
				m_pCompaundShape->addChildShape(btTrans, m_pCollisionShape.get());
			}
		}
	}
}